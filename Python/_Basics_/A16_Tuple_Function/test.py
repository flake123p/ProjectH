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

# Build-in function divmod() returns a tuple
x = divmod(9, 4)
print('type(x) = ', type(x))
print('x = ', x)

# Another way
quot, rem = divmod(9, 4)
print('type(quot = ', type(quot))
print('quot = ', quot)
print('type(rem) = ', type(rem))
print('rem = ', rem)

#
# Variable-length argument tuples [Use * ("gather")]
#
# Scatter the tuple to fit in fixed-length argument function. [Use * (scatter)]
#
def Vari_Argu_Func(*args):
	print('Vari_Argu_Func()... args = ', args)

Vari_Argu_Func(1, 3, 'c')
Vari_Argu_Func(11, 22)

# Scatter tuple for divmod()
x = 10, 3
#quot, rem = divmod(x) #WRONG
quot, rem = divmod(*x)
print('quot = ', quot)
print('rem = ', rem)