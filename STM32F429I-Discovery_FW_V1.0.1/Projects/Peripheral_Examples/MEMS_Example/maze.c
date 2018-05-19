/****************************************************************************
 *              © Copyright 2000-2018 ABB. All rights reserved.
 ****************************************************************************/
/**
 * @file maze.c
 * @brief maze-related code for Experis Project Work
 * @author Marco Stucchi - ABB
 ****************************************************************************/

/****************************************************************************
 *                              Include section                             *
 ****************************************************************************/

#include "maze.h"

/****************************************************************************
 *                            Local define section                          *
 ****************************************************************************/

#define ABS(x)                     (x < 0) ? (-x) : x

// Threshold speed
#define THRESHOLD_SPEED 40.0f

/****************************************************************************
 *                         Types declaration section                        *
 ****************************************************************************/

#define true    1
#define false   0

/****************************************************************************
 *                            Variables definition                          *
 ****************************************************************************/

// Maze
Maze maze;

/****************************************************************************
 *                                 Prototypes                               *
 ****************************************************************************/

/****************************************************************************
 *                           Code: private functions
 ****************************************************************************/

/* functions here should be declared as static */

/****************************************************************************
 *                            Code: public functions
 ****************************************************************************/

// Calculates Maze orientation
bool Maze_GetNewOrientation(float XSpeed, float YSpeed) {

	// Local variables
	static unsigned int timeout = 0;
	eOrientation oldOrientation;
	bool newOrientation = false;
	float XValue = ABS(XSpeed);
	float YValue = ABS(YSpeed);
	
	// Save present orientation
	oldOrientation = maze.orientation;
	
  /* Timeout management */ 
  if (timeout > 0)
  {
      // Subtract call rate
      timeout -= 10;
  }
  else 
  {
      /* To detect movement, prioritize the fastest moving axis */ 
      if ( XValue > YValue)
      {
          /* Priority on the X axis:detect high rotation speed */
          if (XSpeed > THRESHOLD_SPEED)
          {
              /* Check present orientation: */
							maze.orientation = (maze.orientation == eDOWN) ? eNONE : eUP;
          }
          else if (XSpeed < -THRESHOLD_SPEED)
          {
              /* Check present orientation: */
							maze.orientation = (maze.orientation == eUP) ? eNONE : eDOWN;
          }
      }
      else 
      {
          /* Priority on the y axis:detect high rotation speed */
          if (YSpeed > THRESHOLD_SPEED)
          {
              /* Check present orientation: */
							maze.orientation = (maze.orientation == eRIGHT) ? eNONE : eLEFT;
          }
          else if (YSpeed < -THRESHOLD_SPEED)
          {
              /* Check present orientation: */
							maze.orientation = (maze.orientation == eLEFT) ? eNONE : eRIGHT;
          }
      }

			// Check for a change in the orientation
			if (maze.orientation != oldOrientation)
			{
					// Set new orientation
					newOrientation = true;

					// Preset timeout
					timeout = 400;
			}		
  }	

	// Return new orientation	detection
	return newOrientation;
	
} // Maze_GetNewOrientation
