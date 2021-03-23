#ifndef ORDERS_H
#define ORDERS_H

#include "hardware.h"
#include "lights.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/**
 * @file
 * @brief Several functions for taking the orders made to the elevator.
 */


/**
 * @brief the queue of the up orders.
 */
int up_orders[4];

/**
 * @brief the queue of the down orders.
 */
int down_orders[4];

/**
 * @brief the queue of the inside orders.
 */
int inside_orders[4];

/**
 * @brief Function for saving the orders made to the elevator to the arrays. 
 * @param floor The floor where the button has been pushed.
 * @param order_type The order type of the button being pushed.
 */

void orders_saveOrder(int floor, HardwareOrder order_type);

/**
 * @brief Deletes all the orders made on a given floor.
 * @param floor The floor the elevator is currently on.
 */

void orders_deleteFromQueue(int floor);

/**
 * @brief Deletes all the orders in all of the queues.
 */

void orders_deleteAllOrders();

/**
 * @brief Checks if the queue of the up orders is empty.
 * @return int 1 if the queue is empty, 0 if it has remaining orders.
 */
int orders_upOrdersEmpty();

/**
 * @brief Checks if the queue of the down orders is empty.
 * 
 * @return int 1 if the queue is empty, 0 if it has remaining orders.
 */
int orders_downOrdersEmpty();

/**
 * @brief Checks if the queue of the inside orders is empty.
 * 
 * @return int 1 if the queue is empty, 0 if it has remaining orders.
 */
int orders_insideOrdersEmpty();

#endif /* ORDERS_H */