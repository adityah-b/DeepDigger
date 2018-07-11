#include <lpc17xx.h>
#include <stdio.h>
#include <stdbool.h>
#include <RTL.h>

#include "game_bitmaps.h"

// Macros
#define MAX_SCREEN_LENGTH 12
#define MAX_SCREEN_WIDTH 60
#define GOLD 10
#define SILVER 5
#define COPPER 1

#define RIGHT (1 << 23)
#define UP (1 << 24)
#define LEFT (1 << 25)
#define DOWN (1 << 26)

#define SURFACE 2
#define NUM_LEDS 8
#define FUEL_TIME 20
#define END_GAME_THRESHOLD 300
#define NUM_SCROLL 4

// Global Variables
uint32_t min_row = 0, min_col = 0;
uint32_t max_row = 16, max_col = 12;
uint32_t fuel_x, fuel_y;
uint32_t ms_ticks;
uint32_t num_slides = 0;
bool game_over = 0;
char map[MAX_SCREEN_WIDTH][MAX_SCREEN_LENGTH]; // Temporary
bool map_scrolled = false;
OS_SEM display_refreshed, action_performed, needs_refill, fuel_refilled;

/* Character Struct */
typedef struct {
	uint32_t x_pos_prev, y_pos_prev;
	uint32_t x_pos, y_pos;

	uint32_t num_gold, num_emerald, num_copper;
	uint32_t fuel_status;
	uint32_t num_points;

	bool is_flying;
	bool select_action;
	bool game_won;

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
uint32_t* pollJoystick(void);
void pollPushbutton(void);
void setLED(uint32_t val);
void loadBMP(uint32_t row, uint32_t col);
void initMap(void);
void endGameDisplay(void);
void printMap(void);
/* Functions */

/* Tasks */
__task void updateDisplay(void);
__task void updateFuelStatus(void);
__task void moveRobot(void);
__task void buyFuel(void);
__task void mainTask(void);
/* Tasks */