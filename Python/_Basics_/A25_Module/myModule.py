
def linecount(filename):
	count = 0;
	for line in open(filename):
		count += 1
	return count

#
# Test code
#
if __name__ == '__main__':
	print('linecount(\'_env.bat\') = ', linecount('_env.bat'))