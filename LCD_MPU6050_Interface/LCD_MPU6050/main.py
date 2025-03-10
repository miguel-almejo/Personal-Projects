from machine import Pin, I2C
from time import sleep
from pico_i2c_lcd import I2cLcd
import MPU6050
# global values for I2C 
i2c_lcd = I2C(1, sda=Pin(26), scl=Pin(27), freq=400000)
i2c_mpu = I2C(1, sda=Pin(26), scl=Pin(27), freq=400000)
I2C_ADDR = i2c_lcd.scan()[0]

lcd = I2cLcd(i2c_lcd, I2C_ADDR, 2, 16)
mpu = MPU6050.MPU6050(i2c_mpu)

# wakes up the MPU6050
mpu.wake()

while True:
    lcd.blink_cursor_on() # turns cursor on and blinks

    gyro = mpu.read_gyro_data() # gets data from the gyroscope
    accl = mpu.read_accel_data() # gets data from accelerometer

# uses the data gather above and prints it on the LCD Display
    lcd.putstr("G:"+str(f"{gyro[0]:.0f}")+" "+str(f"{gyro[1]:.0f}")+" "+str(f"{gyro[2]:.0f}")+"\n")
    lcd.putstr("A:"+str(f"{accl[0]:.0f}")+" "+str(f"{accl[1]:.0f}")+" "+str(f"{accl[2]:.0f}")+"\n")

# delay of 0.5 seconds
    sleep(0.5)

# turns cursor off and clears the display
    lcd.blink_cursor_off()
    lcd.clear()