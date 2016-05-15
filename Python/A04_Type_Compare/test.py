

print('type(\'abc\') = ', type('abc'))  #class 'str'
x = 10
print('type(10)    = ', type(x))     #class 'int'
print('x = ', x)
x = 1000000000000
print('type(1000000000000) = ', type(x))     #class 'int'
print('x = ', x)
print('type(10.0)  = ', type(10.0))   #class 'float'
print('type(None)  = ', type(None))   #class 'NoneType'
print('type(True)  = ', type(True))   #class 'bool'
print('type(False) = ', type(False))   #class 'bool'

x = ['x'] # Singleton list
print('type(x) = ', type(x))
x = dict()
print('type(x) = ', type(x))
x = 3344, #final comma for tuple type
print('type(x) = ', type(x))

x = 'abc'

if type(x) == str:
	print('type compare method 1: true')
else:
	print('type compare method 1: false')

if isinstance(x, str):
	print('type compare method 2: true')
else:
	print('type compare method 2: false')