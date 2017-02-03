def CreateFile(fileName, funcName, index):
	fout = open(fileName, 'w')
	fout.write('\nint Scrn'+ '{:02d}'.format(index) + funcName + '_OnEntry(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)\n{\n\treturn 0;\n}\n')
	fout.write('\nint Scrn'+ '{:02d}'.format(index) + funcName + '_OnDraw(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)\n{\n\treturn 0;\n}\n')
	fout.write('\nint Scrn'+ '{:02d}'.format(index) + funcName + '_OnEvent(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)\n{\n\treturn 0;\n}\n')
	fout.write('\nint Scrn'+ '{:02d}'.format(index) + funcName + '_OnLeave(IN  u32 ScrnEvtId, IN  void *ScrnEvtHdl)\n{\n\treturn 0;\n}\n')
			
fin = open('in.txt')
ctr = 0
for x in fin:
	y = x.split()
	z = ""
	z = z.join(y)
	outFileName = 'Scrn' + '{:02d}'.format(ctr) + z + '.c'
	CreateFile(outFileName, z, ctr)
	ctr += 1

ctr = 0
fin.close()
fin = open('in.txt')
fout_sets = open('sets.txt', 'w')
fout_sets.write('typedef enum {\n')
for x in fin:
	y = x.split()
	z = "_"
	z = z.join(y) + ','
	if ctr == 0:
		z = z.join(y) + ' = 0,'
		fout_sets.write('\tSCRN_' + '{:02d}'.format(ctr) + '_' + '{:15s}'.format(z) + '// {:d}\n'.format(ctr))
	else:
		z = z.join(y) + ','
		fout_sets.write('\tSCRN_' + '{:02d}'.format(ctr) + '_' + '{:15s}'.format(z) + '// {:d}\n'.format(ctr))
	ctr += 1
fout_sets.write('} SCRN_INDEX;\n\n')

ctr = 0
fin.close()
fin = open('in.txt')
fout_sets = open('sets.txt', 'a')
fout_sets.write('SCREEN_CB_LIST_t My_ScreenCallbackList[] = {\n')
for x in fin:
	y = x.split()
	z = "_"
	z = z.join(y) + ','
	funcName = ""
	funcName = funcName.join(y)
	fout_sets.write('\t{SCRN_' + '{:02d}'.format(ctr) + '_' + '{:s}, 0, '.format(z))
	fout_sets.write('Scrn'+ '{:02d}'.format(ctr) + funcName + '_OnEntry, ')
	fout_sets.write('Scrn'+ '{:02d}'.format(ctr) + funcName + '_OnDraw, ')
	fout_sets.write('Scrn'+ '{:02d}'.format(ctr) + funcName + '_OnEvent, ')
	fout_sets.write('Scrn'+ '{:02d}'.format(ctr) + funcName + '_OnLeave},\n')
	ctr += 1
fout_sets.write('};\n')