#!/usr/bin/python
# cache_affinity.py
#
# This script walks sysfs and determines the level, type, and size of
# each processor cache. It then reports which processor cores each
# cache level is shared with.
#

import glob, os, re, sys

nr_cpus = 0

for line in open("/proc/cpuinfo"):
    if re.search("processor", line):
        nr_cpus += 1

for cpu in range(nr_cpus):
    print "Processor {cpu}".format(cpu=cpu)
    for level in os.listdir("/sys/devices/system/cpu/cpu{cpu}/cache/".format(cpu=cpu)):
        for file in ["level", "type", "size"]:
            name = "/sys/devices/system/cpu/cpu{cpu}/cache/{level}/{file}".format(cpu=cpu, level=level, file=file)
            if os.path.exists(name):
                f = open(name)
                text = f.readline()
                f.close()
                print "{level} {file} is {text}".format(level=level, file=file, text=text.rstrip())

indexes = len(glob.glob("/sys/devices/system/cpu/cpu0/cache/index*"))

for cpu in range(nr_cpus):
    print "Processor {cpu}".format(cpu=cpu)
    for index in range(indexes):
        name = "/sys/devices/system/cpu/cpu{cpu}/cache/index{index}/shared_cpu_map".format(cpu=cpu, index=index)
        if os.path.exists(name):
            f = open(name)
            text = f.readline()
            f.close()
            print "CPU {cpu}, Index {index} = {text}".format(cpu=cpu, index=index, text=text.rstrip())
