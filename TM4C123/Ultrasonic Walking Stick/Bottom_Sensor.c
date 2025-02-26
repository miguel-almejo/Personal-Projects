#include "Bottom_Sensor.h"
#include "tm4c123gh6pm.h"
#include "Timer.h"

void Bottom_Sensor_Init(void){
		TRIGGER_BPin &= ~TRIGGER_BOTTOM; // send low to trigger
		Timer_1usDelay(2);
		TRIGGER_BPin |= TRIGGER_BOTTOM; // send high to trigger
		Timer_1usDelay(10);
		TRIGGER_BPin &= ~TRIGGER_BOTTOM; // send low to trigger
}