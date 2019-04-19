#!/usr/bin/python

"""
Displays a Linux systray icon indicating if VPN is up or not.

Assumes VPN creates a network interface called tun0.

I wish I could have used Python 3 but apparently no pystray support for it.

Requirements: pystray, PIL
"""

import os
import pystray
import time
from PIL import Image
from pystray import Menu, MenuItem

vpnpath = "/sys/class/net/tun0/operstate"

# my preferred icons, red/green
imagedn = Image.open("/usr/share/icons/gnome/16x16/status/user-busy.png")
imageup = Image.open("/usr/share/icons/gnome/16x16/status/user-available.png")

# alternate icons, padlock
# imagedn = Image.open("/usr/share/icons/gnome/16x16/status/changes-allow.png")
# imageup = Image.open("/usr/share/icons/gnome/16x16/status/changes-prevent.png")

def loop(icon):
    icon.visible = True
    while icon.visible:
        if os.path.isfile(vpnpath):
            icon.icon = imageup
        else:
            icon.icon = imagedn
        time.sleep(0.1)

def exit_action(icon):
    icon.visible = False
    icon.stop()

def no_action():
    pass

icon = pystray.Icon("VPN Status Icon", imagedn)
icon.menu = Menu(
    MenuItem("VPN Status Icon", no_action, enabled=False),
    MenuItem("Exit", lambda : exit_action(icon), default=True)
)
icon.title = "VPN Status Icon"
icon.run(setup=loop)
