#include <lpc17xx.h>
#include <stdio.h>
#include <stdbool.h>
#include <RTL.h>

// Macros
#define MAX_SCREEN_LENGTH 100
#define MAX_SCREEN_WIDTH 100
#define GOLD 10
#define SILVER 5
#define COPPER 1
#define UP (1 << 26)
#define DOWN (1 << 24)
#define LEFT (1 << 25)
#define RIGHT (1 << 23)

// Global Variables
uint32_t min_row, min_col;
uint32_t max_row, max_col;
char map[MAX_SCREEN_WIDTH][MAX_SCREEN_LENGTH]; // Temporary

/* Character Struct */
typedef struct {
	uint32_t x_pos, y_pos;

	uint32_t num_gold, num_silver, num_copper;
	uint32_t fuel_status;
	uint32_t num_points;

	bool is_flying;
	bool select_action;

	// Robot Inputs
	volatile uint32_t dir;
} robot_t;
robot_t robot;
/* Character Struct */

/* Semaphore Struct */
typedef uint32_t sem_t;

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
/* Semaphore Struct */

/* Tasks */
__task void updateDisplay(void);
__task void updateFuelStatus(void);
__task void moveRobot(void);
__task void buyFuel(void);

__task void mainTask(void)
{
	// Create tasks
	os_tsk_create(updateDisplay, 1);

	os_tsk_create(updateFuelStatus, 2);
	os_tsk_create(moveRobot, 2);

	os_tsk_create(buyFuel, 3);

	os_tsk_delete_self();
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
			num_bars = 8 - robot.fuel_status;
			cost = 2*num_bars;
			robot.fuel_status = 8;
			robot.num_points -= cost;
		}
		os_tsk_pass();
	}
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
/* Functions */