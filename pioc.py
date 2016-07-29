#!/usr/bin/python
#
# Script to figure out actual Raspberry Pi overclock frequencies
# http://elinux.org/RPiconfig#Overclocking
# You can set avoid_pwm_pll=1 to work around these restrictions

from sys import argv
from os.path import basename
from math import floor

try:
    core_freq = int(argv[1])
    gpu_freq = int(argv[2])
except:
    print "Usage: {script} [core_freq] [gpu_freq]".format(script=basename(argv[0]))
    exit(1)

pll_freq = floor(2400 / (2 * core_freq)) * (2 * core_freq)
gpu_div = floor(pll_freq / gpu_freq)
gpu_freq = pll_freq / gpu_div

print "pll_freq = {pll_freq:.2f} MHz".format(pll_freq=pll_freq)
print "gpu_freq = {gpu_freq:.2f} MHz".format(gpu_freq=gpu_freq)
