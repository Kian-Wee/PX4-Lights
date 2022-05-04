#!/usr/bin/env python
import rospy
from std_msgs.msg import Int32
import serial
import time
from mavros_msgs.msg import State
from sensor_msgs.msg import BatteryState
import os

# Serial connection
# Run ls /dev/serial/by-id/* to check id in linux or check the com port in the arduino ide
#ser = serial.Serial('/dev/ttyUSB0', 9600)
#ser = serial.Serial('COM10', 9600) 
#ser = serial.Serial('/dev/serial/by-id/usb-Raspberry_Pi_PicoArduino_DF6050A04B711139-if00', 115200)
ser = serial.Serial('/dev/serial/by-id/usb-Raspberry_Pi_PicoArduino_DF6050A04B675D39-if00', 115200)
# ser = serial.Serial('/dev/serial/by-id/usb-Raspberry_Pi_PicoArduino_DF6050A04B1A4E39-if00', 115200) #Borealis 1
#ser = serial.Serial('/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_84:F7:03:A0:FC:F0-if00', 115200)

## TODO, ADD CHANNEL 5 BYPASS

time.sleep(2) # wait for the serial connection to initialize

drone_name = os.getenv('DRONE_NAME') #environment variable set in bashrc(or exported in terminal)
previnput=''
HRI=0
armed=None
flightmode=None
battery=0

batterymin=3.2
batterymax=4.2

# Turns on buzzer when user input is on and turns off buzzer when user input is off
# Currently latches on to the previous input, ie requires input 0 to turn it off
def processor():

    global previnput, batterymin, batterymax, flightmode, battery, HRI

    rate = rospy.Rate(2)

    while not rospy.is_shutdown():
        
        user_input=HRI

        sendstring = '' #COMBINE BATTERY WITH FLIGHT MODE to reduce serial load

        if armed==False:
            sendstring+='D'
        elif user_input ==1: #NOTIFY HRI
            print("Alarm is on")
            sendstring+='H'
        elif user_input ==2:
            print("Scout mode")
            sendstring+='R'
        # elif user_input <0:
        #     print("Program Exiting")
        #     sendstring+='L'
        #     ser.close()
        elif flightmode=='OFFBOARD':
            print("Offboard mode")
            sendstring+='O'
        elif flightmode=='STABILIZED':
            print("Stabalized mode")
            sendstring+='S'
        elif user_input ==0:
            #print("Alarm is off")
            sendstring+='L'
        else:
	        #print("Disarming")
            sendstring+='D'

        batterylevel= (battery-batterymin)/(batterymax-batterymin)
        batterylevel= round(batterylevel*10)
        sendstring+=str(int(batterylevel))

        if sendstring != previnput:
            print("Sending to ESP32",sendstring)
            ser.write(sendstring)

        # #for testing
        # print("Sending to Microcontroller",sendstring)
        # ser.write(sendstring)

        previnput=sendstring

        # time.sleep(1)
        rate.sleep()

def callback(msg):
    global HRI
    HRI = msg.data

def statecallback(msg):
    global flightmode, armed
    flightmode = msg.mode
    armed = msg.armed

def batterycallback(msg):
    global battery
    batterytemp=msg.cell_voltage
    battery=sum(batterytemp)/len(batterytemp)

if __name__ == '__main__':
    rospy.init_node(drone_name + '_alarm') 
    hrisub = rospy.Subscriber(drone_name + "/hri_user_input", Int32, callback)
    # statesub = rospy.Subscriber("uav0/mavros/state", State, statecallback)
    # batterysub = rospy.Subscriber("uav0/mavros/battery", BatteryState, batterycallback)

    while True:
        processor()
        rospy.spin()



#Test Publish
#rostopic pub -r 2 /hri_user_input std_msgs/Int32 1
