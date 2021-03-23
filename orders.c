#include <stdio.h>
#include "orders.h"
#include "timer.h"


void orders_saveOrder(int floor, HardwareOrder order_type)
{
    if (hardware_read_order(floor, order_type))
    {
        if (order_type == HARDWARE_ORDER_UP){
            up_orders[floor] = 1;
        }
        if (order_type == HARDWARE_ORDER_INSIDE){
            inside_orders[floor] = 1;
        }
        if (order_type == HARDWARE_ORDER_DOWN){
            down_orders[floor] = 1;
        }
    }
}

void orders_deleteFromQueue(int floor){
  up_orders[floor]=0;
  down_orders[floor]=0;
  inside_orders[floor]=0;
}


void orders_deleteAllOrders(){
  for (int i=0; i<HARDWARE_NUMBER_OF_FLOORS;i++){
    up_orders[i]=0;
    down_orders[i]=0;
    inside_orders[i]=0;
  }
  lights_clearAll();
}