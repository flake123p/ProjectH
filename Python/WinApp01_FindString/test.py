
print('Please type string for search')
userDefineString = input()

fin = open('result.txt')
fout = open('FinalResult.txt', 'w')

pathShortenCtr = 3
findCtr = 0
print('******')
try:
	for everyFileName in fin:
		#
		# Make short file name
		#
		strStart = 0
		for i in range(pathShortenCtr):
			strStart = everyFileName.find('\\', strStart) + 1
		shortFileName = everyFileName[strStart:].strip()
		
		everyFileFp = open(everyFileName.strip())
		lineNum = 1
		for everyLine in everyFileFp:
			if userDefineString in everyLine:
				findCtr += 1
				print("[%3d][Line:%3d] %s" % (findCtr, lineNum, shortFileName))
				fout.write("[%3d][Line:%3d] %s\n" % (findCtr, lineNum, shortFileName))
			lineNum += 1
		everyFileFp.close()
finally:
	fin.close()

print('******\nSearch result for string: ', userDefineString)
fout.write("******\nSearch result for string: %s" % userDefineString)
fout.close()