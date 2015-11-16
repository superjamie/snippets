#!/usr/bin/python

"""
mp3time.py

given a date in the format of yyyy-mm-dd, changes the atime and mtime
of every mp3 file and AlbumArt.jpg to that date, increasing timestamp
in one second increments past midnight
"""

import datetime
import glob
import os
import re
import sys
import time

def print_usage_and_exit():
    print "Usage: {0} yyyy-mm-dd".format(sys.argv[0])
    sys.exit(1)

if len(sys.argv) < 2:
    print_usage_and_exit()

try:
    date = datetime.datetime.strptime(sys.argv[1], "%Y-%m-%d")
except ValueError:
    print_usage_and_exit()

files = glob.glob("*.mp3")
files.sort()

if os.path.isfile("AlbumArt.jpg"):
    files = ["AlbumArt.jpg"] + files

for file in files:
    filetime = time.mktime(date.timetuple())
    print "setting {0}'s date to {1}".format(file, filetime)
    os.utime(file, (filetime, filetime))
    date += datetime.timedelta(seconds=1)
