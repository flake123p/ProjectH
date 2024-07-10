#!/usr/bin/python

import sys

# Usage: var_maker.py  <input file>  <output file>
# argv:  argv[0]       argv[1]       argv[2]

#print('Number of arguments:', len(sys.argv), 'arguments.')
#print('Argument List:', str(sys.argv))
#print('1st Argument:', str(sys.argv[0]))
#print('2nd Argument:', str(sys.argv[1]))
#print('3rd Argument:', str(sys.argv[2]))

if len(sys.argv) != 3:
	print("Arguments Number Error. It should be 3.")
	sys.exit(1)

inputFile = str(sys.argv[1])
outputFile = str(sys.argv[2])
#
# File IO sample code
#
try:
	fin = open(inputFile)
	fout = open(outputFile, 'w')
except OSError as err:
	print("OS error: {0}".format(err))
	sys.exit(1)
except:
	print("Unexpected error:", sys.exc_info()[0])
	sys.exit(1)

fout.write('var =')
for each_line in fin:
	fout.write(' ' + each_line.strip())
fout.write('\n')

fin.close()
fout.close()