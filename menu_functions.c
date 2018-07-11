/* Functions to output menus on GLCD Display*/

void printBlackOverlay(void)
{
	// Function to print black overlay with padding; used for menu background
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
	// Function to output general stats to GLCD display
	unsigned char message[50];
	
	printBlackOverlay();
	
	GLCD_SetTextColor(White);
	GLCD_SetBackColor(Black);

	GLCD_DisplayString (1, 4, 1, "Current Stats");
	printf("Current Stats\n");
	
	sprintf(message, "Total Points: %d", robot.num_points);
	printf("%s\n", message);
	GLCD_DisplayString (3, 2, 1, message);
	
	sprintf(message, "GOLD: %d", robot.num_gold);
	GLCD_DisplayString (4, 2, 1, message);
	printf("%s\n", message);
	
	sprintf(message, "EMERALD: %d", robot.num_emerald);
	GLCD_DisplayString (5, 2, 1, message);
	printf("%s\n", message);
	
	sprintf(message, "COPPER: %d", robot.num_copper);
	GLCD_DisplayString (6, 2, 1, message);
	printf("%s\n", message);
	
	sprintf(message, "Fuel Status: %d", robot.fuel_status);
	GLCD_DisplayString (8, 2, 1, message);
	printf("%s\n", message);
}

void showBuyFuelMenu(uint32_t fuel_centre_option)
{
	// Function which outputs menu to GLCD when user is at fuel refill centre
	unsigned char message[50];
	
	printBlackOverlay();
	GLCD_SetTextColor(White);
	GLCD_SetBackColor(Black);
	
	if (fuel_centre_option == 1)
	{
		GLCD_DisplayString (2, 4, 1, "Successfully");
		GLCD_DisplayString (3, 4, 1, "Refuelled!");
		printf("Successfully Refuelled!\n");
	}
	else if (fuel_centre_option == 2)
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
	printf("%s\n", message);
	GLCD_DisplayString (5, 2, 1, message);
	
	sprintf(message, "Fuel Status: %d", robot.fuel_status);
	GLCD_DisplayString (7, 2, 1, message);
	printf("%s\n", message);
}

void showInstructionMenu(void)
{
	// Function which outputs game instructions to GLCD display
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


void endGameDisplay(void)
{
		// Function to display end game message on win/loss
		// Also displays final game stats
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
		printf("%s\n", message);
		GLCD_DisplayString (5, 2, 1, message);

		sprintf(message, "GOLD: %d", robot.num_gold);
		GLCD_DisplayString (6, 2, 1, message);
		printf("%s\n", message);

		sprintf(message, "EMERALD: %d", robot.num_emerald);
		GLCD_DisplayString (7, 2, 1, message);
		printf("%s\n", message);

		sprintf(message, "COPPER: %d", robot.num_copper);
		GLCD_DisplayString (8, 2, 1, message);
		printf("%s\n", message);
}

/* Functions to output menus on GLCD Display*/
