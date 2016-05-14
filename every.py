#!/usr/bin/python

# every.py
# script to generate every 2-color image of a certain dimension

# if you want these in another format, then afterwards:
# for file in *.ppm; do convert "$file" "${file%.ppm}.png"; rm "$file"; done

# dimension is the image dimensions
# for example, dimension = 5 will make every 5x5 image
dimension = 3

number_of_images = ((2**dimension)**dimension) - 1
namepadlen = len(str(number_of_images))
pixpadlen = len(str(bin(number_of_images)[2:]))

for imagenum in range(number_of_images+1):
    filename = "img" + str(imagenum).zfill(namepadlen) + ".ppm"
    file = open(filename, "w")
    file.write("P3 {0} {0} 1".format(dimension))

    pixels = bin(imagenum)[2:].zfill(pixpadlen)
    for pixel in range(len(pixels)):
        if ((pixel % dimension) == 0):
            file.write("\n")
        thisrow = " {0} {0} {0} ".format(pixels[pixel])
        file.write(thisrow)
    file.write("\n")
    file.close()
