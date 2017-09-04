# -*- coding: utf-8 -*-
"""
import win32com.client
wmi = win32com.client.GetObject ("winmgmts:")
for usb in wmi.InstancesOf ("Win32_USBHub"):
    print usb.DeviceID
"""

import codecs
#hid.core.show_hids(output = codecs.getwriter('mbcs')(sys.stdout))
##################################################################

import sys
import pywinusb.hid as hid

import time


"""
print "--sys.version--"
print sys.version
print "\n--sys.version_info--"
print sys.version_info
print "\n--sys.hexversion--"
print sys.hexversion
print "---"
"""

"""
Vendor ID:              0x03eb
Product ID:             0x2fff
Version number:         0x1000
"""







""""""
def read_handler(data):
	print data[:10]
	str1 = ''.join(str(hex(e)) for e in data)

#output = sys.stdout
#hid.core.show_hids(output = output)

sys.stdout = codecs.getwriter('mbcs')(sys.stdout)

vendor_num = 0x03eb
product_num =
filter = hid.HidDeviceFilter(vendor_i0x2fffd = vendor_num, product_id = product_num)

# usb 확인
devices = filter.get_devices()
device = devices[0]
device.open()
print device
print "="*10


target_vendor_id = vendor_num
target_usage = hid.get_full_usage_id(0xffff, 0x00)
print "target_usage=",target_usage


# 읽기 핸들러 연결
device.set_raw_data_handler(read_handler)

# 쓰기 포트 찾기
report = device.find_output_reports()

report = report[0]

buffer = [0xFF] * 65
buffer[0] = 0x00  # report ID

buffer[1] = 35
buffer[2] = 86
buffer[3] = 68
buffer[4] = 82
buffer[5] = 63
buffer[6] = 13

# 데이터 전송 설정
report.set_raw_data(buffer)
device.send_output_report(buffer)
report.send()

# 전송!
while True:
	tmp = raw_input()
	report.set_raw_data(buffer)
	device.send_output_report(buffer)
	report.send()
# 잠깐 대기(1초)
	time.sleep(2)
	print "="

# usb 닫기
device.close()
