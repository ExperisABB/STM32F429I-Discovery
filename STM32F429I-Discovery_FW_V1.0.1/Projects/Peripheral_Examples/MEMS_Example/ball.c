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
#include "math.h"

/****************************************************************************
 *                            Local define section                          *
 ****************************************************************************/

// Proximity calculation
#define NEW_PROXI_METHOD  true

// Wall orientation 
#define VERTICAL_WALL			false
#define HORIZONTAL_WALL   true
	
// Elastic impact
#define ELASTIC_K					(0.75f)

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
    
		#if (NEW_PROXI_METHOD == false)
		
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
		
		#endif
}

#if (NEW_PROXI_METHOD == true)

/****************************************************************************
 * @brief  Check if the ball is close to a wall 
 * @param  horizontal check if the wall is horizontal or vertical
 * @retval true the ball is close to the wall
 * @retval false the ball is not close to the wall
 ****************************************************************************/

unsigned int Ball_closeToWall(bool horizontal)
{
    unsigned int Xaddress;
    int offset;
    int x_offset, y_offset; 
    unsigned int close = false;
    unsigned int fast = false;
    
    // Check wall disposition
    if (!horizontal)
    {
        // Vertical wall detection 
        
        // Set X offset 
        offset = -BALL_RADIUS;
        
        // Check direction 
        if (ball.x_speed >= 0) 
        {
            x_offset = BALL_RADIUS + MARGIN;
            
            // Detect fast balls
            if (ball.x_speed + ball.x_accel > 100.0f) fast = true;
        }
        else 
        {
            x_offset = -BALL_RADIUS - MARGIN;

            // Detect fast balls
            if (ball.x_speed + ball.x_accel < -100.0f) fast = true;
        }

        // Proximity loop
        while (offset <= BALL_RADIUS)
        {
            // Get pixel address
            Xaddress = LCD_SetCursor(ball.x_position + x_offset, ball.y_position + offset);
            
            // Check pixel content
            if ((*(unsigned short int *)Xaddress == LCD_COLOR_BLACK) || 
                (fast && (*(unsigned short int *)(Xaddress+2) == LCD_COLOR_BLACK)))
            {
                // Set close found and quit the loop
                close = true;
                ball.x_position = ball.x_position - ball.x_speed / 100.0f;
                
                break;
            }
            // Increase the offset
            offset++;
        }
    }
    else 
    {
        // Horizontal wall detection 
        
        // Set X offset 
        offset = -BALL_RADIUS;
        
        // Check direction 
        if (ball.y_speed >= 0) 
        {
            y_offset = BALL_RADIUS + MARGIN;

            // Detect fast balls
            if (ball.y_speed + ball.y_accel > 100.0f) fast = true;
        }
        else 
        {
            y_offset = -BALL_RADIUS - MARGIN;

            // Detect fast balls
            if (ball.y_speed + ball.y_accel < -100.0f) fast = true;
        }

        // Proximity loop
        while (offset <= BALL_RADIUS)
        {
            // Get pixel address
            Xaddress = LCD_SetCursor(ball.x_position + offset, ball.y_position + y_offset);
            
            // Check pixel content
            if ((*(unsigned short int *)Xaddress == LCD_COLOR_BLACK)  || 
                (fast && (*(unsigned short int *)(Xaddress+2*X_SIZE) == LCD_COLOR_BLACK)))
            {
                // Set close found
                close = true;
                ball.y_position = ball.y_position - ball.y_speed/100.0f;
                
                // Quit the loop
                break;
            }
            
            // Increase the offset
            offset++;
        }
    }
    
    // Return result
    return close;
    
} // end Ball_closeToWall


/****************************************************************************
 * @brief  Check if the ball is close to a wall, in both directions
 ****************************************************************************/

void Ball_checkProxymity(void)
{
    /* Proximity test: vertical wall ? */
    if (Ball_closeToWall(VERTICAL_WALL)) 
    {
      //LCD_SetTextColor(LCD_COLOR_YELLOW);
      //ball.x_position = MAZE_TOP_X+BALL_RADIUS+1;
      ball.x_speed = -ball.x_speed * ELASTIC_K;
    }
    
    /* Proximity test: horizontal wall ? */
    if (Ball_closeToWall(HORIZONTAL_WALL)) 
    {
        //LCD_SetTextColor(LCD_COLOR_YELLOW);
        ball.y_speed = -ball.y_speed * ELASTIC_K;
    }
}

#endif // NEW_PROXY_METHOD


/****************************************************************************
 * @brief  Manage the hole sink 
 ****************************************************************************/

void Ball_checkHole(void) {
    
    // Calculate catetis
    float catetoX = ball.x_position - maze.hole.X;
    float catetoY = ball.y_position - maze.hole.Y;
    
    // Calculate the distance from the hole
    ball.distance = sqrt(catetoX * catetoX + catetoY * catetoY);
    
    // Check if the ball is inside the hole
    if (ball.distance <= (float)(HOLE_RADIUS-1))
    {
        // Set position to hole center
        ball.x_position = maze.hole.X;
        ball.y_position = maze.hole.Y;
        
        // Clear speed
        ball.x_speed = 0.0f;
        ball.y_speed = 0.0f;
    }

    
    
    
}


