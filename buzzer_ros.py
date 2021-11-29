#!/usr/bin/env python
import rospy
from std_msgs.msg import Int32
import serial
import time

# Serial connection to teensy
#ser = serial.Serial('/dev/ttyUSB0', 9600)
#ser = serial.Serial('COM10', 9600) 
ser = serial.Serial('/dev/serial/by-id/usb-Raspberry_Pi_PicoArduino_DF6050A04B711139-if00', 9600)

## TODO, ADD CHANNEL 5 BYPASS

time.sleep(2) # wait for the serial connection to initialize

# Turns on buzzer when user input is on and turns off buzzer when user input is off
# Currently latches on to the previous input, ie requires input 0 to turn it off
def callback(msg):
    user_input = msg.data
    
    if user_input >0:
        print("Alarm is on")
        ser.write(b'H') 
    elif user_input ==0:
        print("Alarm is off")
        ser.write(b'L')
    elif user_input <0:
        print("Program Exiting")
        ser.write(b'L')
        ser.close()
    else:
        print("Invalid")

if __name__ == '__main__':
    rospy.init_node('alarm') 
    sub = rospy.Subscriber("/hri_user_input", Int32, callback)
    rospy.spin()


#Test
#rostopic pub /hri_user_input std_msgs/Int32 1
