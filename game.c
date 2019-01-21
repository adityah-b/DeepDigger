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
	printMap();

	while(!game_over)
	{
		// Wait until something has changed on the screen
		os_sem_wait(&action_performed, 0xffff);

		if(map_scrolled)
		{	
			printMap();
			map_scrolled = false;
		}
		else
		{
			loadBMP(robot.x_pos_prev, robot.y_pos_prev);
			loadBMP(robot.x_pos, robot.y_pos);
		}
		// Signal that LCD Display has been redrawn	
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
	uint32_t cur_state = 0;
	uint32_t prev_state = 0;
	unsigned char* direction;
	uint32_t *joystick_inputs;

	while(!game_over)
	{
		int32_t x_pos_next=0, y_pos_next=0;

		os_sem_wait(&display_refreshed, 0xffff);

		joystick_inputs = pollJoystick();
		robot.select_action = joystick_inputs[1];

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
		prev_state = cur_state;

		// Check if map boundaries need to be updated
		if (robot.x_pos + 1 >= max_row && max_row + NUM_SCROLL < MAX_SCREEN_LENGTH)
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
		
		// Signal that an action has been performed
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
		os_sem_wait(&needs_refill, 0xffff);

		num_bars = NUM_LEDS - robot.fuel_status;
		cost = 2*num_bars;
		if (robot.num_points >= cost)
		{
			robot.fuel_status = NUM_LEDS;
			robot.num_points -= cost;
		}
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
		if (fuel_time_cur - fuel_time_next >= fuel_consumption_rate)
		{
			robot.fuel_status--;
			fuel_time_next += fuel_consumption_rate;
		}
		// Game ends when fuel is <= 0
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
		os_sem_send(&fuel_refilled);
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

	return joystick_inputs;
}

void configPushbutton(void)
{
	LPC_PINCON -> PINSEL4 &= ~(3 <<20);
	LPC_GPIO2 -> FIODIR &= ~(1 << 10);
	LPC_GPIOINT -> IO2IntEnF |= (1 << 10);
	NVIC_EnableIRQ(EINT3_IRQn);
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
    for (row = 0; row < MAX_SCREEN_LENGTH; row++)
    {
        for (col = 0; col < MAX_SCREEN_WIDTH; col++)
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

void endGameDisplay(void)
{
    unsigned char *message;
    GLCD_Clear(White);
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Blue);

    message = robot.game_won ? "YOU WON!!" : "YOU LOST!!";
    GLCD_DisplayString (5, 5, 1, message);   
}
void printMap(void)
{
	uint32_t row, col;
	for (row=min_row; row<max_row; row++)
		{
			for (col=min_col; col<max_col; col++)
			{
				// Print char value of array element at {row, col} on LCD Display
				loadBMP(row, col);
			}
		}
}
/* Functions */


int main(void)
{
	LPC_GPIO1 -> FIODIR = 0xB0000000;
	LPC_GPIO2 -> FIODIR = 0x0000007C;
	
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
