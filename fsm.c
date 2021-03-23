#include <stdio.h>
#include "fsm.h"

void fsm_currentFloor()
{
    for (int i = 0; i < 4; i++)
    {
        if (hardware_read_floor_sensor(i))
        {
            currentFloor=i;
            if (state ==FSM_ASCENDING){
              betweenFloors = 1;
            }
            if (state ==FSM_DECENDING){
              betweenFloors =-1;
            }
        }
    }
}

void fsm_initialize()
{
    int initializeDone = 0;
    while (!initializeDone)
    {
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
            if (hardware_read_floor_sensor(i))
            {
                hardware_command_movement(HARDWARE_MOVEMENT_STOP);
                initializeDone=1;
            }
        }
    }
}

void fsm_doorOpen(int floor){
    doorOpen=1;
    recentlyStopped=1;
    timer_start();
    lights_setDoorLight(1);  
    lights_clearLightsOnFloor(floor);
    orders_deleteFromQueue(floor);
}

void fsm_maxFloor(){
  for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
    if (inside_orders[i] || up_orders[i] || down_orders[i]){
      maxFloor = i;
    }
  }
}

void fsm_minFloor(){
  for (int i = HARDWARE_NUMBER_OF_FLOORS; i > -1; i--){
    if (inside_orders[i] || up_orders[i] || down_orders[i]){
      minFloor = i;
    }
  }
}


void fsm_setState(){
    for (int i=0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
        if (up_orders[i] || down_orders[i] || inside_orders[i]){
            if (currentFloor < i){
                state=FSM_ASCENDING;
            }
            else if (currentFloor > i){
                state=FSM_DECENDING;
            }
        }
    }
}



void fsm_checkUpOrdersAscending(){
  for (int i=currentFloor; i<4; i++){
    if (up_orders[i]){
      if (currentFloor<i && !timer_notExpired(3)){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        inMovement=1;
      }
      if (hardware_read_floor_sensor(i)){
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        inMovement=0;
        fsm_doorOpen(i);
        fsm_currentFloor();
      }
    }
  }
}

void fsm_checkUpOrdersDecending(){
  for (int i=currentFloor; i>-1; i--){
    if (up_orders[i]){
      if (currentFloor>i && !timer_notExpired(3)){
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        inMovement=1;
      } 
      if (hardware_read_floor_sensor(i)){
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        inMovement=0;
        fsm_doorOpen(i);
        fsm_currentFloor();
      }
    }
  }
}

void fsm_checkDownOrdersDecending(){
  for (int i=currentFloor; i>-1; i--){
    if (down_orders[i]){
      if (currentFloor>i && !timer_notExpired(3)){
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        inMovement=1;
      }  
      if (hardware_read_floor_sensor(i)){   
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        inMovement=0;
        fsm_doorOpen(i);
        fsm_currentFloor();
      }
    }
  }
}

void fsm_checkDownOrdersAscending(){
  for (int i=currentFloor; i<4; i++){
    if (down_orders[i]){
      if (currentFloor<i && !timer_notExpired(3)){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        inMovement=1;
      }
      if (hardware_read_floor_sensor(i)){
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        inMovement=0;
        fsm_doorOpen(i);
        fsm_currentFloor();
      }
    }
  }
}

void fsm_checkInsideOrders(){
  if (state==FSM_ASCENDING){
    for (int i=currentFloor; i<4; i++){
      if (inside_orders[i]){
        if (currentFloor<i && !timer_notExpired(3)){
          hardware_command_movement(HARDWARE_MOVEMENT_UP);
          inMovement=1;
        }
        if (hardware_read_floor_sensor(i)){
          hardware_command_movement(HARDWARE_MOVEMENT_STOP);
          inMovement=0;
          fsm_doorOpen(i);
          fsm_currentFloor();
        }
      }
    }
  }
  else if (state==FSM_DECENDING){
    for (int i=currentFloor; i>-1; i--){
      if (inside_orders[i]){
        if (currentFloor>i && !timer_notExpired(3)){
          hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
          inMovement=1;
        }
        if (hardware_read_floor_sensor(i)){
          hardware_command_movement(HARDWARE_MOVEMENT_STOP);
          inMovement=0;
          fsm_doorOpen(i);
          fsm_currentFloor();
        }
      }
    }
  }
}

void fsm_prioritizeOrders(){
  fsm_maxFloor();
  fsm_minFloor();
  if (state==FSM_ASCENDING){
      fsm_checkUpOrdersAscending();
      fsm_checkInsideOrders(); 
      if ((orders_upOrdersEmpty() && orders_insideOrdersEmpty()) || inside_orders[0]){
        fsm_checkDownOrdersAscending();  
        }
  }
  else if(state==FSM_DECENDING){
      fsm_checkDownOrdersDecending();
      fsm_checkInsideOrders();
      if ((orders_downOrdersEmpty() && orders_insideOrdersEmpty()) || inside_orders[3]){
          fsm_checkUpOrdersDecending();
      }
  }
  if (up_orders[0] && down_orders[3]){
    if (state==FSM_ASCENDING){
      fsm_checkDownOrdersAscending();
    }
    if (state==FSM_DECENDING){
      fsm_checkUpOrdersDecending();
    }
  }
}

void fsm_stop(){

    while(hardware_read_stop_signal(1)){
    recentlyStopped=1;
    lights_clearAllOrderLights();
    for (int i=0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
      if(hardware_read_floor_sensor(i)){
        doorOpen=1;
        lights_setDoorLight(1);
        stoppedOnFloor=1;
        timer_start();
        lights_clearAllOrderLights();

      }
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    inMovement=0;
    lights_setStopLight(1);
    orders_deleteAllOrders();
  }  
  if (stoppedOnFloor==1){
    lights_setDoorLight(1);
    lights_clearAllOrderLights();
    if (!timer_notExpired(3)){
      stoppedOnFloor=0;
      lights_setDoorLight(0);
    }       
  }
}


void fsm_recentlyStopped(){
  if (recentlyStopped){
    if(inside_orders[currentFloor] || up_orders[currentFloor] || down_orders[currentFloor]){
      if (betweenFloors==1){
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        inMovement=1;
      }
      else if (betweenFloors==-1){
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        inMovement=1;
      }
    }
  }
  if (hardware_read_floor_sensor(currentFloor)){
    recentlyStopped=0;
  }
}

void fsm_obstruction(){
  if (hardware_read_obstruction_signal() && doorOpen){
    lights_setDoorLight(1);
    timer_start();
  }
}
