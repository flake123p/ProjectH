'''
	%d for integer
	%g for floating-point
	%s for string
'''
import os

fileName = 'test.py'
print('os.getcwd() = ', os.getcwd())
print('abspath()   = ', os.path.abspath(fileName))
print('exists()    = ', os.path.exists(fileName))
print('isdir()     = ', os.path.isdir(fileName))
print('isfile()    = ', os.path.isfile(fileName))
print('listdir()   = ', os.listdir( os.getcwd() ))

if fileName.endswith('.py') :
	print(fileName, ' is python file.')
elif fileName.endswith('.mp3') :
	print(fileName, ' is mp3 file.')
else :
	print(fileName, ' is unknown file.')

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
