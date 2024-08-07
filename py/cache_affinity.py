#!/usr/bin/python
# cache_affinity.py
#
# This script walks sysfs and determines the level, type, and size of
# each processor cache. It then reports which processor cores each
# cache level is shared with.
#

import glob, os, re

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
                with open(name) as f:
                    print " {level} {file} is {text}".format(level=level, file=file, text=f.readline().rstrip())

for cpu in range(nr_cpus):
    print "Processor {cpu}".format(cpu=cpu)
    for index in range(len(glob.glob("/sys/devices/system/cpu/cpu{cpu}/cache/index*".format(cpu=cpu)))):
        name = "/sys/devices/system/cpu/cpu{cpu}/cache/index{index}/shared_cpu_map".format(cpu=cpu, index=index)
        if os.path.exists(name):
            with open(name) as f:
                print " CPU {cpu}, Index {index} = {text}".format(cpu=cpu, index=index, text=f.readline().rstrip())
