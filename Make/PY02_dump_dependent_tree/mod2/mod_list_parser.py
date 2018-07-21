#!/usr/bin/python

import sys

# Usage: mod_list_parser.py  <input file (mod list)>  <output file 1(mod tree)>  <output file 2(all modules)>
# argv:  argv[0]             argv[1]                  argv[2]                    argv[3]

if len(sys.argv) != 4:
	print("Arguments Number Error. It should be 4.")
	sys.exit(1)

#
# File IO sample code
#
try:
	finModList  = open(str(sys.argv[1]))
	foutModTree = open(str(sys.argv[2]), 'w')
	foutModAll  = open(str(sys.argv[3]), 'w')
except OSError as err:
	print("OS error: {0}".format(err))
	sys.exit(1)
except:
	print("Unexpected error:", sys.exc_info()[0])
	sys.exit(1)

foutModTree.write('var =')
for each_line in finModList:
	foutModTree.write(' ' + each_line.strip())
foutModTree.write('\n')

foutModAll.write('var =')

finModList.close()
foutModTree.close()
foutModAll.close()
