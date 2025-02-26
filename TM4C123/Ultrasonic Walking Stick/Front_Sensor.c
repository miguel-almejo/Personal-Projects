#include "Front_Sensor.h"
#include "tm4c123gh6pm.h"
#include "Timer.h"

void Front_Sensor_Init(void){
		TRIGGER_FPin &= ~TRIGGER_FRONT; // send low to trigger
		Timer_1usDelay(2);
		TRIGGER_FPin |= TRIGGER_FRONT; // send high to trigger
		Timer_1usDelay(10);
		TRIGGER_FPin &= ~TRIGGER_FRONT; // send low to trigger
}

