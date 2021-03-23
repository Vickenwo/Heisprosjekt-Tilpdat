/**
 * @file 
 * @brief Driver for the elevator final state machine 
*/ 

#ifndef FSM_H
#define FSM_H
#include <stdio.h>
#include "timer.h"
#include "orders.h"
#include "hardware.h"

/**
 * @brief State type used in @c fsm_setState, @c fsm_checkInsideOrders and in @c fsm_prioritizeOrders .
 */
typedef enum 
{
    FSM_IDLE,
    FSM_ASCENDING,
    FSM_DECENDING,
} FsmElevstate;

/**
 * @brief The variable that the elevator state is saved in.
 */
FsmElevstate state;

/**
 * @brief The variable that saves the current floor.
 */
int currentFloor;

/**
 * @brief The variable that is 1 if the door is open, and 0 if the door is closed.
 * Used in @c fsm_doorOpen .
 */
int doorOpen;

/**
 * @brief The variable that saves the maximum floor of the orders.
 */
int maxFloor;

/**
 * @brief The variable that saves the minimum floor of the orders.
 */
int minFloor;

/**
 * @brief Informs us if the elevator stopped on a floor when the stop button was pushed. 1 if it did, 0 if it did not.
 * Used in @c fsm_stop .
 */

int stoppedOnFloor;

/**
 * @brief Informs us if the stop button was recently pushed, 1 if it was, 0 if it was not.
 * Used in @c fsm_recentlyStopped .
 */
int recentlyStopped;

/**
 * @brief Informs us if the elevator was stopped between floors. -1 if it was decending, 1 if it was ascending.
 * Used in @c fsm_recentlyStopped .
 */
int betweenFloors;

/**
 * @brief Informs us if the elevator is moving or not. 1 if movement is either @c HARDWARE_MOVEMENT_UP or @c HARDWARE_MOVEMENT_DOWN , 
 * 0 if movement is @c HARDWARE_MOVEMENT_STOP .
 * Changes everytime the @c hardware_command_movement is being called.
 */
int inMovement;


/**
 * @brief Calculates the maximum floor of the orders.
 * Must be calculated to make sure that the state does not change until the elevator has reached its final destination in that direction.
 * Saves the value in the global int value @c maxFloor .
*/ 
void fsm_maxFloor();

/**
 * @brief Calculates the minimum floor of the orders.
 * Must be calculated to make sure that the state does not change until the elevator has reached its final destination in that direction.
 * Saves the value in the global int value @c minFloor .
*/ 
void fsm_minFloor();


/**
 * @brief Calculates the currentFloor that the elevator is on, and saves this value in the global int variable @c currentFloor .
 * Must be called to know where the elevator is.
 * It also saves the variable @c betweenFloors to deside if the elevator is between two floors.
 */
void fsm_currentFloor();

/**
 * @brief Must be called to make sure that the elevator is in a defined state.
 */

void fsm_initialize();

/**
 * @brief Is called when the elevator has stopped at a floor.
 * It deletes all orders on that floor and clears the order lights as well as sets the door light.
 * It also sets the timer and sets the variables @c doorOpen and @c recentlyStopped
 * @param floor gives the floor that the elevator has stopped on.
 */

void fsm_doorOpen(int floor);

/**
 * @brief Sets the elevator state to either @c FSM_ASCENDING and @c FSM_DECENDING depending on which direction the elevator is moving.
 * This is calculated based on which floors that has orders and if they are higher up or lower than  @c currentFloor .
 */

void fsm_setState();

/**
 * @brief Checks the orders in @c up_orders when the elevator is ascending.
 * It then moves to the requested floors, stops, and calls @c fsm_DoorOpen .
 */

void fsm_checkUpOrdersAscending();

/**
 * @brief Checks the orders in @c up_orders when the elevator is decending.
 * It then moves to the requested floors, stops, and calls @c fsm_DoorOpen .
 */

void fsm_checkUpOrdersDecending();

/**
 * @brief Checks the orders in @c down_orders when the elevator is decending.
 * It then moves to the requested floors, stops, and calls @c fsm_DoorOpen .
 */

void fsm_checkDownOrdersDecending();

/**
 * @brief Checks the orders in @c down_orders when the elevator is ascending.
 * It then moves to the requested floors, stops, and calls @c fsm_DoorOpen .
 */

void fsm_checkDownOrdersAscending();

/**
 * @brief Takes the inside orders from the @c inside_orders list, and checks whether the elevator should go up or down, and moves and stops accordingly. 
 */

void fsm_checkInsideOrders();

/**
 * @brief Prioritizes the orders made from the different lists, and runs them in the right order. 
 * This is based on which direction the elevator is moving.
 */

void fsm_prioritizeOrders();

/**
 * @brief Stopps the elevator while the stop button is pushed, and makes sure that no orders are processed during this time.
 * If the elevator was stopped on a floor the value @c stoppedOnFloor is given the value 1 and the door is opened until the timer runs out.
 */
void fsm_stop();

/**
 * @brief Makes sure that the elevator is able to move to the most recent floor if the stop button was pushed between two floors.
 */

void fsm_recentlyStopped();

/**
 * @brief Controls the obstruction switch, and makes sure that the system is only affected by this when the door is open. 
 * 
 */

void fsm_obstruction();

#endif