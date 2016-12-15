'''
	%d for integer
	%g for floating-point
	%s for string
'''
import os
from shutil import copyfile

BaseEnvBat = os.path.join(os.getcwd(), '_env.bat')
print('BaseEnvBat =', BaseEnvBat)

def walk(dirname):
	for name in os.listdir(dirname):
		path = os.path.join(dirname, name)
		if os.path.isfile(path):
			if name == '_env.bat':
				
				if BaseEnvBat != path:
					print('REPLACE: ', path)
					copyfile(BaseEnvBat, path)
		else:
			walk(path)

print('call walk()')
walk( 'D:\WS\workspace\ProjectH' )



