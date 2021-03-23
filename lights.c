#include "lights.h"


void lights_setDoorLight(int open) {
	if (open) {
		io_set_bit(LIGHT_DOOR_OPEN);
	}
	else
		io_clear_bit(LIGHT_DOOR_OPEN);
}

void lights_setStopLight(int stop) {
	if (stop) {
		io_set_bit(LIGHT_STOP);
	}
	else
		io_clear_bit(LIGHT_STOP);
}

void lights_clearAll(){
	hardware_command_stop_light(0);
	hardware_command_door_open(0);
	lights_clearAllOrderLights();
}


void lights_clearAllOrderLights()
{
    HardwareOrder order_types[3] = {
        HARDWARE_ORDER_UP,
        HARDWARE_ORDER_INSIDE,
        HARDWARE_ORDER_DOWN};

    for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++)
    {
        for (int i = 0; i < 3; i++)
        {
            HardwareOrder type = order_types[i];
            hardware_command_order_light(f, type, 0);
        }
    }
}

void lights_setOrderLights(){
	for (int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++)
        {
            if (hardware_read_order(f, HARDWARE_ORDER_INSIDE))
            {
                hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
            }

            if (hardware_read_order(f, HARDWARE_ORDER_UP))
            {
                hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
            }

            if (hardware_read_order(f, HARDWARE_ORDER_DOWN))
            {
                hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
            }
        }
}

void lights_clearLightsOnFloor(int floor){
	hardware_command_order_light(floor, HARDWARE_ORDER_DOWN, 0);
	hardware_command_order_light(floor, HARDWARE_ORDER_UP, 0);
	hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, 0);
}