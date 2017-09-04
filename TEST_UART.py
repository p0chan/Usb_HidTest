from serial import *
from threading import Thread
import time
import numpy as np

flag =0

RecvBuf = np.zeros(128, np.long)
RecvBufWCnt=0
RecvBufRCnt=0


def PushChar(data):
    global RecvBuf
    global RecvBufWCnt
    global flag
    if(RecvBufWCnt>127):
        RecvBufWCnt =0
    RecvBuf[RecvBufWCnt] = ord(data)
    RecvBufWCnt +=1
    if(RecvBuf[RecvBufWCnt]==0x0D):
        flag =1
def PopChar():
    global RecvBuf
    global RecvBufWCnt
    global RecvBufRCnt
    if (RecvBufRCnt > 127):
        RecvBufRCnt = 0
    RetVal = RecvBuf[RecvBufRCnt]
    RecvBufRCnt += 1
    if RecvBufWCnt==RecvBufRCnt :
        RecvBufWCnt =0
        RecvBufRCnt =0
    return RetVal

def receiving(ser):
    print "receiving Thread Run"
    while True:
        aa = ser.inWaiting()
        if aa :
            PushChar(ser.read(1))

def wrinting(ser):
    print "wrinting Thread Run"
    while True:
        tmp = raw_input()
        print "Write : ",tmp
        tmp += '\r\n'
        ser.write(tmp)


ser = Serial('com1',57600,timeout=1)
thR =  Thread(target=receiving, args=(ser,)).start()
thW =  Thread(target=wrinting, args=(ser,)).start()

ss ="recv : "
while True:
    Tmpdata=0
    if RecvBufWCnt != RecvBufRCnt:
        Tmpdata = PopChar()
        ss += unichr(Tmpdata)
        if(Tmpdata == 0x0D):
            print ss
            ss = "recv : "


print "=="
ser.close()