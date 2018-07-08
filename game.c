#include "game.h"

/* Semaphores */
void init(sem_t *s, uint32_t count)
{
	*s = count;
}

void wait(sem_t *s)
{
	__disable_irq();
	while(*s ==0)	{
		__enable_irq();
		__disable_irq();
	}
	(*s)--;
	__enable_irq();
}

void signal(sem_t *s)
{
	__disable_irq();
	(*s)++;
	__enable_irq();
}
/* Semaphores */

/* Tasks */
__task void mainTask(void)
{
	// Create tasks
	os_tsk_create(updateDisplay, 1);

	os_tsk_create(updateFuelStatus, 2);
	os_tsk_create(moveRobot, 2);

	os_tsk_create(buyFuel, 3);

	os_tsk_delete_self();
}

__task void updateDisplay(void)
{
	os_itv_set(10);
	
	// Declare current row and column
	uint32_t row, col;

	while(1)
	{
		// Wait until something has changed on the screen
		wait(&action_performed);

		GLCD_SetTextColor(Blue);
		GLCD_SetBackColor(Blue);
		GLCD_Clear(Blue);

		for (row=min_row; row<max_row; row++)
		{
			for (col=min_col; col<max_col; col++)
			{
				// Print char value of array element at {row, col} on LCD Display
				output_bmp(row, col);
			}
		}
		// Signal that LCD Display has been redrawn	
		signal(&display_refreshed);
		os_tsk_pass();
	}
}

__task void moveRobot(void)
{

	// TODO: Implement points system

	uint32_t cur_state = 0;
	uint32_t prev_state = 0;

	os_itv_set(10);

	while(1)
	{
		pollJoystick();
		pollPushbutton();

		cur_state = robot.dir;

		if (cur_state != prev_state)
		{
			switch(robot.dir)
			{
				case RIGHT:
					if (robot.x_pos+1 < MAX_SCREEN_LENGTH)
					{
						robot.x_pos++;
					}
					break;
				case LEFT:
					if (robot.x_pos-1 >= SURFACE)
					{
						robot.x_pos--;
					}
					break;
				case UP:
					if(robot.is_flying)
					{
						if (robot.y_pos-1 >= 0)
						{
							robot.y_pos--;
						}
					}
					break;
				case DOWN:
					if (!robot.is_flying)
					{
						if (robot.y_pos+1 <= max_col)
						{
							robot.y_pos++;
						}
					}
					break;
			}
		}
		prev_state = cur_state;

		// Check if map boundaries need to be updated
		if (robot.x_pos > max_row)
		{
			min_row += 3;
			max_row += 3;
		}
		if (robot.x_pos < min_row)
		{
			min_row -= 3;
			max_row -= 3;
		}

		os_tsk_pass();
	}
}

__task void buyFuel(void)
{
	os_itv_set(10);

	uint32_t num_bars;
	uint32_t cost;

	while(1)
	{
		if (map[robot.x_pos][robot.y_pos] == 'F' && robot.select_action)
		{
			num_bars = NUM_LEDS - robot.fuel_status;
			cost = 2*num_bars;
			robot.fuel_status = NUM_LEDS;
			robot.num_points -= cost;
		}
		os_tsk_pass();
	}
}

__task void updateFuelStatus(void)
{
	os_itv_set(10);

	uint32_t fuel_time_next = 0;
	uint32_t fuel_consumption_rate = FUEL_TIME;

	while(1)
	{
		// Perhaps add mutex to control which task (updateFuel or buyFuel) gets to
		// update robot.fuel_status

		// Lose 1 bar of fuel every 10s (5s if robot is flying)
		fuel_consumption_rate = robot.is_flying ? 0.5*FUEL_TIME : FUEL_TIME;
		// Decrement fuel tank by 1 per period
		if (ms_ticks - fuel_time_next >= fuel_consumption_rate)
		{
			robot.fuel_status--;
			fuel_time_next += fuel_consumption_rate;
		}
		// Add condition where game ends when robot.fuel_status <= 0
		if (robot.fuel_status == 0)
		{
			gameOver = true;
			// gameOver();
		}
		setLED(robot.fuel_status);
		os_tsk_pass();
	}
	/* Put this in main()
	SysTick_Config(SystemCoreClock/1000);
	*/

}
/* Tasks */

/* Functions */
inline void pollJoystick(void)
{
	// Value read from joystick register
	uint32_t joystick_val;
	// Bits that indicate direction of joystick
	uint32_t direction_mask = 15 << 23;
	// Bit indicates if joystick button pressed
	uint32_t button_mask = 1 << 20;

	// Read from Joystick Register
	joystick_val = LPC_GPIO1 -> FIOPIN;

	// robot.dir will take values of either UP, DOWN, LEFT, RIGHT, or a random value
	robot.dir = (~joystick_val & direction_mask);
	// Joystick button bit is inverted
	robot.select_action = joystick_val & button_mask ? false:true;
}

inline void pollPushbutton(void)
{
	uint32_t val = LPC_GPIO2 -> FIOPIN;
	uint32_t mask = 1 << 10;

	robot.is_flying = (~val & mask) ? true : false;
}

inline void setLED(uint32_t val)
{
	/* Remember to transfer this code to main()
	// Set Pins on GPIO1 and GPIO2 to output
	LPC_GPIO1 -> FIODIR = 0xB0000000;
	LPC_GPIO2 -> FIODIR = 0x0000007C;
	*/
	uint32_t lower_mask = (1 << 3) - 1;
	uint32_t upper_mask = ((1<<8)- 1) & (~lower_mask);

	if (val > NUM_LEDS)
	{
		val = NUM_LEDS;
	}

	uint32_t num = (1 << val) - 1;

	// Clear LEDs corresponding to empty fuel bars
	uint32_t gpio_1_clr = (~num) & lower_mask;
	uint32_t gpio_2_clr = (~num) & upper_mask;

	uint32_t fioclr1 = 0x0 | ((gpio_1_clr & 0x3)<<28) | ((gpio_1_clr & 0x4)<<29);
	uint32_t fioclr2 = 0x0 | (gpio_2_clr>>1);
	LPC_GPIO1 -> FIOCLR = fioclr1;
	LPC_GPIO2 -> FIOCLR = fioclr2;

	// Set LEDs corresponding to available fuel bars
	uint32_t gpio_1_set = num & lower_mask;
	uint32_t gpio_2_set = num & upper_mask;

	uint32_t fioset1 = 0x0 | ((gpio_1_set & 0x3)<<28) | ((gpio_1_set & 0x4)<<29);
	uint32_t fioset2 = 0x0 | (gpio_2_set>>1);
	LPC_GPIO1 -> FIOSET = fioset1;
	LPC_GPIO2 -> FIOSET = fioset2;
}


void loadBMP(int row, int col)
{
	unsigned char *output_bmp;
	switch(map[row][col])
	{
		// E R D G S C P X F
		// Dirt, gold, emerald, copper, sky, rock, path,fuel
		// TODO: digger in black path, digger in sky,
		case 'S':
			output_bmp = (unsigned char *)sky_bmp;
			break;
		case 'R':
			output_bmp = (unsigned char *)rock_bmp;
			break;
		case 'D':
			output_bmp = (unsigned char *)dirt_bmp;
			break;
		case 'G':
			output_bmp = (unsigned char *)gold_bmp;
			break;
		case 'E':
			output_bmp = (unsigned char *)emerald_bmp;
			break;
		case 'C':
			output_bmp = (unsigned char *)copper_bmp;
			break;
		case 'F':
			output_bmp = (unsigned char *)fuel_bmp;
			break;
		case 'X':
			if (row <= SURFACE)
			{
				if (robot.dir == LEFT)
					output_bmp = (unsigned char *)x_sky_left_bmp;
				else
					output_bmp = (unsigned char *)x_sky_right_bmp;
			}
			else
			{
				if (robot.dir == LEFT)
					output_bmp = (unsigned char *)x_path_left_bmp;
				else
					output_bmp = (unsigned char *)x_path_right_bmp;
			}
			break;
		default:
			output_bmp = (unsigned char *)path_bmp;
			break;
	}

	GLCD_Bitmap(row,col,40,40, output_bmp);
}

void SysTick_Handler(void)
{
	ms_ticks++;
}
/* Functions */
