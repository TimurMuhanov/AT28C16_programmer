import serial
import time

def addSum(x):
    s=0
    for i in x:
        s = s + i
    return x + bytearray(str(hex(s))[-1].upper(),'ascii')

def checkDif(x):
    s=0
    for i in x[:-1]:
        s = s - i
    y = ord(str(hex(s%16))[-1].upper())
    return y == x[-1]

def addAddr(x,a):
    y = str(hex(a))
    y = y[:2]+"000"+y[2:]
    return x + bytearray(y[-3:].upper(),'ascii')

def addData(x,d):
    y = str(hex(d))
    y = y[:2]+"000"+y[2:]
    return x + bytearray(y[-2:].upper(),'ascii')

# x = b'.W'
# print(addSum(addData(addAddr(b'.T',43),0)))
# y = b'.WAAAFF0'
# z = b'.WAAAFFC'
# print(checkDif(y))
# print(checkDif(z))
# exit()

ser = serial.Serial('/dev/ttyUSB0',115200,timeout=0.5)
print(ser.name)
time.sleep(2)
xx = addSum(addData(addAddr(b'.T',1),0))
print(xx)
ser.write(xx)
print("now must be 1 pin set LOW")

run = True
rcvng = False
get_ans = True
counter = 1
while run:
    if (ser.in_waiting > 0):
        x = ser.read() # example of reading 1 byte
        if (rcvng):
            b = b + x
            if (len(b) >= 8):
                if (checkDif(b)):
                    print("OK", str(b))
                    # input("input any for next step")
                    ser.write(addSum(addData(addAddr(b'.T',counter),1)))
                    # time.sleep(0.5)
                    time.sleep(0.5) # for auto mode
                    x = ser.read(8)
                    print(x)
                    rcvng = False
                    counter = counter + 1
                    if (counter > 48):
                        run = False
                    else:
                        ser.write(addSum(addData(addAddr(b'.T',counter),0)))
                        print("now must be ",counter," pin set LOW")
                else:
                    print("Error: ",str(b))
                    run = False
        else:
            if (x == b'.'):
                rcvng = True
                b = x

ser.close()
