#!/usr/bin/python

import sys

#usage: test.py <inputfile> <outputfile>

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))
print('1st Argument:', str(sys.argv[0]))
print('2nd Argument:', str(sys.argv[1]))
print('3rd Argument:', str(sys.argv[2]))


#
# File IO sample code
#
try:
	if len(sys.argv) != 3:
		raise ValueError
	
	inputFile = str(sys.argv[1])
	fin = open(inputFile)
	fin.close()
except ValueError:
	print("Arguments Number Error.")
	sys.exit(1)
except OSError as err:
	print("OS error: {0}".format(err))
	sys.exit(1)
except:
	print("Unexpected error:", sys.exc_info()[0])
	print("Exit!!");
	sys.exit(1)
