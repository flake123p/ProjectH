#!/usr/bin/python

import sys

try:
	fin = open('bad_file')
	for line in fin:
		print(line)
	fin.close()
except OSError as err:
	print("OS error: {0}".format(err))
except:
	print("Unexpected error:", sys.exc_info()[0])
	print("Exit!!");
	sys.exit(1)

try:
	raise ValueError('abxx')
except ValueError:
	print("Could not convert data to an integer.")
	raise #re-raise exception