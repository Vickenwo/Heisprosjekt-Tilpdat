#ifndef LIGHTS_H
#define LIGHTS_H

#include "io.h"
#include "hardware.h"
#include "channels.h"
#include <time.h>
#include <stdlib.h>
#include <signal.h>

/**
 * @file
 * @brief All the light functions for the elevator.
 */

/**
 * @brief Sets door light if door is open.
 * @param open tells if door is open or closed, 1 if open, 0 if closed.
*/
void lights_setDoorLight(int open);

/**
 * @brief Sets stop light if stop butten is pushed.
 * @param stop tells if stop button is pushed or not.
*/
void lights_setStopLight(int stop);

/**
 * @brief Clears all the lights on the panel.
 * This does not include the door light or the current floor light.
*/

void lights_clearAll();

/**
 * @brief Clears all the lights on the floor.
 * @param floor the floor the elevator is currently on.
*/

void lights_clearLightsOnFloor(int floor);

/**
 * @brief Clears all the order lights on all floors.
 */

void lights_clearAllOrderLights();

/**
 * @brief Sets the order light on the order button that has been pushed.
 */
void lights_setOrderLights();

#endif