#!/usr/bin/python
# errno 
# prints the name or number of an error code

import re
from sys import argv

def print_usage_and_exit():
    print "Usage: {script} [errno number such as 1 or errno symbol such as EPERM]".format(script=argv[0])
    exit(1)

first = ""

try:
    first = argv[1]
except:
    print_usage_and_exit()

if re.search("^[0-9a-zA-Z].*", first):
    for errno_file in ["/usr/include/asm-generic/errno-base.h", "/usr/include/asm-generic/errno.h"]:
        for line in open(errno_file):
            if re.search("^[0-9].*", first):
                searchstring = "\s"+first+"\s"
            else:
                searchstring = first
            if re.search(searchstring, line):
                print line.rstrip()
else:
    print_usage_and_exit()
