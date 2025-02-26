from machine import Pin, time_pulse_us
import time

red_1 = Pin(21, Pin.OUT) # P21 - Red 1
yellow_1 = Pin(20, Pin.OUT)# P20 - Yellow 1
green_1 = Pin(19, Pin.OUT) # P18 - Green 1
red_2 = Pin(10, Pin.OUT) # P10 - Red 2
yellow_2 = Pin(11, Pin.OUT) # P11 - Yellow 2
green_2 = Pin(12, Pin.OUT) # P12 - Green 2

trig_1 = Pin(17, Pin.OUT) # P17 - Trigger pin 1
echo_1 = Pin(16, Pin.IN) # P16 - Echo pin 1
trig_2 = Pin(14, Pin.OUT) # P14 - Trigger pin 2
echo_2 = Pin(15, Pin.IN) # P15 - Echo pin 2

sound_speed = 0.0135043 # inches per microsecond

# function to get distance from right sensor
def get_distR():
    trig_1.value(0) # set trigger pin low
    time.sleep_us(2) # small delay

    trig_1.value(1) # set trigger pin high
    time.sleep_us(10)
    trig_1.value(0)

    duration = time_pulse_us(echo_1, 1, 30000)
    dist_1 = (sound_speed*duration)/2 # equation to get distance in inches
    return dist_1 # return distance value

def get_distL():
    trig_2.value(0) # set trigger pin low
    time.sleep_us(2) # small delay

    trig_2.value(1) # set trigger pin high
    time.sleep_us(10)
    trig_2.value(0)

    duration = time_pulse_us(echo_2, 1, 30000)
    dist_2 = (sound_speed*duration)/2 # equation to get distance in inches
    return dist_2 # return distance value

# initial state for the traffic light
def traffic_light_init():
    # initial values for LEDs - All Off
    red_1.value(0)
    green_1.value(0)
    yellow_1.value(0)

    red_2.value(0)
    green_2.value(0)
    yellow_2.value(0)

# State 1
def State1():
    # green 1 & red 2 - off
    green_1.value(1)
    red_2.value(1)

# State 2
def State2():
    # green 1 - off; yellow 1 - on
    green_1.value(0)
    yellow_1.value(1)

# State 3
def State3():
    # red 2 & yellow 1 - off; red 1 & green 2 - on
    red_2.value(0)
    yellow_1.value(0)
    red_1.value(1)
    green_2.value(1)

# State 4        
def State4():
    # green 2 - off; yellow 2 - on
    green_2.value(0)
    yellow_2.value(1)

# State 5
def State5():
    # red 1 - off; yellow 2 - off
    red_1.value(0)
    yellow_2.value(0)

# Main Function
def main():
    traffic_light_init() # initialize traffic light
    while True: # infinite while loop
        State1()
        dist_1 = get_distR() 
        print(f"Distance 1: {dist_1:.0f} in") # prints distance from sensor 1
        # if statement to determine the next delay
        if dist_1 < 5.0: 
            time.sleep(1.5)
            State2()
        elif dist_1 > 5.0:
            time.sleep(3)
            State2()
        time.sleep(0.75)
        State3()
        dist_2 = get_distL()
        print(f"Distance 2: {dist_2:.0f} in") # prints distance from sensor 2
        # if statement to determine the next delay
        if dist_2 < 5.0:
            time.sleep(1.5)
            State4()
        elif dist_2 > 5.0:
            time.sleep(3)
            State4()
        time.sleep(0.75)
        State5()

# call main
main()