#!/usr/bin/python

# Usage: list_to_make_var.py  <input file (mod list)>  <output file - makefile>  <var name>
# argv:  argv[0]              argv[1]                  argv[2]                   argv[3]

#
# Include library
#
import os
import sys

def OpenFile(fileName, mode = 'r'): # mode : 'r', 'w', ...
	try:
		fp = open(fileName, mode)
	except OSError as err:
		print("OS error: {0}".format(err))
		sys.exit(1)
	except:
		print("Unexpected error:", sys.exc_info()[0])
		sys.exit(1)
	return fp

#
# main
#
if len(sys.argv) != 4:
	print("Arguments Number Error. It should be 4.")
	sys.exit(1)

varName = str(sys.argv[3])
finList      = OpenFile(str(sys.argv[1]))
foutMakefile = OpenFile(str(sys.argv[2]), 'w')
foutMakefile.write(varName + ' = ')
for each_line in finList:
	each_mod = each_line.strip()
	foutMakefile.write(' ' + each_mod)
foutMakefile.write('\n')

finList.close()
foutMakefile.close()