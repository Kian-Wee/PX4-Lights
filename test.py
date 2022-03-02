import serial
import time
a="abcd"

# ser = serial.Serial('/dev/ttyUSB0', 9600)
ser = serial.Serial('/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_84:F7:03:A0:FC:F0-if00', 9600)
# ser = serial.Serial('/dev/serial/by-id/usb-Espressif_USB_JTAG_serial_debug_unit_84:F7:03:A1:0F:04-if00', 9600)


while True:
    ser.write(b'S5\n')
    print(b"1")
    time.sleep(1)
    ser.write(b'R10\n')
    print(b"0")
    time.sleep(1)
    # ser.write(a)
    # print(a)
    # time.sleep(1)