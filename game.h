#include <lpc17xx.h>
#include <stdio.h>
#include <stdbool.h>
#include <RTL.h>

#include "game_bitmaps.h"

// Macros
#define MAX_SCREEN_LENGTH 100
#define MAX_SCREEN_WIDTH 100
#define GOLD 10
#define SILVER 5
#define COPPER 1
#define LEFT (1 << 23)
#define UP (1 << 24)
#define RIGHT (1 << 25)
#define DOWN (1 << 26)
#define SURFACE 2
#define NUM_LEDS 8
#define FUEL_TIME 10

// Global Variables
uint32_t min_row, min_col;
uint32_t max_row, max_col;
uint32_t ms_ticks;
bool gameOver = 0;
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

/* Semaphore */
typedef uint32_t sem_t;
void init(sem_t *s, uint32_t count);
void wait(sem_t *s);
void signal(sem_t *s);
/* Semaphore */

/* Functions */
/*inline*/ void pollJoystick(void);
/*inline*/ void pollPushbutton(void);
/*inline*/ void setLED(uint32_t val);
void loadBMP(uint32_t row, uint32_t col);
/* Functions */

/* Tasks */
__task void updateDisplay(void);
__task void updateFuelStatus(void);
__task void moveRobot(void);
__task void buyFuel(void);
__task void mainTask(void);
/* Tasks */
