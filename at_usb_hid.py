import ctypes as c
import time
mydll = c.WinDLL('AtUsbHid')
myfunc = mydll['findHidDevice']
myfunc.argtypes = (c.c_long, c.c_long,)
myfunc.restype = c.c_bool
res = myfunc(0x03eb,0x2fff)
print res
Fwrite = mydll['writeData']
Fwrite.argtypes = (c.c_char_p,)
Fwrite.restype = c.c_bool
#res = Fwrite(c.c_char_p("#VER?"))
res = Fwrite(c.c_char_p("#ISPR?8000\r"))
print res
time.sleep(1)
p = c.c_char_p("")
print p, type(p)
FRead = mydll['readData']
FRead.argtypes = (c.c_char_p,)
FRead.restype = c.c_bool
res = FRead(p)
print res,p
myfunc2 = mydll['closeDevice']
myfunc2()
