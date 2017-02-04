

def UpperAndUnderlineString(string):
	mystr = '';
	for i in string:
		if i == " ":
			mystr = mystr + "_"
		elif i != "\n":
			mystr = mystr + i
	return mystr.upper()


def FillSpaceInString(string, lengthToFill):
	fillLen = lengthToFill - len(string)
	for i in range(0, fillLen):
		string += ' '
	return string

	
def LineToConcatString(string):
	result = ''
	for i in string:
		if i != " " and i != "\n":
			result = result + i
	return result










def EnumCreator():
	in_file.seek(0)
#Enum Head ------------------------------------------------------
	enum_file.write("typedef enum {\n")
#Enum Body ------------------------------------------------------
	loopctr = 0
	for each_line in in_file:
		mystr = '  ';
		for i in each_line:
			if i == " ":
				mystr = mystr + "_"
			elif i != "\n":
				mystr = mystr + i
		
		if loopctr == 0:
			mystr += " = 0,"
		else:
			mystr += ","

		mystr = FillSpaceInString(mystr, 35)
		mystr = mystr + '// ' + str(loopctr) + '\n'
		
		enum_file.write(mystr.upper())
		loopctr += 1
#Enum Tail ------------------------------------------------------
	mystr = FillSpaceInString("  SCRN_MAX_INDEX,", 35)
	mystr = mystr + '// ' + str(loopctr) + '\n'
	enum_file.write("\n" + mystr)
	enum_file.write("}SCRN_INDEX;\n")
	print("EnumCreator() ... OK")



def Doc_C_Creator():
	in_file.seek(0)
	maxlen = 0
	for each_line in in_file:
		mystr = LineToConcatString(each_line)
		doc_c_file.write("  " + mystr + ".c\n")
		#Max Length
		strlen = len(mystr)
		if strlen > maxlen:
			maxlen = strlen
	print("Doc_C_Creator() ... OK")
	return maxlen



def CbHeaderCreator():
#Head ------------------------------------------------------
	cb_header_file.write("#ifndef _SCRN_CALLBACKS_H_\n")
	cb_header_file.write("#define _SCRN_CALLBACKS_H_\n\n")
#Body ------------------------------------------------------
	in_file.seek(0)
	for each_line in in_file:
		mystr = LineToConcatString(each_line)
		cb_header_file.write("SCRN_STATUS \n" + mystr + "OnEntry ( \n")
		cb_header_file.write("  IN  SCRN_EVENT  ScrnEvtId,\n")
		cb_header_file.write("  IN  UINTN       ScrnEvtHdl\n  );\n\n")
		cb_header_file.write("SCRN_STATUS \n" + mystr + "OnDraw ( \n")
		cb_header_file.write("  IN  SCRN_EVENT  ScrnEvtId,\n")
		cb_header_file.write("  IN  UINTN       ScrnEvtHdl\n  );\n\n")
		cb_header_file.write("SCRN_STATUS \n" + mystr + "OnEvent ( \n")
		cb_header_file.write("  IN  SCRN_EVENT  ScrnEvtId,\n")
		cb_header_file.write("  IN  UINTN       ScrnEvtHdl\n  );\n\n")
		cb_header_file.write("SCRN_STATUS \n" + mystr + "OnLeave ( \n")
		cb_header_file.write("  IN  SCRN_EVENT  ScrnEvtId,\n")
		cb_header_file.write("  IN  UINTN       ScrnEvtHdl\n  );\n\n")
#Tail ------------------------------------------------------
	cb_header_file.write("#endif\n")
	print("CbHeaderCreator() ... OK")



def CbArrayCreator(maxlen):
#Head ------------------------------------------------------
	cb_array_file.write("SCREEN_CB_LIST ScreenCallbackList[] = {\n")
#Body ------------------------------------------------------
	in_file.seek(0)
	for each_line in in_file:
		upperStr  = UpperAndUnderlineString(each_line) + ","
		upperStr  = FillSpaceInString(upperStr, maxlen+7)
		concatStr = LineToConcatString(each_line)
		cb_array_file.write("  {" + upperStr)
		cb_array_file.write(" " + group_name + ",")
		cb_array_file.write(" " + FillSpaceInString(concatStr + "OnEntry,", maxlen+7))
		cb_array_file.write(" " + FillSpaceInString(concatStr + "OnDraw,", maxlen+7))
		cb_array_file.write(" " + FillSpaceInString(concatStr + "OnEvent,", maxlen+7))
		cb_array_file.write(" " + concatStr + "OnLeave},\n")
#Tail ------------------------------------------------------
	cb_array_file.write("};\n")
	print("CbArrayCreator() ... OK")



def CbBodyCreator():
	in_file.seek(0)
	for each_line in in_file:
		concatStr = LineToConcatString(each_line)
		fileStr = concatStr + ".c"
		try:
			body_file = open(fileStr, "w")
#Head ------------------------------------------------------
			body_file.write("\n#include \"E00ExerciseCommonLibs.h\"\n")
			body_file.write("#include \"E00ExerciseHeader.h\"\n\n\n")
#Body ------------------------------------------------------
			body_file.write("SCRN_STATUS \n" + concatStr + "OnEntry ( \n")
			body_file.write("  IN  SCRN_EVENT  ScrnEvtId,\n")
			body_file.write("  IN  UINTN       ScrnEvtHdl\n  )\n")
			body_file.write("{\n  return SCRN_RC_SUCCESS;\n}\n\n")
			body_file.write("SCRN_STATUS \n" + concatStr + "OnDraw ( \n")
			body_file.write("  IN  SCRN_EVENT  ScrnEvtId,\n")
			body_file.write("  IN  UINTN       ScrnEvtHdl\n  )\n")
			body_file.write("{\n  return SCRN_RC_SUCCESS;\n}\n\n")
			body_file.write("SCRN_STATUS \n" + concatStr + "OnEvent ( \n")
			body_file.write("  IN  SCRN_EVENT  ScrnEvtId,\n")
			body_file.write("  IN  UINTN       ScrnEvtHdl\n  )\n")
			body_file.write("{\n  return SCRN_RC_SUCCESS;\n}\n\n")
			body_file.write("SCRN_STATUS \n" + concatStr + "OnLeave ( \n")
			body_file.write("  IN  SCRN_EVENT  ScrnEvtId,\n")
			body_file.write("  IN  UINTN       ScrnEvtHdl\n  )\n")
			body_file.write("{\n  return SCRN_RC_SUCCESS;\n}\n\n")
		except IOError as err:
			print("IOError" + str(err) + "\n")
		finally:
			print("Finally: Close Current Body File: " + fileStr);
			if 'body_file' in locals():
				body_file.close()
			else:
				print("skip body_file.close \n");
	print("CbBodyCreator() ... OK")


try:
	in_file 		= open("__src_screen.txt")
	group_file 		= open("__src_group.txt")
	enum_file 		= open("1_enum.txt", "w")
	doc_c_file 		= open("2_dot_c.txt", "w")
	cb_header_file 	= open("3_cb_header.txt", "w")
	cb_array_file 	= open("4_cb_array.txt", "w")

	for each_line in group_file:
		group_name = each_line
	
	EnumCreator()
	
	maxlen = Doc_C_Creator()
	print("max length is: " + str(maxlen))
	
	CbHeaderCreator()
	
	CbArrayCreator(maxlen)

	CbBodyCreator()
	
except IOError as err:
	print("IOError" + str(err) + "\n")

finally:
	print("\n\nFinally: Close All Files!!");
	if 'in_file' in locals():
		in_file.close()
	else:
		print("skip in_file.close \n");
	# ------------------------------------
	if 'group_file' in locals():
		group_file.close()
	else:
		print("skip group_file.close \n");
	# ------------------------------------
	if 'enum_file' in locals():
		enum_file.close()
	else:
		print("skip enum_file.close \n");
	# ------------------------------------
	if 'doc_c_file' in locals():
		doc_c_file.close()
	else:
		print("skip doc_c_file.close \n");
	# ------------------------------------
	if 'cb_header_file' in locals():
		cb_header_file.close()
	else:
		print("skip cb_header_file.close \n");
	# ------------------------------------
	if 'cb_array_file' in locals():
		cb_array_file.close()
	else:
		print("skip cb_array_file.close \n");




