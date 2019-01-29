#!/usr/bin/python

# Usage: list_to_make_var.py  <in-file1:mod list>  <in-file2:OFS>  <out-file1:build script>  <out-file2:clean script>  <OS>
# argv:  argv[0]              argv[1]              argv[2]         argv[3]                   argv[4]                   argv[5]

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



mod_base_path = '../../mod/'
curr_os = str(sys.argv[5])
if curr_os == 'WIN':
	mod_build_file = 'build_mod.bat'
	mod_clean_file = 'clean_mod.bat'
else:
	mod_build_file = 'build_mod.sh'
	mod_clean_file = 'clean_mod.sh'

#
# main
#
if len(sys.argv) != 6:
	print("Arguments Number Error. It should be 6.")
	sys.exit(1)


finList      = OpenFile(str(sys.argv[1]))
finOFS       = OpenFile(str(sys.argv[2]))
foutBuildfile = OpenFile(str(sys.argv[3]), 'w')
foutCleanfile = OpenFile(str(sys.argv[4]), 'w')

#
# Extract OFS
#
state = 0
OFS_String = ""
OFS_Exist = 0
for each_line in finOFS:
	each_word_list = each_line.split()
	#print(each_word_list)
	for each_word in each_word_list:
		# Find OFS
		if state == 0:
			if each_word != "OFS":
				print("Error. Miss \"OFS\" symbol in OFS file.")
				sys.exit(1)
			state = 1
		# Find equal sign
		elif state == 1:
			if each_word != "=":
				print("Error. Miss \"=\" symbol in OFS file.")
				sys.exit(1)
			state = 2
		# Make OFS string
		else:
			OFS_Exist = 1
			OFS_String = OFS_String + " " + each_word
if curr_os == 'WIN':
	if OFS_Exist == 1:
		OFS_String = "OFS=\"" + OFS_String + "\""
else:
	if OFS_Exist == 1:
		OFS_String = OFS_String ### "OFS=\"" + OFS_String + "\""

if curr_os == 'WIN':
	foutBuildfile.write('@ECHO OFF\n')
	foutCleanfile.write('@ECHO OFF\n')
	foutBuildfile.write('SET CURR_CD=%CD%\n')
	foutCleanfile.write('SET CURR_CD=%CD%\n')
	for each_line in finList:
		each_mod = each_line.strip()
		# build files
		str = 'CD ' + mod_base_path + each_mod + '\n' + 'CALL ' + mod_build_file + ' ' + OFS_String + '\n'
		foutBuildfile.write(str)
		foutBuildfile.write('set rc=%ERRORLEVEL%\n')
		foutBuildfile.write('CD %CURR_CD%\n')
		foutBuildfile.write('IF %rc% NEQ 0 ( exit /b %rc% )\n\n')
		# clean files
		str = 'CD ' + mod_base_path + each_mod + '\n' + 'CALL ' + mod_clean_file + ' --DisablePause\n'
		foutCleanfile.write(str)
		foutCleanfile.write('CD %CURR_CD%\n')
	foutCleanfile.write('if "%1" NEQ "--DisablePause" (\n')
	foutCleanfile.write('	pause\n')
	foutCleanfile.write(')\n')
else: #LINUX
	foutBuildfile.write('temp_local_path=$PWD\n')
	foutCleanfile.write('temp_local_path=$PWD\n')
	for each_line in finList:
		each_mod = each_line.strip()
		# build files
		str = 'cd ' + mod_base_path + each_mod + '\n' + './' + mod_build_file + ' ' + OFS_String + '\n'
		foutBuildfile.write(str)
		foutBuildfile.write('rc=$?\n')
		foutBuildfile.write('cd $temp_local_path\n')
		foutBuildfile.write('if [ $rc != 0 ]; then\n')
		foutBuildfile.write('	exit $rc\n')
		foutBuildfile.write('fi\n\n')
		# clean files
		str = 'cd ' + mod_base_path + each_mod + '\n' + './' + mod_clean_file + '\n'
		foutCleanfile.write(str)
		foutCleanfile.write('cd $temp_local_path\n')

finList.close()
foutBuildfile.close()
foutCleanfile.close()
