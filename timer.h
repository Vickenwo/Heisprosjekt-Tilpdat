#ifndef TIMER_H
#define TIMER_H
#include <time.h>
#include <stdio.h>
/**
 * @file 
 * @brief Functions that control the systems timer
 */


/**
 * @brief Sets a timestamp with the function @c clock() , and by doing this starts the timer.
 */
void timer_start();

/**
 * @brief Informs us if the timer has expired or not.
 * @param duration The amount of time the timer is on.
 * @return int 1 if the timer has not yet expired, 0 if the timer has expired. 
 */
int timer_notExpired(int duration);


#endif