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

# previous folder
print('curr  folder = ', os.getcwd()) # current
print('prev  folder = ', os.path.dirname(os.getcwd()))
print('prev2 folder = ', os.path.dirname(os.path.dirname(os.getcwd())))
print('prev3 folder = ', os.path.dirname(os.path.dirname(os.path.dirname(os.getcwd()))))
# split for find out indivisual folder name
split1 = os.path.split(os.getcwd())
split2 = os.path.split(split1[0])
print('split1       = ', split1)
print('split1[1]    = ', split1[1])
print('split2       = ', split2)
print('split2[1]    = ', split2[1])
newPath = split2[0] + '\\\\MOD'
print('newPath      = ', newPath)
split3 = os.path.split(newPath)
print('split3       = ', split3)
