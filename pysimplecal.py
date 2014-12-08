#!/usr/bin/env python

"""
pysimplecal.py

a python port of gsimplecal by dmedvinsky
https://github.com/dmedvinsky/gsimplecal

tested with python 2.7, requires pygtk and psutil

bind this script to a click on the clock in tint2

if your panel height is not 30 pixels, modify the window.move accordingly
"""

import gtk
from os import getpid
from sys import argv, exit
import time

import psutil  ## pip install psutil

class Calendar:
    def __init__(self):
        window = gtk.Window()
        window.set_border_width(0)
        window.set_skip_taskbar_hint(True)

        hbox = gtk.HBox(False, 0)

        self.calendar = gtk.Calendar()
        self.calendar.set_property("show-heading", True)
        self.calendar.set_property("show-day-names", True)
        self.day = int(time.strftime("%d"))
        self.calendar.select_day(self.day)
        self.calendar.mark_day(self.day)

        window.connect("destroy", lambda w: gtk.main_quit())

        window.add(hbox)
        hbox.pack_start(self.calendar, False, False, 0)

        window.show_all()
        window.set_gravity(gtk.gdk.GRAVITY_SOUTH_EAST)
        width, height = window.get_size()
        window.move(gtk.gdk.screen_width() - width, gtk.gdk.screen_height() - height - 30)

myname = argv[0]
mypid = getpid()
for process in psutil.process_iter():
    if process.pid != mypid:
        for path in process.cmdline():
            if myname in path:
                print "process found"
                process.terminate()
                exit()

Calendar()
gtk.main()
