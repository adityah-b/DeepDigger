#include <lpc17xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <RTL.h>
#include "uart.h"
#include <stdint.h>
#include "game.h"
#include "GLCD.h"
#include "timer.h"


//#include "game_bitmaps.h"

/* Semaphores */
void init(sem_t *s, uint32_t count)
{
	*s = count;
}

void wait(sem_t *s)
{
	__disable_irq();
	while(*s == 0)	{
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

sem_t action_performed, display_refreshed;
sem_t mutt;
/* Semaphores */

/* Tasks */
__task void mainTask(void)
{
	// Create tasks
	os_tsk_create(updateDisplay, 1);

	os_tsk_create(moveRobot, 2);
	os_tsk_create(updateFuelStatus, 2);

	os_tsk_create(buyFuel, 3);

	os_tsk_delete_self();
}

__task void updateDisplay(void)
{
	// Declare current row and column
	uint32_t row, col;
	//uint32_t count = 0;
	os_itv_set(10);

	while(!game_over)
	{
		// Wait until something has changed on the screen
		//wait(&action_performed);
		//printf("Entered updateDisplay\n");
		//while (mutt == 0);
		//printf("Mutex value in updateDisplay: %d\n", mutt);
		
		//GLCD_SetTextColor(Blue);
		//GLCD_SetBackColor(Blue);
		//GLCD_Clear(Blue);

		printf("min_row: %d, max_row: %d\n", min_row, max_row);
		for (row=min_row; row<max_row; row++)
		{
			for (col=min_col; col<max_col; col++)
			{
				// Print char value of array element at {row, col} on LCD Display
				//printf("Row: %d, Col: %d\n", row, col);
				loadBMP(row, col);
				//while(count <= 1000000) count++;
				//if (count > 1000000) count = 0;
			}
		}
		// Signal that LCD Display has been redrawn	
		//signal(&display_refreshed);
		mutt--;
		//printf("Mutex value at end of updateDisplay: %d\n", mutt);
		os_tsk_pass();
		os_itv_wait();
	}
    
    if (game_over)
    {
        endGameDisplay();
    }
}

__task void moveRobot(void)
{

	// TODO: Implement points system, implement map update, implement rock boundary conditions, implement is_flying state condition

	uint32_t cur_state = 0;
	uint32_t prev_state = 0;
	unsigned char* direction;
	os_itv_set(50);

	while(!game_over)
	{
		// wait(&display_refreshed);
		//printf("Entered moveRobot\n");
		//while (mutt == 1);
		//printf("Mutex value in moveRobot: %d\n", mutt);
		int32_t x_pos_next=0, y_pos_next=0;
		pollJoystick();
		printf("isFlying: %d\n", robot.is_flying);
		cur_state = robot.dir;

		if (cur_state != prev_state)
		{
			switch(robot.dir)
			{
				case DOWN:
					if (!robot.is_flying)
					{
						if (robot.x_pos+1 < MAX_SCREEN_LENGTH)
						{
							map[robot.x_pos][robot.y_pos] = robot.x_pos == SURFACE ? 'S' : 'P';
							x_pos_next = 1;
							direction = "DOWN";
						}
					}
					break;
				case UP:
					if(robot.is_flying)
					{
						if (robot.x_pos-1 >= SURFACE)
						{
							map[robot.x_pos][robot.y_pos] = 'P';
							x_pos_next = -1;
							direction = "UP";
						}
					}
					break;
				case LEFT:
					if (robot.y_pos-1 >= 0)
					{
						map[robot.x_pos][robot.y_pos] = robot.x_pos == SURFACE ? 'S' : 'P';
						y_pos_next = -1;
						direction = "LEFT";
					}
					break;
				case RIGHT:
					if (robot.y_pos+1 <= max_col)
					{
						map[robot.x_pos][robot.y_pos] = robot.x_pos == SURFACE ? 'S' : 'P';
						y_pos_next = 1;
						direction = "RIGHT";
					}
					break;
			}
			
			if (robot.x_pos == fuel_x && robot.y_pos == fuel_y)
				map[robot.x_pos][robot.y_pos] = 'F';

			robot.x_pos += x_pos_next;
			robot.y_pos += y_pos_next;
			
			if (robot.is_flying && map[robot.x_pos][robot.y_pos] != 'P')
			{
				if (map[robot.x_pos][robot.y_pos] == 'S' || map[robot.x_pos][robot.y_pos] == 'F')
				{
					map[robot.x_pos][robot.y_pos] = 'X';
				}
				else
				{
					robot.x_pos -= x_pos_next;
					robot.y_pos -= y_pos_next;
				}
			}
			else if (map[robot.x_pos][robot.y_pos] != 'R')
			{
                if (map[robot.x_pos][robot.y_pos] == 'G')
                {
                    robot.num_gold++;
                    robot.num_points+=10;
                }
                else if (map[robot.x_pos][robot.y_pos] == 'E')
                {
                    robot.num_emerald++;
                    robot.num_points+=5;
                }
                else if (map[robot.x_pos][robot.y_pos] == 'C')
                {
                    robot.num_copper++;
                    robot.num_points+=1;
                }
				map[robot.x_pos][robot.y_pos] = 'X';
			}
			else
			{
				robot.x_pos -= x_pos_next;
				robot.y_pos -= y_pos_next;
			}

            if (robot.num_points > END_GAME_THRESHOLD)
            {
                game_over = true;
                robot.game_won = true;
            }
		}
		//printf("DIRECTION: %s, robot.dir: %d, pushbutton: %d\n", direction, robot.dir, robot.is_flying);
		prev_state = cur_state;

		// Check if map boundaries need to be updated
		if (robot.x_pos >= max_row && max_row+3 <MAX_SCREEN_LENGTH)
		{
			min_row += 3;
			max_row += 3;
            num_slides++;
		}
		if (robot.x_pos <= min_row && min_row-3 >= SURFACE)
		{
			min_row -= 3;
			max_row -= 3;
            num_slides--;
		}
		
		//signal(&action_performed);
		
		mutt++;
		//printf("RobotX: %d, RobotY: %d\n", robot.x_pos, robot.y_pos);
		printf("MAP: %c, JoyButt: %d\n", map[robot.x_pos][robot.y_pos], robot.select_action);
		os_tsk_pass();
		os_itv_wait();
	}
}

__task void buyFuel(void)
{
	uint32_t num_bars;
	uint32_t cost;
	
	os_itv_set(200);

	while(!game_over)
	{
		if (robot.x_pos == fuel_x && robot.y_pos == fuel_y && robot.select_action)
		{
			num_bars = NUM_LEDS - robot.fuel_status;
			cost = 2*num_bars;
			robot.fuel_status = NUM_LEDS;
			robot.num_points -= cost;
		}
		os_tsk_pass();
		os_itv_wait();
	}
}

__task void updateFuelStatus(void)
{
	uint32_t fuel_time_cur = 0;
	uint32_t fuel_time_next = 0;
	uint32_t fuel_consumption_rate = FUEL_TIME;
	
	os_itv_set(200);

	while(!game_over)
	{
		//printf("ENTERED FUEL UPDATE\n");
		// Perhaps add mutex to control which task (updateFuel or buyFuel) gets to
		// update robot.fuel_status

		// Lose 1 bar of fuel every 10s (5s if robot is flying)
		fuel_consumption_rate = robot.is_flying ? 0.5*FUEL_TIME : FUEL_TIME;
		// Decrement fuel tank by 1 per period
		fuel_time_cur = timer_read()/1E6;
		//printf("fuel_time_cur: %d\n", fuel_time_cur);
		if (fuel_time_cur - fuel_time_next >= fuel_consumption_rate)
		{
			printf("Entered 10s condition, robot_fuel_status: %d\n", robot.fuel_status);
			robot.fuel_status--;
			fuel_time_next += fuel_consumption_rate;
		}
		// Add condition where game ends when robot.fuel_status <= 0
		if (robot.fuel_status <= 0)
		{
			game_over = true;
            robot.game_won = false;
		}
		setLED(robot.fuel_status);
		os_tsk_pass();
		os_itv_wait();
	}
}
/* Tasks */

/* Functions */
void pollJoystick(void)
{
	// Value read from joystick register
	volatile uint32_t joystick_val;
	uint32_t joystick_button;
	
	// Bits that indicate direction of joystick
	uint32_t direction_mask = 15 << 23;
	// Bit indicates if joystick button pressed
	uint32_t button_mask = 1 << 20;

	// Read from Joystick Register
	joystick_val = LPC_GPIO1 -> FIOPIN;

	// robot.dir will take values of either UP, DOWN, LEFT, RIGHT, or a random value
	robot.dir = (~joystick_val & direction_mask);
	// Joystick button bit is inverted
	robot.select_action = (~joystick_val) & button_mask ? true:false;
}

void configPushbutton(void)
{
	LPC_PINCON -> PINSEL4 &= ~(3 <<20);
	LPC_GPIO2 -> FIODIR &= ~(1 << 10);
	LPC_GPIOINT -> IO2IntEnF |= (1 << 10);
	NVIC_EnableIRQ(EINT3_IRQn);
	/*
	uint32_t val = LPC_GPIO2 -> FIOPIN;
	uint32_t mask = 1 << 10;

	robot.is_flying = (~val & mask) ? true : false;*/
}

void EINT3_IRQHandler(void)
{
	robot.is_flying = !robot.is_flying;
	printf("Pushbutton ISR Running\n");
	LPC_GPIOINT -> IO2IntClr |= (1 <<10);
}

void setLights(uint32_t num)
{
	uint32_t lower_mask = (1 << 3) - 1;
	uint32_t upper_mask = ((1<<8)- 1) & (~lower_mask);
	
	uint32_t gpio_1_set = num & lower_mask;
	uint32_t gpio_2_set = num & upper_mask;
	
	uint32_t fioset1 = 0x0 | ((gpio_1_set & 0x3)<<28) | ((gpio_1_set & 0x4)<<29);
	uint32_t fioset2 = 0x0 | (gpio_2_set>>1);
	LPC_GPIO1 -> FIOSET = fioset1;
	LPC_GPIO2 -> FIOSET = fioset2;
}

void clearLights(uint32_t num)
{
	uint32_t lower_mask = (1 << 3) - 1;
	uint32_t upper_mask = ((1<<8)- 1) & (~lower_mask);
	
	uint32_t gpio_1_set = num & lower_mask;
	uint32_t gpio_2_set = num & upper_mask;
	
	uint32_t fioset1 = 0x0 | ((gpio_1_set & 0x3)<<28) | ((gpio_1_set & 0x4)<<29);
	uint32_t fioset2 = 0x0 | (gpio_2_set>>1);
	LPC_GPIO1 -> FIOCLR = fioset1;
	LPC_GPIO2 -> FIOCLR = fioset2;
}
void setLED(uint32_t val)
{
	uint32_t num;
	
	//printf("ENTERED SET_LED\n");
	if (val > NUM_LEDS)
	{
		val = NUM_LEDS;
	}

	num = (1 << val) - 1;

	clearLights(255);
	setLights(num);
}


void loadBMP(uint32_t row, uint32_t col)
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
			fuel_x = row;
			fuel_y = col;
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
			//output_bmp = (unsigned char *)digger_bmp;
 			break;
 		default:
			output_bmp = (unsigned char *)path_bmp;
			break;
	}

	GLCD_Bitmap(row*20 - num_slides*3*20,col*20,20,20, output_bmp);
}

void initMap(void)
{
    uint32_t row, col;
    for (row = 0; row < MAX_SCREEN_LENGTH; row++)
    {
        for (col = 0; col < MAX_SCREEN_WIDTH; col++)
        {
            uint32_t rand_val = rand()%100;
						//printf("Row: %d, Col: %d\n", row, col);
            if (row <= SURFACE)
            {
                map[row][col] = 'S';
            }
            else if (rand_val < 20)
            {
                map[row][col] = 'R';
            }
            else if (rand_val >= 20 && rand_val < 40)
            {
                map[row][col] = 'C';
            }
            else if (rand_val >= 40 && rand_val < 80)
            {
                map[row][col] = 'D';
            }
            else if (rand_val >= 80 && rand_val < 94)
            {
                map[row][col] = 'E';
            }
            else
            {
                map[row][col] = 'G';
            }
						//printf("Row: %d, Col: %d, Val: %c\n", row, col, map[row][col]);
        }
    }
}

void endGameDisplay(void)
{
    unsigned char *message;
    GLCD_Clear(White);
	GLCD_SetBackColor(Blue);
	GLCD_SetTextColor(White);

    message = robot.game_won ? "CONGRATULATIONS! YOU WON!!" : "OH NO YOU RAN OUT OF FUEL. BETTER LUCK NEXT TIME!";
    GLCD_DisplayString (5, 5, 1, message)    
}

/*void SysTick_Handler(void)
{
	ms_ticks++;
}*/
/* Functions */


int main(void)
{
	LPC_GPIO1 -> FIODIR = 0xB0000000;
	LPC_GPIO2 -> FIODIR = 0x0000007C;
	// Initialize semaphores to 0
 	//init(&action_performed, 1);
 	//init(&display_refreshed, 1);
	init(&mutt, 0);
	
	// Initialize peripherals
	LED_setup();
	timer_setup();
	GLCD_Init();
	GLCD_Clear(White);
	GLCD_SetBackColor(Blue);
	GLCD_SetTextColor(White);
	configPushbutton();
	robot.x_pos = 2;
	robot.y_pos = 5;
	printf("Starting Program\n");
	printf("Mutex value: %d\n", mutt);
	srand(timer_read());
	initMap();
	setLED(8);
	setLED(3);
	map[robot.x_pos][robot.y_pos] = 'X';
	robot.fuel_status = 8;
	map[SURFACE][0] = 'F';

	// Initialize main task
	os_sys_init(mainTask);
}
