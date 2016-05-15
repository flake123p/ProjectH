'''
	Final comma for declare single element Tuple
		e.g.:
		x = 33,

		e.g.:
		x = '1',

		e.g.:
		x = tuple()

	Tuple:
		1. Sequence of value(can be any type)
		2. Indexed by integers.
		3. Like lists, but tuples are "immutable".
'''

x = 11, 22
print('type(x) = ', type(x))
print('x = ', x)

x = tuple('banana')
print('type(x) = ', type(x))
print('x = ', x)

# TypeError: 'tuple' object does not support item assignment
# x[0] = 'c'  #WRONG

# You can replace one tuple with another
y = ('c',) + x[1:]
print('type(y) = ', type(y))
print('y = ', y)

# Tuple assignment
print('SWAP a b:')
a = 2
b = 7
a, b = b, a #SWAP
print('a = ', a)
print('b = ', b)

print('Split string using tuple:')
x = 'flake123p@gmail.com'
a, b = x.split('@')
print('a = ', a)
print('b = ', b)
