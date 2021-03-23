#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "fsm.h"
#include "orders.h"
#include "timer.h"

static void sigint_handler(int sig)
{
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

int main()
{
    int error = hardware_init();
    if (error != 0)
    {
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);

    fsm_initialize();
    lights_clearAll();

    while (1)
    {
        for (int i = 0; i < 4; i++)
        {
            orders_saveOrder(i, HARDWARE_ORDER_UP);
            orders_saveOrder(i, HARDWARE_ORDER_DOWN);
            orders_saveOrder(i, HARDWARE_ORDER_INSIDE);
        }
        

        lights_setOrderLights();
        
        fsm_currentFloor();

        hardware_command_floor_indicator_on(currentFloor);

        if (currentFloor==maxFloor || currentFloor==minFloor || state==FSM_IDLE){
            if(!inMovement){
               fsm_setState(); 
            }
        }   
        
        fsm_prioritizeOrders();
        
        if(!timer_notExpired(3) && doorOpen){
            doorOpen=0;
            lights_setDoorLight(0);
        }

        fsm_stop();

        fsm_recentlyStopped();
        
        fsm_obstruction();

        printf("%d", state);
    }


    return 0;
}