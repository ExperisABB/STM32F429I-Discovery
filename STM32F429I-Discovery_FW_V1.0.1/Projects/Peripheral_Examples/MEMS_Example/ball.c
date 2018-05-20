/****************************************************************************
 *              © Copyright 2000-2018 ABB. All rights reserved.
 ****************************************************************************/
/**
 * @file ball.c
 * @brief ball-related code for Experis Project Work
 * @author Marco Stucchi
 ****************************************************************************/

/****************************************************************************
 *                              Include section                             *
 ****************************************************************************/

#include "ball.h"
#include "maze.h"

/****************************************************************************
 *                            Local define section                          *
 ****************************************************************************/

/****************************************************************************
 *                         Types declaration section                        *
 ****************************************************************************/

/****************************************************************************
 *                            Variables definition                          *
 ****************************************************************************/

// Ball model
Ball ball = {.x_position = X_MIDDLE, .y_position = Y_MIDDLE};

/****************************************************************************
 *                            Constants definition                          *
 ****************************************************************************/

/****************************************************************************
 *                                 Prototypes                               *
 ****************************************************************************/

unsigned int LCD_SetCursor(unsigned short int Xpos, unsigned short int Ypos);

/****************************************************************************
 *                           Code: private functions
 ****************************************************************************/

/* functions here should be declared as static */

/****************************************************************************
 *                            Code: public functions
 ****************************************************************************/

/****************************************************************************
 * @brief  Adjust Ball acceleration, speed and position
 * @retval None
 ****************************************************************************/

void Ball_Adjust_aVS(unsigned int orientation) 
{
    float x_accel, y_accel;
    
    // Calculate acceleration due to board orientation
    switch (orientation)
    {
        case eNONE:
            x_accel = 0;
            y_accel = 0;
            break;
        
        case eUP:
            x_accel = 0;
            y_accel = G_accel;
            break;

        case eDOWN:
            x_accel = 0;
            y_accel = -G_accel;
            break;

        case eRIGHT:
            x_accel = -G_accel;
            y_accel = 0;
            break;
        
        case eLEFT:
            x_accel = G_accel;
            y_accel = 0;
            break;
    }
    
    // Check for deceleration (X axis)
    if (x_accel == 0)
    {   // Flat board: if there's a speed, work to stop the ball
        if (ball.x_speed >= MIN_SPEED)
        {
            x_accel = -F_decel;
        }           
        else if (ball.x_speed <= -MIN_SPEED)
        {
            x_accel = F_decel;
        }
        else ball.x_speed = 0;
    }
        
    // Check for deceleration (Y axis)
    if (y_accel == 0)
    {   // Flat board: if there's a speed, work to stop the ball
        if (ball.y_speed >= MIN_SPEED)
        {
            y_accel = -F_decel;
        }           
        else if (ball.y_speed <= -MIN_SPEED)
        {
            y_accel = F_decel;
        }
        else ball.y_speed = 0;
    }

    // Speed calculation
    ball.x_speed += x_accel/100.0f;
    ball.y_speed += y_accel/100.0f;
    
    // Speed limit check
    if (ball.x_speed > MAX_SPEED) ball.x_speed = MAX_SPEED;
    if (ball.x_speed < -MAX_SPEED) ball.x_speed = -MAX_SPEED;
    if (ball.y_speed > MAX_SPEED) ball.y_speed = MAX_SPEED;
    if (ball.y_speed < -MAX_SPEED) ball.y_speed = -MAX_SPEED;
    
    // Adjust position
    ball.x_position += ball.x_speed / 100.0f;
    ball.y_position += ball.y_speed / 100.0f;
    
    // Check limits
    if ((unsigned int)ball.x_position < (MAZE_TOP_X+BALL_RADIUS+1)) 
    { ball.x_position = MAZE_TOP_X+BALL_RADIUS+1;
      ball.x_speed = -ball.x_speed * 0.6f;
    }

    if ((unsigned int)ball.x_position > (MAZE_LEAST_X-BALL_RADIUS-1)) 
    { ball.x_position = MAZE_LEAST_X-BALL_RADIUS-1;
      ball.x_speed = -ball.x_speed * 0.6f;
    }

    // Check limits
    if ((unsigned int)ball.y_position < (MAZE_TOP_Y+BALL_RADIUS+1)) 
    { ball.y_position = MAZE_TOP_Y+BALL_RADIUS+1;
      ball.y_speed = -ball.y_speed * 0.6f;
    }
    
    if ((unsigned int)ball.y_position > (MAZE_LEAST_Y-BALL_RADIUS-1)) 
    { ball.y_position = MAZE_LEAST_Y-BALL_RADIUS-1;
      ball.y_speed = -ball.y_speed * 0.6f;
    }
}

