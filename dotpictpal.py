#!/usr/bin/python

# This script takes a CSV dump of the Android dotpict palette database
# and converts the palettes into GIMP GPL palette files
#
# dotpict can be found at:
# https://play.google.com/store/apps/details?id=net.dotpicko.dotpict
#
# export the database with a sqlite editor such as:
# https://play.google.com/store/apps/details?id=com.ksk.sqliteeditor
#
# the database is /data/data/net.dotpicko.dotpict/databases/dotpict.db
# palettes are stored in the "Pallets" table

# TODO
# support taking an existing GPL palette and adding to existing database

import csv

# http://stackoverflow.com/questions/33124347/convert-integers-to-rgb-values-and-back-with-python
# begin
def int_to_color(RGBint):
    blue =  RGBint & 255
    green = (RGBint >> 8) & 255
    red =   (RGBint >> 16) & 255
    # alpha = (RGBint >> 24) & 255 ## ignored
    return red, green, blue

def color_to_int(rgb):
    red = rgb[0]
    green = rgb[1]
    blue = rgb[2]
    alpha = 255  ## ignored
    print red, green, blue
    RGBint = (alpha << 24) + (red << 16) + (green << 8) + blue
    return RGBint
# end

# http://stackoverflow.com/questions/2918362/writing-string-to-a-file-on-a-new-line-everytime
# begin
class cfile(file):
    #subclass file to have a more convienient use of writeline
    def __init__(self, name, mode = 'r'):
        self = file.__init__(self, name, mode)

    def writel(self, string):
        self.writelines(string + '\n')
        return None
# end

## main

with open("sqlite.Pallets.csv", "rb") as infile:
    with cfile("palettes.csv", "w") as outfile:
        for row in csv.reader(infile):
            outfile.writel('\"' + row[8] + '","' + row[2].replace(',', '","') + '\"')

with open("palettes.csv", "rb") as infile:
    for row in csv.reader(infile):
        with cfile(row[0] + ".gpl", "w") as dest:
            headerdone = False
            for col in row:
                if headerdone == True:
                    colval = int(col) + (2**32)
                    dest.writel("%3d %3d %3d  Untitled" % int_to_color(colval))
                else:
                    dest.writel("GIMP Palette")
                    dest.writel("Name: " + col)
                    dest.writel("Columns: 1")
                    dest.writel("#")
                    headerdone = True
