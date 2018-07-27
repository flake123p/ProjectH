#!/usr/bin/python

# Usage: list_to_make_var.py  <input file (mod list)>  <output file 1(build script)>  <output file 2(clean script)>  <OS>
# argv:  argv[0]              argv[1]                  argv[2]                        argv[3]                        argv[4]

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



mod_base_path = '../'
curr_os = str(sys.argv[4])
if curr_os == 'WIN':
	mod_build_file = 'build_mod.bat'
	mod_clean_file = 'clean_mod.bat'
else:
	mod_build_file = 'build_mod.sh'
	mod_clean_file = 'clean_mod.sh'

#
# main
#
if len(sys.argv) != 5:
	print("Arguments Number Error. It should be 5.")
	sys.exit(1)


finList      = OpenFile(str(sys.argv[1]))
foutBuildfile = OpenFile(str(sys.argv[2]), 'w')
foutCleanfile = OpenFile(str(sys.argv[3]), 'w')


for each_line in finList:
	each_mod = each_line.strip()
	# build files
	str = mod_base_path + each_mod + '/' + mod_build_file + '\n'
	foutBuildfile.write(str)
	# clean files
	str = mod_base_path + each_mod + '/' + mod_clean_file + '\n'
	foutCleanfile.write(str)

finList.close()
foutBuildfile.close()
foutCleanfile.close()