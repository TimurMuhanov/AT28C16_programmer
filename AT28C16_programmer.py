import serial
import time
import sys

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

serialname=""
filename=""
if len(sys.argv) > 2:
    serialname=sys.argv[1]
    filename=sys.argv[2]
else:
    print("need serial and file")
    exit()

dat=[]
with open(filename,"rb") as file:
    dat=file.read()

# for i in dat:
#     print(i)

ser = serial.Serial(serialname,115200,timeout=0.5)
print(ser.name)
time.sleep(2)
# xx = addSum(addData(addAddr(b'.T',1),0))
# print(xx)
# ser.write(xx)
# print("now must be 1 pin set LOW")

run = True
rcvng = False
get_ans = True
counter = 0
print("writing...")
for i in dat:
    xx = addSum(addData(addAddr(b'.W',counter),i))
    ser.write(xx)
    print("sended   ",xx)
    down_counter=1000
    run = True
    while run:
        if (ser.in_waiting >= 8):
            x = ser.read(ser.in_waiting)
            print("received ",x)
            if (x[0] != ord(b'.')):
                print(x[0]," != ",ord(b'.'))
                x = x[1:]
            else:
                if (checkDif(x[:8])):
                    if (x[6]==b'R'):
                        print("error received ",b)
                        ser.close()
                        exit()
                    run = False
        time.sleep(0.01)
        down_counter = down_counter-1
        if (down_counter<=0):
            print("no answer")
            ser.close()
            exit()
    counter = counter + 1

print("checking...")
counter = 0
run = True
for i in dat:
    xx = addSum(addData(addAddr(b'.R',counter),0))
    ser.write(xx)
    print("sended   ",xx)
    down_counter=1000
    run = True
    while run:
        if (ser.in_waiting >= 8):
            x = ser.read(ser.in_waiting)
            print("received ",x)
            if (x[0] != ord(b'.')):
                print(x[0]," != ",ord(b'.'))
                x = x[1:]
            else:
                if (checkDif(x[:8])):
                    if (x[6]==b'R'):
                        print("error received ",b)
                        ser.close()
                        exit()
                    else:
                        a = int(x.decode('ascii')[2:5],16)
                        d = int(x.decode('ascii')[5:7],16)
                        if (a != counter):
                            print("error: was readed another address. readed ",hex(a)," but must be ",hex(counter))
                            ser.close()
                            exit()
                        if (d != i):
                            print("error: was readed another data. readed ",hex(d)," but must be ",hex(i))
                            ser.close()
                            exit()
                    run = False
        time.sleep(0.01)
        down_counter = down_counter-1
        if (down_counter<=0):
            print("no answer")
            ser.close()
            exit()
    counter = counter + 1

print("ok")

ser.close()
