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
#include "ball.h"

/****************************************************************************
 *                            Local define section                          *
 ****************************************************************************/

#define ABS(x)                     (x < 0) ? (-x) : x

// Threshold speed
#define THRESHOLD_SPEED 40.0f

// Line orientation
#define LCD_DIR_HORIZONTAL       0x0000
#define LCD_DIR_VERTICAL         0x0001

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
void LCD_FillTriangle(unsigned short x1, unsigned short x2, unsigned short x3, unsigned short y1, unsigned short y2, unsigned short y3);
void LCD_DrawFullCircle(unsigned short Xpos, unsigned short Ypos, unsigned short Radius);

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
        LCD_SetTextColor(LCD_COLOR_BLUE2);
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


/****************************************************************************
 * @brief  Draws the ball, and clears the previous one
 * @retval None
 ****************************************************************************/

void Maze_DrawTheBall(unsigned int x, unsigned int y)
{
    /* local variables */ 
    static unsigned int _x = 0; 
    static unsigned int _y = 0;
	  static bool _1stTime = true;

		/* Clear old ball (except on the 1st call) */
		if (!_1stTime)
		{
	  /* Set color to clear */
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DrawFullCircle(_x, _y, BALL_RADIUS);
		}
		else _1stTime = false;
		
    /* Set color to draw */
    LCD_SetTextColor(LCD_COLOR_RED);
    LCD_DrawFullCircle(x, y, BALL_RADIUS);
    
    /* Adjust coordinates for next draw operation */ 
    _x = x; 
    _y = y;
    
} // Maze_DrawTheBall


/****************************************************************************
 * @brief  Draws the ball, and clears the previous one
 * @retval None
 ****************************************************************************/

void Maze_DrawInner(void)
{
    /* 1st example * /
    // Draw line
    LCD_DrawLine(X_MIDDLE, Y_MIDDLE, MAZE_SIZE/3, LCD_DIR_HORIZONTAL); */

    /* 2nd example */
    // Draw lines
    LCD_DrawLine(X_MIDDLE, Y_MIDDLE-MAZE_SIZE/2, MAZE_SIZE, LCD_DIR_VERTICAL);
    LCD_DrawLine(X_MIDDLE-MAZE_SIZE/2, Y_MIDDLE, MAZE_SIZE, LCD_DIR_HORIZONTAL);

    // Make some holes
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DrawLine(X_MIDDLE, Y_MIDDLE-MAZE_SIZE/5, 4*BALL_RADIUS, LCD_DIR_VERTICAL);
    LCD_DrawLine(X_MIDDLE, Y_MIDDLE+MAZE_SIZE/4, 4*BALL_RADIUS, LCD_DIR_VERTICAL);
    LCD_DrawLine(X_MIDDLE+MAZE_SIZE/3, Y_MIDDLE, 4*BALL_RADIUS, LCD_DIR_HORIZONTAL);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    
    
    // Place the ball
    ball.x_position = X_MIDDLE - MAZE_SIZE/4;
    ball.y_position = Y_MIDDLE - MAZE_SIZE/4;
}
