#!/usr/bin/python

# Creates a number of dirs and a number of unique files in each dir
# Usage: testdirs.py [number of dirs] [number of files]

import sys
import os

try:
    # assign the script name (argv[0]) to "scriptname"
    # assign the second argument (argv[1]) to "numdirs"
    # assign the third argument (argv[2]) to "numfiles"
    scriptname, numdirs, numfiles = sys.argv
except ValueError:
    # if the user has not passed 3 arguments, a ValueError exception is raised
    # so we print the usage message and quit
    print "Creates a number of dirs and a number of unique files in each dir"
    print "Usage: {} [number of dirs] [number of files]".format(sys.argv[0])
    sys.exit()

# for each directory number (this starts at 0, we'll add 1 later)
for dirnum in range(int(numdirs)):
    # make a directory name called "testdir-X" starting at 1
    dirname = "testdir-{}".format(dirnum + 1)
    # make that directory
    os.makedirs(dirname)
    # for each file number (this starts at 0, we'll add 1 later)
    for filenum in range(int(numfiles)):
        # create a full filename called "testdir-X/testfile-Y" starting a file 1
        filename = "{}/testfile-{}".format(dirname, filenum + 1)
        with open(filename, "w") as file:
                # write the full filename into the file (which is unique)
                file.write(filename)
                # write urandom into the file (which may not be unique)
                file.write(os.urandom(512 * 5))
