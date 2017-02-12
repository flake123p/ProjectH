name_list = []
up_name_list = [] # Uppercase which been used in typedef enum

fin = open('__src_screen.txt')
for each_line in fin:
	# Produce name list
	each_word_list = each_line.split()
	concat_string = ""
	concat_string = concat_string.join(each_word_list) # Scrn 00 Aaa Xxx ==> Scrn00AaaXxx
	name_list.append(concat_string)
	# Produce uppercase list
	upper_list = []
	for each_word in each_word_list:
		upper_list.append(each_word.upper())
	up_concat_string = "_"
	up_concat_string = up_concat_string.join(upper_list) # Scrn 00 Aaa Xxx ==> SCRN_00_AAA_XXX
	up_name_list.append(up_concat_string)
fin.close()

#Create Cpp File
for each_word in name_list:
	fout = open(each_word + '.cpp', 'w')
	fout.write('int ' + each_word + '_OnEntry(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)\n{\n\treturn 0;\n}\n\n')
	fout.write('int ' + each_word + '_OnDraw(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)\n{\n\treturn 0;\n}\n\n')
	fout.write('int ' + each_word + '_OnEvent(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)\n{\n\treturn 0;\n}\n\n')
	fout.write('int ' + each_word + '_OnLeave(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)\n{\n\treturn 0;\n}\n\n')

# Let PaPa see the result.
print('name_list    = ', name_list)
print('up_name_list = ', up_name_list)

#
# Write sets.txt  1. cpp filename list
#
fout_sets = open('sets.txt', 'w')
for each_word in name_list:
	fout_sets.write('{:s}.cpp\n'.format(each_word))
#
# Write sets.txt  2. num
#
fout_sets.write('\n\ntypedef enum {\n')
ctr = 0
for each_word in up_name_list:
	if ctr == 0:
		string = '\t' + each_word + ' = 0,'
	else:
		string = '\t' + each_word + ','
	fout_sets.write('{:30s}'.format(string) + '// {:d}\n'.format(ctr))
	ctr += 1
fout_sets.write('} SCRN_INDEX;\n\n')
#
# Write sets.txt  3. callback array
#
fout_sets.write('SCREEN_CB_LIST_t My_ScreenCallbackList[] = {\n')
ctr = 0
for each_word in name_list:
	fout_sets.write('\t')
	fout_sets.write('{' + '{:24} 0, '.format(up_name_list[ctr] + ','))
	fout_sets.write('{:28s}'.format(each_word + '_OnEntry, '))
	fout_sets.write('{:28s}'.format(each_word + '_OnDraw, '))
	fout_sets.write('{:28s}'.format(each_word + '_OnEvent, '))
	fout_sets.write('{:s}'.format(each_word + '_OnLeave},\n'))
	ctr += 1
fout_sets.write('};\n\n')
#
# Write sets.txt  4. callback function declaration list
#
for each_word in name_list:
	fout_sets.write('int ' + each_word + '_OnEntry(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl);\n')
	fout_sets.write('int ' + each_word + '_OnDraw(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl);\n')
	fout_sets.write('int ' + each_word + '_OnEvent(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl);\n')
	fout_sets.write('int ' + each_word + '_OnLeave(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl);\n')
fout_sets.close()