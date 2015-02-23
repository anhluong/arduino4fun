#!/usr/bin/python2.7

"""
Author: Anh Luong
Date: May 29, 2013
"""

import smbus as smbus
import sys
import socket, threading

HOST = '192.168.0.145'
PORT = 23

if len(sys.argv) == 3:
    HOST = sys.argv[1]
    PORT = int(sys.argv[2])
else:
    print "sudo ./buggy <host> <port>"

bus = smbus.SMBus(1)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
connection, address = s.accept()
print '%s:%s connected.' % address
while True:
    data = connection.recv(1024)
    print data
    data = data.split()
    if len(data) == 3:
        bus.write_word_data(0x48, 1, int(data[1]))
        bus.write_word_data(0x48, 2, int(data[2]))
    elif len(data) == 5:
        #Right side?
        bus.write_word_data(0x48, 1, int(data[1]))
        bus.write_word_data(0x48, 2, int(data[2]))
        #Left side?
        bus.write_word_data(0x48, 3, int(data[3]))
        bus.write_word_data(0x48, 4, int(data[4]))
    else:
        bus.write_word_data(0x48, 1, 0)
        bus.write_word_data(0x48, 2, 0)
    if not data:
        break

connection.close()

