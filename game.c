#include "game.h"
#include "menu_functions.c"

/* Tasks */
__task void mainTask(void)
{
	// Initialize Semaphores
	os_sem_init(&display_refreshed, 0);
	os_sem_init(&action_performed, 1);
	os_sem_init(&fuel_refilled, 1);
	os_sem_init(&needs_refill, 0);
	
	
	// Create tasks
	os_tsk_create(updateDisplay, 1);
	os_tsk_create(moveRobot, 1);
	os_tsk_create(updateFuelStatus, 1);
	os_tsk_create(buyFuel, 1);

	os_tsk_delete_self();
}

__task void updateDisplay(void)
{
	volatile uint32_t *joystick_inputs;
	
	// Display instructions and wait until user has requested to continue
	showInstructionMenu();
	joystick_inputs = pollJoystick();
	while(joystick_inputs[1] == 0)
	{
		os_dly_wait(20);
		joystick_inputs = pollJoystick();
	}
	//os_dly_wait(50);
	
	// Print initial game map on LCD
	printMap();

	while(!game_over)
	{
		// Wait until something has changed on the screen
		os_sem_wait(&action_performed, 0xffff);
		
		if (menu_requested && !(robot.x_pos == fuel_x && robot.y_pos == fuel_y))
		{
			// If user requested stats menu and digger is not at fuel refill station
			// show stats menu and close automatically. Then reprint game map
			showGeneralMenu();
			os_dly_wait(1000);
			printMap();
			menu_requested = false;
		}
		else if (!menu_requested)
		{
			if(map_scrolled)
			{
				// If user scrolled past map edges, reprint map
				printMap();
				map_scrolled = false;
			}
			else
			{
				// Otherwise, reprint current and previous cell on map
				loadBMP(robot.x_pos_prev, robot.y_pos_prev);
				loadBMP(robot.x_pos, robot.y_pos);
			}
		}
		
		// Signal that inputs can now be accepted
		os_sem_send(&display_refreshed);
		os_tsk_pass();
	}
    
	if (game_over)
	{
		// Output end game screen
		endGameDisplay();
	}
	
	// Keep running this task indefinitely if game over
	while(1);
}

__task void moveRobot(void)
{
	uint32_t cur_state = 0;
	uint32_t prev_state = 0;
	uint32_t *joystick_inputs;
	
	while(!game_over)
	{
		int32_t x_pos_next=0, y_pos_next=0;

		// Wait until display has been refreshed before accepting new inputs
		os_sem_wait(&display_refreshed, 0xffff);

		joystick_inputs = pollJoystick();
		robot.select_action = joystick_inputs[1];

		if (robot.select_action)
		{
			// If joystick has been pushed, user has requested a menu
			menu_requested = true;
		}
		
		// Store joystick direction {up, down, left, right} in cur_state
		cur_state = joystick_inputs[0];

		if (cur_state == RIGHT || cur_state == LEFT)
		{
			// Latch robot.dir to either right or left direction
			robot.dir = cur_state;
		}

		// If user has changed direction of joystick
		if (cur_state != prev_state)
		{
			// Update previous robot coordinates
			robot.x_pos_prev = robot.x_pos;
			robot.y_pos_prev = robot.y_pos;
			
			switch(cur_state)
			{
				// Move robot by 1 cell if future coordinates satisfy conditions
				case DOWN:
					if (!robot.is_flying)
					{
						if (robot.x_pos+1 < MAX_MAP_WIDTH)
						{
							// If robot is not flying and future coordinates are within bounds,
							// update current cell to be either sky or path
							map[robot.x_pos][robot.y_pos] = robot.x_pos == SURFACE ? 'S' : 'P';
							x_pos_next = 1;
						}
					}
					break;
				case UP:
					if(robot.is_flying)
					{
						if (robot.x_pos - 1 >= SURFACE)
						{
							// If robot is in flying mode, wants to go up, and is beneath
							// the surface, update current cell to path and then move up
							map[robot.x_pos][robot.y_pos] = 'P';
							x_pos_next = -1;
						}
					}
					break;
				case LEFT:
					if (robot.y_pos != 0 && robot.y_pos - 1 >= 0)
					{
						map[robot.x_pos][robot.y_pos] = robot.x_pos == SURFACE ? 'S' : 'P';
						y_pos_next = -1;
					}
					break;
				case RIGHT:
					if (robot.y_pos + 1 < max_col)
					{
						map[robot.x_pos][robot.y_pos] = robot.x_pos == SURFACE ? 'S' : 'P';
						y_pos_next = 1;
					}
					break;
			}
			
			// If the robot position was the fuel station, overwrite 'S' with 'F'
			if (robot.x_pos == fuel_x && robot.y_pos == fuel_y)
			{
				map[robot.x_pos][robot.y_pos] = 'F';
			}

			robot.x_pos += x_pos_next;
			robot.y_pos += y_pos_next;
			
			if (robot.is_flying && map[robot.x_pos][robot.y_pos] != 'P')
			{
				// Check to ensure that robot can only travel through paths when flying
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
			// If robot position had a mineral that was not a rock, increase point tally
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

				if (robot.num_points > END_GAME_THRESHOLD)
				{
						// Win game if accumulated points exceed threshold
						game_over = true;
						robot.game_won = true;
				}
				
				map[robot.x_pos][robot.y_pos] = 'X';
			}
			else
			{
				// Otherwise, prevent robot from moving to this cell
				robot.x_pos -= x_pos_next;
				robot.y_pos -= y_pos_next;
			}
		}
		prev_state = cur_state;

		// Check if map boundaries need to be updated
		if (robot.x_pos + 1 >= max_row && max_row + NUM_SCROLL <= MAX_MAP_WIDTH)
		{
			min_row += NUM_SCROLL;
			max_row += NUM_SCROLL;
			num_slides++;
			map_scrolled = true;
		}
		if (robot.x_pos - 1 <= min_row && min_row >= NUM_SCROLL)
		{
			min_row -= NUM_SCROLL;
			max_row -= NUM_SCROLL;
			num_slides--;
			map_scrolled = true;
		}
		
		// Signal that input was received
		os_sem_send(&action_performed);	
	}
	os_tsk_delete_self();
}

__task void buyFuel(void)
{
	uint32_t num_bars;
	uint32_t cost;
	
	while(!game_over)
	{
		// Wait until receives signal from updateFuelStatus task
		os_sem_wait(&needs_refill, 0xffff);

		// Calculate cost of refuelling to full tank
		num_bars = NUM_LEDS - robot.fuel_status;
		cost = 2*num_bars;
		
		// Show appropriate Fuel Store menu
		if (cost == 0)
		{
			showBuyFuelMenu(2);
		}
		else if (robot.num_points >= cost && menu_requested)
		{
			robot.fuel_status = NUM_LEDS;
			robot.num_points -= cost;
			
			showBuyFuelMenu(1);
			setLED(robot.fuel_status);
		}
		else if (menu_requested)
		{
			showBuyFuelMenu(0);
		}
		
		// Close overlayed menu automatically and redraw map
		os_dly_wait(1000);
		printMap();
		menu_requested = false;
		
		// Signal that fuel was successfully purchased
		os_sem_send(&fuel_refilled);
	}
	os_tsk_delete_self();
}

__task void updateFuelStatus(void)
{
	uint32_t fuel_time_cur = 0;
	uint32_t fuel_time_next = 0;
	uint32_t fuel_consumption_rate = FUEL_TIME;
	
	while(!game_over)
	{
		os_sem_wait(&fuel_refilled, 0xffff);

		// Lose 1 bar of fuel every 10s (5s if robot is flying)
		fuel_consumption_rate = robot.is_flying ? 0.5*FUEL_TIME : FUEL_TIME;
		
		// Decrement fuel tank by 1 per period
		fuel_time_cur = timer_read()/1E6;
		if (fuel_time_cur - fuel_time_next >= fuel_consumption_rate)
		{
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
		if (robot.x_pos == fuel_x && robot.y_pos == fuel_y && robot.select_action)
		{
			// If robot is on fuel tank cell, signal that fuel can be purchased
			os_sem_send(&needs_refill);
		}
		else
		{
			// Otherwise, pass semaphore back to current task
			os_sem_send(&fuel_refilled);
		}
		os_tsk_pass();
	}
	os_tsk_delete_self();
}
/* Tasks */

/* Functions */
uint32_t* pollJoystick(void)
{
	// Value read from joystick register
	volatile uint32_t joystick_val;
	uint32_t joystick_inputs[2];
	
	// Bits that indicate direction of joystick
	uint32_t direction_mask = 15 << 23;
	// Bit indicates if joystick button pressed
	uint32_t button_mask = 1 << 20;

	// Read from Joystick Register
	joystick_val = LPC_GPIO1 -> FIOPIN;

	// robot.dir will take values of either UP, DOWN, LEFT, RIGHT, or a random value
	joystick_inputs[0] = (~joystick_val & direction_mask);
	// Joystick button bit is inverted
	joystick_inputs[1] = (~joystick_val) & button_mask ? true:false;

	// Function returns direction in [0] and button push status in [1]
	return joystick_inputs;
}

void configPushbutton(void)
{
	// Configure pushbutton interrupt
	LPC_PINCON -> PINSEL4 &= ~(3 <<20);
	LPC_GPIO2 -> FIODIR &= ~(1 << 10);
	LPC_GPIOINT -> IO2IntEnF |= (1 << 10);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(void)
{
	// ISR for pushbutton
	robot.is_flying = !robot.is_flying;
	printf("Pushbutton ISR running\n");
	LPC_GPIOINT -> IO2IntClr |= (1 <<10);
}

void setLights(uint32_t num)
{
	// Function outputs number to LEDs
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
	// Function clears LED bits
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
	
	// Set LEDs based on amount of fuel left
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
	
	// Depending on content of cell, output appropriate bitmap
	switch(map[row][col])
	{
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
			// Display appropriate robot bitmap depending on whether sky or path, left or right
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
	
	// Output to LCD at correct row and col, taking into account scrolling
	GLCD_Bitmap(row*20 - num_slides*NUM_SCROLL*20,col*20,20,20, output_bmp);
}

void initMap(void)
{
		/* Function to generate initial randomized map based on probability
		;  Rock 		= 20% of map
		;  Copper 	= 20% of map
		;  Dirt			= 40% of map
		;  Emerald 	= 14% of map
		;  Gold			= 6% of map
		*/
    uint32_t row, col;
    for (row = 0; row < MAX_MAP_WIDTH; row++)
    {
        for (col = 0; col < MAX_SCREEN_LENGTH; col++)
        {
            uint32_t rand_val = rand()%100;
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
        }
    }
}

void printMap(void)
{
	// Function to print entire map within game dimensions
	uint32_t row, col;
	
	for (row=min_row; row<max_row; row++)
	{
		for (col=min_col; col<max_col; col++)
		{
			// Print bitmap of array element at {row, col} on LCD Display
			loadBMP(row, col);
		}
	}
}

/* Functions */


int main(void)
{
	LPC_GPIO1 -> FIODIR = 0xB0000000;
	LPC_GPIO2 -> FIODIR = 0x0000007C;
	
	printf("Starting Program\n");
	
	// Initialize peripherals
	LED_setup();
	timer_setup();
	GLCD_Init();
	GLCD_Clear(White);
	GLCD_SetBackColor(Blue);
	GLCD_SetTextColor(White);
	configPushbutton();
	
	// Initialize robot to center on surface
	robot.x_pos = 2;
	robot.y_pos = 5;

	// Init random function with current timer value as seed
	srand(timer_read());
	
	// Generate map
	initMap();

	// Initialize robot states
	map[robot.x_pos][robot.y_pos] = 'X';
	robot.fuel_status = 8;
	map[SURFACE][0] = 'F';

	// Initialize main task
	os_sys_init(mainTask);
}

// TODO: Semaphores, Points and flying display, select menu(perhaps), improve end game display, gravity?