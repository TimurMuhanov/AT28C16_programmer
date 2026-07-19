import serial

ser = serial.Serial('/dev/ttyUSB0',115200,timeout=0)
print(ser.name)
ser.write(b'.W00000')

x=ser.read() # example of reading 1 byte

ser.close()
