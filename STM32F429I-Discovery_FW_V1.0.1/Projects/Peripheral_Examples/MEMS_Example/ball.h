/****************************************************************************
 *              � Copyright 2000-2018 ABB. All rights reserved.
 ***************************************************************************/
/**
 * @file Ball.h
 * @brief Ball interface file 
 ************************************************************************* */

#ifndef BALL_h       // include me once
#define BALL_h

/****************************************************************************
 *                          Global interface include section                *
 ****************************************************************************/

/****************************************************************************
 *                               Global Defines                             *
 ****************************************************************************/

// Speed
#define MAX_SPEED 120.0f    // Top speed [pixel/s]
#define MIN_SPEED   0.8f    // Minimum speed

// Acceleration
#define G_accel    60.0f    // Acceleration due to gravity [pixel/s^2]
#define F_decel    10.0f    // Deceleration due to friction [pixel/s^2]

#define BALL_RADIUS   6			// [pixels]
#define HOLE_RADIUS	 12

/****************************************************************************
 *                               Global Typedef                             *
 ****************************************************************************/

typedef struct {

    // Accelerations
    float x_accel;
    float y_accel;
    
    // Speeds
    float x_speed;
    float y_speed;

    // Position
    float x_position;
    float y_position;
    
    // Distance from the hole
    float distance;
    
} Ball;

/****************************************************************************
 *                        Variables exported by this module                 *
 ****************************************************************************/

// Ball model
extern Ball ball;

/****************************************************************************
 *                        Function exported by this module                  *
 ****************************************************************************/

// Adjust Ball acceleration, speed and position
void Ball_Adjust_aVS(unsigned int orientation);
    
// Check if the ball is close to a wall, in both directions
void Ball_checkProxymity(void);

// Manage the hole sink 
void Ball_checkHole(void);

#endif      // include me once
