#!/usr/bin/python

"""
Send an Unsolicited Neighbour Advertisement.

Usage: send_na.py [interface name] [IPv6 address to announce]

Requires scapy (available in EPEL). Must be run as root.
"""

# LICENSE - Creative Commons Zero 1.0 Universal
# http://creativecommons.org/publicdomain/zero/1.0/

import fcntl
import logging
import os
import socket
import struct
import sys

# Prevent scapy import logging WARNING for no IPv6 default route
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)

try:
    from scapy.all import Ether, IPv6, ICMPv6ND_NA, ICMPv6NDOptDstLLAddr, sendp
except ImportError:
    print "Requires scapy (EPEL: yum install scapy)."
    sys.exit(1)


def get_hw_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    info = fcntl.ioctl(s.fileno(), 0x8927, struct.pack('256s', ifname[:15]))
    return ':'.join(['%02x' % ord(char) for char in info[18:24]])


try:
    scriptname, myif, myip = sys.argv
except ValueError:
    print __doc__
    sys.exit(1)

if os.geteuid() != 0:
    print "Must be run as root."
    sys.exit(1)


# Get MAC address of interface to use as source MAC
myll = get_hw_address(myif)

# Dest MAC is all-nodes multicast MAC
e = Ether(dst="33:33:00:00:00:01", src=myll, type=0x86dd)

# Dest IP is all-nodes multicast address
ip6 = IPv6(src=myip, dst="ff02::1", nh=58)

# Set Flags and Target Addr as per RFC-4861 Section 4.4
nd_na = ICMPv6ND_NA(R=0, S=0, O=1, tgt=myip)

# Target Link Layer Address option, not required but helpful
ndo_lladdr = ICMPv6NDOptDstLLAddr(lladdr=myll)

# Send constructed frame
sendp (e/ip6/nd_na/ndo_lladdr, iface=myif)
