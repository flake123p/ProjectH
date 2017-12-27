
#
# Include library
#
import sys
sys.path.insert(0, '../_Lib')
from LibEverything import *

inFile = open('uart_cmd_name.txt')
funcFile = open('funcFile.txt', 'w')
cbArrayFile = open('cbArrayFile.txt', 'w')
enumFile = open('enumFile.txt', 'w')

def OutputFuncFile(funcString, countString):
	if funcString != "Null":
		funcFile.write('u8 APP_UART_Cmd' + countString + funcString + '(u16 len, u8 XDATA *p_cmdPayload)' + '\n')
		funcFile.write('{' + '\n')
		funcFile.write('    p_cmdPayload[0] = (u8)len; //Dummy for avoid build warning' + '\n')
		funcFile.write('    return 0;' + '\n')
		funcFile.write('}' + '\n\n')

def OutputCbArrayFile(funcString, countString, counter, enumString):
	if funcString != "Null":
		cbStr = '    APP_UART_Cmd' + countString + funcString + ','
	else:
		cbStr = '    NULL,'
	counterStr = str(counter)
	counterStr = LibString.FillWithCharInTail(counterStr, 2, ' ')
	cbStr = LibString.FillWithCharInTail(cbStr, 50, ' ') + '// ' + counterStr + ', ' + enumString
	cbArrayFile.write(cbStr + '\n')

def GetEnumString(each_line_upper_list, countString):
	countString = LibString.StringToUpperCase(countString)
	enumString = 'APP_UART_CMD_' + countString + '_'
	for each_word in each_line_upper_list:
		enumString += each_word + '_'
	enumString = enumString[:-1]
	return enumString

def OutputenumFile(enumString, counter):
	stringToDump = '    ' + enumString + ','
	stringToDump = LibString.FillWithCharInTail(stringToDump, 55, ' ')
	enumFile.write(stringToDump + '// ' +str(counter) + '\n')


counter = 0

for each_line in inFile:
	each_line = each_line.strip()
	each_line = LibString.StringToLowerCase(each_line)
	if len(each_line) > 0:
		funcString = ""
		each_word_list = each_line.split()
		for each_word in each_word_list:
			funcString = funcString + LibString.CharToUpperCase(each_word, 0 ,1)
		countString = hex(counter)
		countString = countString[2:]
		countString = LibString.FillWithCharInHead(countString, 2, '0')
		
		each_line_upper = LibString.StringToUpperCase(each_line)
		each_line_upper_list = each_line_upper.split()
		enumString = GetEnumString(each_line_upper_list, countString)
		
		OutputFuncFile(funcString, countString)
		OutputCbArrayFile(funcString, countString, counter, enumString)
		OutputenumFile(enumString, counter)
		counter += 1