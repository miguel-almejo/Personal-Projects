#include "Left_Sensor.h"
#include "tm4c123gh6pm.h"
#include "Timer.h"

void Left_Sensor_Init(){
	
	TRIGGER_LPin &= ~TRIGGER_LEFT;
	Timer_1usDelay(2);
	TRIGGER_LPin |= TRIGGER_LEFT;
	Timer_1usDelay(10);
	TRIGGER_LPin &= ~TRIGGER_LEFT;
	
}