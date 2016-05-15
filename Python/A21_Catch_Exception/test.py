
try:
	fin = open('bad_file')
	for line in fin:
		print(line)
	fin.close()
except:
	print('Something went wrong!')