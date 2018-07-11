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
	// Declare current row and column
	//uint32_t count = 0;
	//os_itv_set(10);
	//printf("UPDATE DISPLAY INIT MUTEX VAL: %d, %d, %d\n", screen_busy[0], screen_busy[1], screen_busy[2]);
	//os_mut_init(&screen_busy);
	//printf("UPDATE DISPLAY NEXT MUTEX VAL: %d, %d, %d\n", screen_busy[0], screen_busy[1], screen_busy[2]);
	showInstructionMenu();
	joystick_inputs = pollJoystick();
	while(joystick_inputs[1] == 0)
	{
		printf("Instruction menu. Press joystick to continue\n");
		joystick_inputs = pollJoystick();
	}
	os_dly_wait(200);
	printMap();

	while(!game_over)
	{
		os_sem_wait(&action_performed, 0xffff);
		//os_mut_wait(&screen_busy, 0xffff);
		//printf("MUTEX AFTER WAIT: %d, %d, %d\n", screen_busy[0], screen_busy[1], screen_busy[2]);
		printf("Entered Update Display\n");
		// Wait until something has changed on the screen
		//signal(&display_refreshed);
		//printf("Entered updateDisplay\n");
		//while (mutt == 0);
		//printf("Mutex value in updateDisplay: %d\n", mutt);
		
		//printf("min_row: %d, max_row: %d\n", min_row, max_row);
		
		if (menu_requested && !(robot.x_pos == fuel_x && robot.y_pos == fuel_y))
		{
			showGeneralMenu();
			os_dly_wait(1000);
			printMap();
			menu_requested = false;
			os_sem_send(&display_refreshed);
		}
		else if (!menu_requested)
		{
			printf("menu requested: %d\n", menu_requested);
			if(map_scrolled)
			{	
				printf("Min Row: %d, Max Row: %d, Num Slides: %d", min_row, max_row, num_slides);
				printMap();
				map_scrolled = false;
			}
			else
			{
				loadBMP(robot.x_pos_prev, robot.y_pos_prev);
				loadBMP(robot.x_pos, robot.y_pos);
			}
			// Signal that LCD Display has been redrawn	
			//signal(&display_refreshed);
			
			//os_mut_release(&screen_busy);
			//printf("MUTEX AFTER RELEASE: %d, %d, %d\n", screen_busy[0], screen_busy[1], screen_busy[2]);
			
			//os_itv_wait();
		}
		
		os_sem_send(&display_refreshed);
		os_tsk_pass();
	}
    
	if (game_over)
	{
			endGameDisplay();
	}
	while(1);
}

__task void moveRobot(void)
{

	// TODO: Implement points system, implement map update, implement rock boundary conditions, implement is_flying state condition

	uint32_t cur_state = 0;
	uint32_t prev_state = 0;
	unsigned char* direction;
	uint32_t *joystick_inputs;
	//os_itv_set(10);
	//printf("MUTEX MOVE ROBOT: %d, %d, %d\n", screen_busy[0], screen_busy[1], screen_busy[2]);

	while(!game_over)
	{
		int32_t x_pos_next=0, y_pos_next=0;
		//os_mut_wait(&screen_busy, 0xffff);
		//printf("MUTEX AFTER WAIT: %d, %d, %d\n", screen_busy[0], screen_busy[1], screen_busy[2]);
		//printf("Entered Move Robot, MUTEX VAL: %d\n", screen_busy);
		//signal(&action_performed);
		//wait(&display_refreshed);
		os_sem_wait(&display_refreshed, 0xffff);
		printf("Entered moveRobot\n");
		//while (mutt == 1);
		//printf("Mutex value in moveRobot: %d\n", mutt);
		joystick_inputs = pollJoystick();
		robot.select_action = joystick_inputs[1];

		if (robot.select_action)
		{
			menu_requested = true;
		}
		cur_state = joystick_inputs[0];

		if (cur_state == RIGHT || cur_state == LEFT)
		{
			robot.dir = cur_state;
		}

		if (cur_state != prev_state)
		{
			robot.x_pos_prev = robot.x_pos;
			robot.y_pos_prev = robot.y_pos;
			
			switch(cur_state)
			{
				case DOWN:
					if (!robot.is_flying)
					{
						if (robot.x_pos+1 < MAX_MAP_WIDTH)
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
						if (robot.x_pos - 1 >= SURFACE)
						{
							map[robot.x_pos][robot.y_pos] = 'P';
							x_pos_next = -1;
							direction = "UP";
						}
					}
					break;
				case LEFT:
					if (robot.y_pos != 0 && robot.y_pos - 1 >= 0)
					{
						map[robot.x_pos][robot.y_pos] = robot.x_pos == SURFACE ? 'S' : 'P';
						y_pos_next = -1;
						direction = "LEFT";
					}
					break;
				case RIGHT:
					if (robot.y_pos + 1 < max_col)
					{
						map[robot.x_pos][robot.y_pos] = robot.x_pos == SURFACE ? 'S' : 'P';
						y_pos_next = 1;
						direction = "RIGHT";
					}
					break;
			}
			
			if (robot.x_pos == fuel_x && robot.y_pos == fuel_y)
			{
				map[robot.x_pos][robot.y_pos] = 'F';
			}

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

				if (robot.num_points > END_GAME_THRESHOLD)
				{
						game_over = true;
						robot.game_won = true;
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
		
		//signal(&action_performed);
		os_sem_send(&action_performed);
		
		//printf("RobotX: %d, RobotY: %d\n", robot.x_pos, robot.y_pos);
		//printf("MAP: %c, JoyButt: %d\n", map[robot.x_pos][robot.y_pos], robot.select_action);
		//printf("MUTEX MOVE ROBOT AFTER WAIT: %d, %d, %d\n", screen_busy[0], screen_busy[1], screen_busy[2]);
		//os_tsk_pass();
		//os_itv_wait();
	}
	os_tsk_delete_self();
}

__task void buyFuel(void)
{
	uint32_t num_bars;
	uint32_t cost;
	
	//os_itv_set(200);

	while(!game_over)
	{
		os_sem_wait(&needs_refill, 0xffff);
		printf("Entered buyFuel\n");

		num_bars = NUM_LEDS - robot.fuel_status;
		cost = 2*num_bars;
		
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
		os_dly_wait(1000);
		printMap();
		menu_requested = false;
		
		os_sem_send(&fuel_refilled);
	}
	os_tsk_delete_self();
}

__task void updateFuelStatus(void)
{
	uint32_t fuel_time_cur = 0;
    //uint32_t fuel_time_prev = 0;
	uint32_t fuel_time_next = 0;
    //uint32_t blocked_time = 0;
	uint32_t fuel_consumption_rate = FUEL_TIME;
	
    //os_mut_init(&fuel_lock);
	//os_itv_set(200);

	while(!game_over)
	{
		printf("Entered updateFuelStatus\n");
		os_sem_wait(&fuel_refilled, 0xffff);
        //fuel_time_prev = timer_read()/1E6;

        //os_mut_wait(&fuel_lock, 0xffff);

        
        //blocked_time += (fuel_time_cur - fuel_time_prev);
        //fuel_time_cur -= blocked_time;
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
			//printf("Entered 10s condition, robot_fuel_status: %d\n", robot.fuel_status);
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
			os_sem_send(&needs_refill);
		}
		else
		{
			os_sem_send(&fuel_refilled);
		}
		os_tsk_pass();
		//os_itv_wait();
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

	return joystick_inputs;
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

	GLCD_Bitmap(row*20 - num_slides*NUM_SCROLL*20,col*20,20,20, output_bmp);
}

void initMap(void)
{
    uint32_t row, col;
    for (row = 0; row < MAX_MAP_WIDTH; row++)
    {
        for (col = 0; col < MAX_SCREEN_LENGTH; col++)
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
    unsigned char message[50];
    GLCD_Clear(White);
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Blue);

		if (robot.game_won)
		{
			GLCD_DisplayString (2, 2, 1, "CONGRATULATIONS!!");
			GLCD_DisplayString (3, 5, 1, "YOU WON!!");
		}
		else
		{
			GLCD_DisplayString (2, 2, 1, "OH NO YOU RAN OUT");
			GLCD_DisplayString (3, 5, 1, "OF FUEL!!");
		}

		sprintf(message, "Total Points: %d", robot.num_points);
		printf("Message: %s\n", message);
		GLCD_DisplayString (5, 2, 1, message);

		sprintf(message, "GOLD: %d", robot.num_gold);
		GLCD_DisplayString (6, 2, 1, message);
		printf("Message: %s\n", message);

		sprintf(message, "EMERALD: %d", robot.num_emerald);
		GLCD_DisplayString (7, 2, 1, message);
		printf("Message: %s\n", message);

		sprintf(message, "COPPER: %d", robot.num_copper);
		GLCD_DisplayString (8, 2, 1, message);
		printf("Message: %s\n", message);
}
void printMap(void)
{
	uint32_t row, col;
	unsigned char *output_bmp;
	for (row=min_row; row<max_row; row++)
	{
		for (col=min_col; col<max_col; col++)
		{
			// Print char value of array element at {row, col} on LCD Display
			//printf("Row: %d, Col: %d\n", row, col);
			loadBMP(row, col);
		}
	}
}

void printBlackOverlay(void)
{
	uint32_t row, col;
	unsigned char *output_bmp;
	
	for (row = MENU_X_PADDING; row < MAX_SCREEN_WIDTH - MENU_X_PADDING; row++)
	{
		for (col = MENU_Y_PADDING; col < MAX_SCREEN_LENGTH - MENU_Y_PADDING; col++)
		{
			output_bmp = (unsigned char *)path_bmp;
			GLCD_Bitmap(row*20,col*20,20,20, output_bmp);
		}
	}
}

void showGeneralMenu(void)
{	
	unsigned char message[50];
	
	printBlackOverlay();
	
	GLCD_SetTextColor(White);
	GLCD_SetBackColor(Black);

	GLCD_DisplayString (1, 4, 1, "Current Stats");
	printf("Current Stats\n");
	
	sprintf(message, "Total Points: %d", robot.num_points);
	printf("Message: %s\n", message);
	GLCD_DisplayString (3, 2, 1, message);
	
	sprintf(message, "GOLD: %d", robot.num_gold);
	GLCD_DisplayString (4, 2, 1, message);
	printf("Message: %s\n", message);
	
	sprintf(message, "EMERALD: %d", robot.num_emerald);
	GLCD_DisplayString (5, 2, 1, message);
	printf("Message: %s\n", message);
	
	sprintf(message, "COPPER: %d", robot.num_copper);
	GLCD_DisplayString (6, 2, 1, message);
	printf("Message: %s\n", message);
	
	sprintf(message, "Fuel Status: %d", robot.fuel_status);
	GLCD_DisplayString (8, 2, 1, message);
	printf("Message: %s\n", message);
}

void showBuyFuelMenu(uint32_t money_available)
{
	unsigned char message[50];
	
	printBlackOverlay();
	GLCD_SetTextColor(White);
	GLCD_SetBackColor(Black);
	
	if (money_available == 1)
	{
		GLCD_DisplayString (2, 4, 1, "Successfully");
		GLCD_DisplayString (3, 4, 1, "Refuelled!");
		printf("Successfully Refuelled!\n");
	}
	else if (money_available == 2)
	{
		GLCD_DisplayString (2, 2, 1, "You are already");
		GLCD_DisplayString (3, 4, 1, "at max fuel!");
		printf("You are at max fuel!\n");
	}
	else
	{
		GLCD_DisplayString (2, 2, 1, "Need more points");
		GLCD_DisplayString (3, 5, 1, "to refuel!");
		printf("Not enough money to refuel!\n");
	}
	
	sprintf(message, "Total Points: %d", robot.num_points);
	printf("Message: %s\n", message);
	GLCD_DisplayString (5, 2, 1, message);
	
	sprintf(message, "Fuel Status: %d", robot.fuel_status);
	GLCD_DisplayString (7, 2, 1, message);
	printf("Message: %s\n", message);
}

void showInstructionMenu(void)
{
	GLCD_Clear(White);
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Blue);
	GLCD_DisplayString (1, 4, 1, "DEEP DIGGER!");
	
	GLCD_DisplayString (10, 2, 0, "Objective:");
	GLCD_DisplayString (12, 2, 0, "Collect as many minerals as possible, before you run");
	GLCD_DisplayString (14, 2, 0, "out of fuel. Gold=10, Emerald=5, Copper=1 points.");
	
	GLCD_DisplayString (17, 2, 0, "Game Instructions:");
	GLCD_DisplayString (19, 2, 0, "-Use joystick to control movement");
	GLCD_DisplayString (21, 2, 0, "-Push joystick to open menu");
	GLCD_DisplayString (23, 2, 0, "-To enable flying, press pushbutton");
	GLCD_DisplayString (25, 2, 0, "-To refuel, get to RED block and push joystick");
	
	GLCD_DisplayString (28, 2, 0, "Press joystick to continue");
}
/* Functions */


int main(void)
{
	LPC_GPIO1 -> FIODIR = 0xB0000000;
	LPC_GPIO2 -> FIODIR = 0x0000007C;
	
	//os_mut_init(&screen_busy);
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
	//printf("Mutex value: %d\n", mutt);
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

// TODO: Semaphores, Points and flying display, select menu(perhaps), improve end game display, gravity?