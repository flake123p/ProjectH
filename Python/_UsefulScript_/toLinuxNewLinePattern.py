#!/usr/bin/python

import sys

#usage: toLinuxNewLinePattern.py <inputfile> <outputfile>
#
# Unix/Linux "new line" pattern is 0x0A
# Windows new line pattern is 0x0D 0x0A
#
fin = open(str(sys.argv[1]))
fout = open(str(sys.argv[2]), 'w+b')

for x in fin:
	bAry = bytearray(x.strip(), 'utf-8')
	fout.write(bAry)
	fout.write(b'\x0A')
