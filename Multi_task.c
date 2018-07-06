#include <lpc17xx.h>
#include <stdio.h>
#include <stdbool.h>
#include <RTL.h>

#include "GLCD.h"

#include "game.h"


__task void updateDisplay(void)
{
	os_itv_set(10);

	while(1)
	{
		// Read IMU data

		// Signal that IMU data has been read successfully
		signal(&cond_1);
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
	init(&cond_1, 0);
	init(&cond_2, 0);

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
