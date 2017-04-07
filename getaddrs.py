#!/usr/bin/python

"""
Usage: getaddrs.py [interface name]
"""

# http://code.activestate.com/recipes/439094-get-the-ip-address-associated-with-a-network-inter/

import fcntl
import socket
import struct
import sys

def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915, # SIOCGIFADDR
        struct.pack('256s', ifname[:15])
    )[20:24])

def get_hardware_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    info = fcntl.ioctl(s.fileno(), 0x8927,  struct.pack('256s', ifname[:15]))
    return ':'.join(['%02x' % ord(char) for char in info[18:24]])

try:
    scriptname, myif = sys.argv
except ValueError:
    print __doc__
    sys.exit(1)

print get_hardware_address(myif)
print get_ip_address(myif)
