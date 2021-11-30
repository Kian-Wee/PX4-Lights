#!/usr/bin/env python
import rospy
from std_msgs.msg import Int32
import serial
import time
from mavros_msgs.msg import State
from sensor_msgs.msg import BatteryState

# Serial connection to teensy
#ser = serial.Serial('/dev/ttyUSB0', 9600)
#ser = serial.Serial('COM10', 9600) 
ser = serial.Serial('/dev/serial/by-id/usb-Raspberry_Pi_PicoArduino_DF6050A04B711139-if00', 115200)

## TODO, ADD CHANNEL 5 BYPASS

time.sleep(2) # wait for the serial connection to initialize

previnput=''
HRI=0
armed=None
flightmode=None
battery=None

batterymin=3.2
batterymax=4.2

# Turns on buzzer when user input is on and turns off buzzer when user input is off
# Currently latches on to the previous input, ie requires input 0 to turn it off
def processor():

    global previnput, batterymin, batterymax
    user_input=HRI

    sendstring = '' #COMBINE BATTERY WITH FLIGHT MODE to reduce serial load

    if armed==False:
        sendstring+='D'
    elif user_input >0:
        print("Alarm is on")
        sendstring+='H'
    elif user_input ==0:
        print("Alarm is off")
        sendstring+='L'
    elif user_input <0:
        print("Program Exiting")
        sendstring+='L'
        ser.close()
    elif flightmode=='OFFBOARD':
        sendstring+='O'
    elif flightmode=='STABILIZED':
        sendstring+='S'

    batterylevel= (battery-batterymin)/(batterymax-batterymin)
    batterylevel= round(batterylevel/10)*10
    sendstring+=batterylevel

    # sendstring+="/n"

    if user_input != previnput:
        ser.write(sendstring)

    previnput=sendstring



def callback(msg):
    global HRI
    HRI = msg.data

def statecallback(msg):
    global flightmode, armed
    flightmode = msg.mode
    armed = msg.armed

def batterycallback(msg):
    global battery
    batterytemp=msg.cellvoltage
    battery=sum(batterytemp)/len(batterytemp)

if __name__ == '__main__':
    rospy.init_node('alarm') 
    hrisub = rospy.Subscriber("/hri_user_input", Int32, callback)
    statesub = rospy.Subscriber("uav0/mavros/state", State, statecallback)
    batterysub = rospy.Subscriber("uav0/mavros/battery", BatteryState, batterycallback)
    rospy.spin()


#Test Publish
#rostopic pub /hri_user_input std_msgs/Int32 1
