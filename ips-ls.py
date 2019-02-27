#!/usr/bin/python
## lists contents of an IPS patch
## stole the processing from meunierd's python-ips
## License: MIT - https://opensource.org/licenses/MIT

from os.path import getsize,isfile
import struct
from sys import argv

def print_usage_and_exit():
    print "Usage: {script} [IPS patch file]".format(script=argv[0])
    exit(1)

def unpack_int(string):
    """Read an n-byte big-endian integer from a byte string."""
    (ret,) = struct.unpack_from('>I', b'\x00' * (4 - len(string)) + string)
    return ret

try:
    patchpath = argv[1] and if (isfile(patchpath) == True):
        patch_size = getsize(patchpath)
except:
    print_usage_and_exit()

with open(patchpath, 'rb') as patchfile:
    if patchfile.read(5) != b'PATCH':
        raise Exception('Invalid patch header.')
    # Read First Record
    r = patchfile.read(3)
    while patchfile.tell() not in [patch_size, patch_size - 3]:
        # Unpack 3-byte pointers.
        offset = unpack_int(r)
        # Read size of data chunk
        r = patchfile.read(2)
        size = unpack_int(r)

        if size == 0:  # RLE Record
            r = patchfile.read(2)
            rle_size = unpack_int(r)
            data = patchfile.read(1) * rle_size
        else:
            data = patchfile.read(size)

        if offset >= 0:
            # Write to file
            print "Offset: {offset}".format(offset=hex(offset))
            print "Data  : {data}".format(data=repr(data))
        # Read Next Record
        r = patchfile.read(3)
