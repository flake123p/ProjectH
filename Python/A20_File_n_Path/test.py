'''
	%d for integer
	%g for floating-point
	%s for string
'''
import os

print('os.getcwd() = ', os.getcwd())
print('abspath()   = ', os.path.abspath('test.py'))
print('exists()    = ', os.path.exists('test.py'))
print('isdir()     = ', os.path.isdir('test.py'))
print('isfile()    = ', os.path.isfile('test.py'))
print('listdir()   = ', os.listdir( os.getcwd() ))

#
# join() takes a directory and a file name and joins them into a complete path.
#
def walk(dirname):
	for name in os.listdir(dirname):
		path = os.path.join(dirname, name)
		if os.path.isfile(path):
			print(path)
		else:
			walk(path)

print('call walk()')
walk( os.getcwd() )
