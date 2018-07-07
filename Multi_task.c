#include <lpc17xx.h>
#include <stdio.h>
#include <stdbool.h>
#include <RTL.h>

#include "GLCD.h"

#include "game.h"


// Global Variables
uint32_t min_row, min_col;
uint32_t max_row, max_col;
char map[MAX_SCREEN_WIDTH][MAX_SCREEN_LENGTH]; // Temporary

// Tasks
__task void updateDisplay(void);
__task void pollPushButton(void);
__task void pollJoystick(void);
__task void updateFuelStatus(void);
__task void moveRobot(void);
__task void buyFuel(void);

/* Character Struct */
typedef struct {
	volatile uint32_t x_pos, y_pos;
	
	uint32_t num_gold, num_silver, num_copper;
	uint32_t fuel_status;
	uint32_t num_points;
	
	bool is_flying;
	
	// Robot Inputs
	volatile uint32_t dir;
	volatile uint32_t select_action;
} robot_t;
robot_t robot;
/* Character Struct */


/* Semaphore Implementation */
typedef uint32_t sem_t;
sem_t action_performed, display_refreshed;

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
/* Semaphore Implementation */


__task void mainTask(void)
{
	// Create tasks
	os_tsk_create(updateDisplay, 1);
	
	os_tsk_create(pollPushButton, 2);
	os_tsk_create(pollJoystick, 2);
	
	os_tsk_create(updateFuelStatus, 3);
	os_tsk_create(moveRobot, 3);
	
	os_tsk_create(buyFuel, 4);

	
	os_tsk_delete_self();
}

void loadBMP(int row, int col)
{
	unsigned char *output_bmp;
	switch(map[row][col])
	{
		// E R D G S C P X F
		case 'E':
			output_bmp = (unsigned char *)E_bmp;
			break;
		case 'R':
			output_bmp = (unsigned char *)R_bmp;
			break;
		case 'D':
			output_bmp = (unsigned char *)D_bmp;
			break;
		case 'G':
			output_bmp = (unsigned char *)G_bmp;
			break;
		case 'S':
			output_bmp = (unsigned char *)S_bmp;
			break;
		case 'C':
			output_bmp = (unsigned char *)C_bmp;
			break;
		case 'X_P':
			output_bmp = (unsigned char *)X_P_bmp;
			break;
		case 'X_E':
			output_bmp = (unsigned char *)X_E_bmp;
			break;
		case 'F':
			output_bmp = (unsigned char *)F_bmp;
			break;
		default:
			output_bmp = (unsigned char *)P_bmp;
			break;
	}

	GLCD_Bitmap(row,col,40,40, output_bmp);
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

__task void pollPushButton(void)
{
	os_itv_set(10);

	// Sensor Fusion Algorithm Initialization

	while(1)
	{
		// Wait until IMU data has been read
		wait(&cond_1);

		// Update data

		// Signal that sensorFusion algorithm has been run successfully
		signal(&cond_2);
		os_tsk_pass();
	}
}

__task void pollJoystick(void)
{
	os_itv_set(10);

	while(1)
	{
		// Wait until sensorFusion algorithm has been run
		wait(&cond_2);

		// Output data to game
		//printf("%f,%f,%f\n", sensor_fusion_getRoll(), -sensor_fusion_getYaw(), sensor_fusion_getPitch());
		os_tsk_pass();
	}
}

__task void updateFuelStatus(void)
{
	os_itv_set(10);

	while(1)
	{
		// Wait until sensorFusion algorithm has been run
		wait(&cond_2);

		// Output data to game
		//printf("%f,%f,%f\n", sensor_fusion_getRoll(), -sensor_fusion_getYaw(), sensor_fusion_getPitch());
		os_tsk_pass();
	}
}
__task void moveRobot(void)
{
	os_itv_set(10);

	while(1)
	{

		// Wait until sensorFusion algorithm has been run
		wait(&cond_2);

		// Output data to game
		//printf("%f,%f,%f\n", sensor_fusion_getRoll(), -sensor_fusion_getYaw(), sensor_fusion_getPitch());
		os_tsk_pass();
	}
}

int main(void)
{
	unsigned char *direction, *button_pressed;
	uint32_t val;
	uint32_t mask = 15 << 23;

	unsigned char *digger_bmp2;
	int i = 160, j = 120;
	int count = 0;
	// Initialize semaphores to 0
	init(&action_performed, 1);
	init(&display_refreshed, 0);

	// Initialize peripherals
	LED_setup();
	GLCD_Init();
	digger_bmp2 = (unsigned char *)digger_bmp;
	while (1)
	{
		val = LPC_GPIO1 -> FIOPIN;
		switch(~val & mask)
		{
			case 1 << 23 :
				//direction = "RIGHT ";
				i+=40;
				break;
			case 1 << 24 :
				//direction = "DOWN  ";
				j+=40;
				break;
			case 1 << 25 :
				//direction = "LEFT  ";
				i-=40;
				break;
			case 1 << 26 :
				//direction = "UP    ";
				j-=40;
				break;
			default:
				direction = "No Dir";
		}

		if (i+40 >= 320) i = 0;
		GLCD_SetTextColor(Blue);
		GLCD_SetBackColor(Blue);
		GLCD_Clear(Blue);
		while(count <= 1000)
		{
			count++;
		}
		count = 0;
		GLCD_Bitmap(i,j,40,40, digger_bmp2);
//		i+=40;
		while(count <= 10000000){
			count++;
		}
		count = 0;
	}
	printf("Starting Program");

	// Initialize main task
	//os_sys_init(mainTask);
}

/* run update map, map array should be 100x100
initially,
min_row = 0, max_row = 6
min_col = 0, max_col = 8

in char array, position of robot marked with X
when printing, if array[x_pos][y_pos] == X, run task to print robot bitmap
then continue from there
*/
