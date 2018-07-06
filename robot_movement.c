#define UP (1 << 26)
#define DOWN (1 << 24)
#define LEFT (1 << 25)
#define RIGHT (1 << 23)

/* Character Struct */
typedef struct {
	volatile uint32_t x_pos, y_pos;
	
	uint32_t num_gold, num_silver, num_copper;
	uint32_t fuel_status;
	uint32_t num_points;
	
	bool is_flying;
	bool select_action;
	// Robot Inputs
	volatile uint32_t dir;
	//unsigned char *dir;
	//volatile uint32_t select_action;
} robot_t;
robot_t robot;
/* Character Struct */



/*__task */void pollJoystick(void)
{
	//os_itv_set(10);
	
	uint32_t joy_val;
	uint32_t direction_mask = 15 << 23;
	uint32_t joybutton_mask = 1 << 20;
	
	while (1)
	{
		joy_val = LPC_GPIO1 -> FIOPIN;
		switch(~joy_val & direction_mask)
		{
			case RIGHT:
				robot.dir = RIGHT;
				//robot.x_pos+=40;
				break;
			case DOWN:
				robot.dir = DOWN;
				//robot.y_pos+=40;
				break;
			case LEFT:
				robot.dir = LEFT;
				//robot.x_pos-=40;
				break;
			case UP:
				robot.dir = UP;
				//robot.y_pos-=40;
				break;
			default:
				robot.dir = 0;
		}
		robot.select_action = joy_val & joybutton_mask ? false: true;
		//os_tsk_pass();
	}
}

void pollPushbutton(void)
{
	uint32_t val = LPC_GPIO2 -> FIOPIN;
	uint32_t mask = 1 << 10;
	
	robot.is_flying = (~val & mask) ? true : false;
	
	//return (~val & mask) >> 10;
}

__task void moveRobot(void)
{
	os_itv_set(10);
	
	while(1)
	{
		pollJoystick();
		pollPushbutton();
		
		switch(robot.dir)
		{
			case RIGHT:
				robot.x_pos += 40;
				break;
			case LEFT:
				robot.x_pos -= 40;
				break;
			case UP:
				if(robot.is_flying)
				{
					robot.y_pos -= 40;
				}
				break;
			case DOWN:
				if (!robot.is_flying)
				{
					robot.y_pos += 40;
				}
				break;
		}
		
		// Wait until sensorFusion algorithm has been run
		//wait(&cond_2);
		
		// Output data to game
		//printf("%f,%f,%f\n", sensor_fusion_getRoll(), -sensor_fusion_getYaw(), sensor_fusion_getPitch());
		os_tsk_pass();
	}
}