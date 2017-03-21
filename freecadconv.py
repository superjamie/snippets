#!/usr/bin/freecadcmd
# Uses freecadcmd to convert from one supported format to another

import os
import sys

import FreeCAD
import Part
import Mesh

try:
    infile, outfile = sys.argv[2], sys.argv[3]
except ValueError:
    print "Uses freecadcmd to convert from one supported format to another"
    print "Usage: {} [source file] [dest file]".format(sys.argv[0])
    sys.exit()

def touch(fname, times=None):
    with open(fname, 'a'):
        os.utime(fname, times)

touch(outfile)

Part.open(infile)

o = [ FreeCAD.getDocument("Unnamed").findObjects()[0] ]

Mesh.export(o, outfile)
