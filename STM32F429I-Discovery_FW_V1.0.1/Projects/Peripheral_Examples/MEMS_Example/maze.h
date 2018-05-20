/****************************************************************************
 *              © Copyright 2000-2018 ABB. All rights reserved.
 ***************************************************************************/
/**
 * @file Maze.h
 * @brief Maze interface file 
 ************************************************************************* */

#ifndef MAZE_h       // include me once
#define MAZE_h

/****************************************************************************
 *                          Global interface include section                *
 ****************************************************************************/

#include "stdbool.h"

/****************************************************************************
 *                               Global Defines                             *
 ****************************************************************************/

// Display dimensions [pixels]
#define X_SIZE      240
#define Y_SIZE      320

// Arrow dimensions
#define BASE        36  
#define ALTEZZA     12

// Arrow position
#define ARROW_FROM_MAZE 5
#define MARGIN          1
#define X_MAX           (X_SIZE-1-MARGIN)
#define X_MIN           (MARGIN)
#define X_MIDDLE        (X_SIZE/2)
#define Y_MAX           (Y_SIZE-1-MARGIN)
#define Y_MIN           (MARGIN)
#define Y_MIDDLE        (Y_SIZE/2)

// Maze features and position
#define MAZE_SIZE     198
#define MAZE_MARGIN    10
#define MAZE_TOP_X   (X_MIDDLE-MAZE_SIZE/2)
#define MAZE_TOP_Y   (Y_MIDDLE-MAZE_SIZE/2)
#define MAZE_LEAST_X (X_MIDDLE+MAZE_SIZE/2)
#define MAZE_LEAST_Y (Y_MIDDLE+MAZE_SIZE/2)

// LCD colors
#define LCD_COLOR_WHITE 0xFFFF
#define LCD_COLOR_BLACK 0x0000

/****************************************************************************
 *                              Global Constants                            *
 ****************************************************************************/

/****************************************************************************
 *                               Global Typedef                             *
 ****************************************************************************/

typedef enum { eNONE = 0, eUP, eDOWN, eRIGHT, eLEFT } eOrientation; 

typedef struct {

	eOrientation orientation; 		// board orientation
	eOrientation oldOrientation; 	// board orientation (previous value)
	
} Maze;

/****************************************************************************
 *                        Variables exported by this module                 *
 ****************************************************************************/

// Maze
extern Maze maze;

/****************************************************************************
 *                        Function exported by this module                  *
 ****************************************************************************/

// Calculates Maze orientation
bool Maze_GetNewOrientation(float XSpeed, float YSpeed);

// Draws the maze's outer border 
void Maze_DrawBorder(void);

// Draws the board orientatation 
void Maze_DrawBoardOrientation(unsigned int orientation, unsigned int oldOrientation);
	
#endif      // include me once
