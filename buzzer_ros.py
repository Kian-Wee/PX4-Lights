#!/usr/bin/env python
import rospy
from std_msgs.msg import String
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
ser = serial.Serial('/dev/serial/by-id/usb-Raspberry_Pi_PicoArduino_DF6050A04B2D4639-if00', 115200)
#ser = serial.Serial('/dev/serial/by-id/usb-Raspberry_Pi_PicoArduino_DF6050A04B534438-if00', 115200) #2
#ser = serial.Serial('/dev/serial/by-id/usb-Raspberry_Pi_PicoArduino_DF6050A04B675D39-if00', 115200)
#ser = serial.Serial('/dev/serial/by-id/usb-Raspberry_Pi_PicoArduino_DF6050A04B1A4E39-if00', 115200) #1
#ser = serial.Serial('/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_84:F7:03:A0:FC:F0-if00', 115200)

time.sleep(2) # wait for the serial connection to initialize

drone_name = os.getenv('DRONE_NAME') #environment variable set in bashrc(or exported in terminal)


batterymin=3.2
batterymax=4.2

# Turns on buzzer when user input is on and turns off buzzer when user input is off
# Currently latches on to the previous input, ie requires input 0 to turn it off
class processor():

    def __init__(self):
        rospy.Subscriber(drone_name + "/hri_mode", String, self.modecallback)
        rospy.Subscriber(drone_name + "/mavros/state", State, self.statecallback)
        rospy.Subscriber(drone_name + "/mavros/battery", BatteryState, self.batterycallback)
        rospy.Subscriber(drone_name + "/planner_staus",  String, self.plannerstatecallback)

        self.previnput=''
        self.usermode=''
        self.armed=None
        self.flightmode=None
        self.battery=0
        self.plannerstate=''

        rate = rospy.Rate(2)

        rospy.on_shutdown(self.quit)

        while not rospy.is_shutdown():

            self.sentstring = '' #Combines all output to reduce serial load

            if self.armed==False:
                self.sentstring+='D'
            elif self.usermode =="Distract":
                # print("Alarm is on")
                self.sentstring+='H'
            elif self.usermode =="Sweep":
                # print("Scout mode")
                self.sentstring+='R'
            elif self.flightmode=='OFFBOARD':
                # print("Offboard mode")
                self.sentstring+='O'
            elif self.flightmode=='STABILIZED':
                # print("Stabalized mode")
                self.sentstring+='S'
            else:
                #print("Disarming")
                self.sentstring+='D'
                
            if self.plannerstate =="Formation":
                self.sentstring+='F'
            elif self.plannerstate =="S&A":
                self.sentstring+='A'
            elif self.plannerstate =="Moving":
                self.sentstring+='M'
            else: # Idle
                self.sentstring+='I'

            batterylevel= (self.battery-batterymin)/(batterymax-batterymin)
            batterylevel= round(batterylevel*10)
            self.sentstring+=str(int(batterylevel))

            if self.sentstring != self.previnput:
                print("Sending to ESP32",self.sentstring)
                ser.write(self.sentstring)

            # #for testing
            # print("Sending to Microcontroller",self.sentstring)
            # ser.write(self.sentstring)

            self.previnput=self.sentstring

            rate.sleep()

    def quit(self):
        # print('BUZZER IS SHUTTING DOWN')
        ser.write('D0')
        ser.close()
        # rospy.signal_shutdown("BUZZER SHUTTING DOWN")

    def modecallback(self,msg):
        self.usermode = msg.data

    def statecallback(self,msg):
        self.flightmode = msg.mode
        self.armed = msg.armed

    def batterycallback(self,msg):
        batterytemp=msg.cell_voltage
        if batterytemp == []:
            self.battery = 0
        else:
            self.battery=sum(batterytemp)/len(batterytemp)

    def plannerstatecallback(self,msg):
        self.plannerstate=msg.data

if __name__ == '__main__':
    rospy.init_node(drone_name + '_lights') 

    while True:
        processor()
        rospy.spin()



#Test Publish
#rostopic pub -r 2 /hri_user_input std_msgs/Int32 1
