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

void LCD_DrawRect(unsigned short Xpos, unsigned short Ypos, unsigned short Height, unsigned short Width);
void LCD_SetTextColor(unsigned short Color);
void LCD_DrawLine(unsigned short Xpos, unsigned short Ypos, unsigned short Length, unsigned char Direction);

/****************************************************************************
 *                           Code: private functions
 ****************************************************************************/

/* functions here should be declared as static */

/****************************************************************************
 *                            Code: public functions
 ****************************************************************************/

/****************************************************************************
 * @brief  Draws the maze's outer border 
 * @retval None
 ****************************************************************************/

void Maze_DrawBorder(void)
{
    /* Draw the border */ 
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_DrawRect(MAZE_TOP_X, MAZE_TOP_Y, MAZE_SIZE, MAZE_SIZE);
    
} // end Maze_DrawBorder


/****************************************************************************
 * @brief  Draws the board orientatation 
 * @retval None
 ****************************************************************************/

void Maze_DrawBoardOrientation(unsigned int orientation, unsigned int oldOrientation)
{
    /* Local variables */ 
    unsigned int _orientation = oldOrientation;
    
    /* Init loop variables: 
    1st time: write a white triangle over the old one
    2nd time: write a black triangle with the new orientation
    */
    
    /* Set color */
    LCD_SetTextColor(LCD_COLOR_WHITE);
    
    for (int count = 0; count < 2; count++)
    {
    
        /* Adjust drawing */ 
        switch (_orientation)
        {
            case eNONE:
                break;
            
            case eUP: 
                LCD_FillTriangle(X_MIDDLE-BASE/2, X_MIDDLE+BASE/2, X_MIDDLE, MAZE_TOP_Y+MAZE_SIZE+ARROW_FROM_MAZE, MAZE_TOP_Y+MAZE_SIZE+ARROW_FROM_MAZE, MAZE_TOP_Y+MAZE_SIZE+ARROW_FROM_MAZE+ALTEZZA);
                break; 
          
            case eDOWN:
                LCD_FillTriangle(X_MIDDLE-BASE/2, X_MIDDLE+BASE/2, X_MIDDLE, MAZE_TOP_Y-ARROW_FROM_MAZE, MAZE_TOP_Y-ARROW_FROM_MAZE, MAZE_TOP_Y-ARROW_FROM_MAZE-ALTEZZA);
                break; 
          
            case eRIGHT:
                LCD_FillTriangle(MAZE_TOP_X-ARROW_FROM_MAZE, MAZE_TOP_X-ARROW_FROM_MAZE, MAZE_TOP_X-ARROW_FROM_MAZE-ALTEZZA, Y_MIDDLE-BASE/2, Y_MIDDLE+BASE/2, Y_MIDDLE);
                break; 
          
            case eLEFT: 
                LCD_FillTriangle(MAZE_TOP_X+MAZE_SIZE+ARROW_FROM_MAZE, MAZE_TOP_X+MAZE_SIZE+ARROW_FROM_MAZE, MAZE_TOP_X+MAZE_SIZE+ARROW_FROM_MAZE+ALTEZZA, Y_MIDDLE-BASE/2, Y_MIDDLE+BASE/2, Y_MIDDLE);
                break;
          
            default: 
                break;
        }
    
        /* Set color and change orientation variable */
        LCD_SetTextColor(LCD_COLOR_BLACK);
        _orientation = orientation;
    } 

} // end Maze_DrawBoardOrientation

/****************************************************************************
 * @brief  Calculates Maze orientation 
 * @retval true The board orientation has changed
 * @retval false No orientation change
 ****************************************************************************/

bool Maze_GetNewOrientation(float XSpeed, float YSpeed) {

	// Local variables
	static unsigned int timeout = 0;
	bool newOrientation = false;
	float XValue = ABS(XSpeed);
	float YValue = ABS(YSpeed);
	
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
			if (maze.orientation != maze.oldOrientation)
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
