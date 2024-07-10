
"""
Dictionary:
	1.key-value pair or item
"""

x = dict()
x['a'] = 10
x['a'] = 20
x['b'] = 100

print('x = ', x)
print('\'c\' in x = ', 'c' in x)
print('\'a\' in x = ', 'a' in x)
print('100 in x = ', 100 in x)

# returns the values as a list
vals = x.values()
print('vals = ', vals)

print('dict = ', x)

#
#A previously computed value that is stored for later use is called a "memo"!!!!!
#
Init_Dict_With_Mutiple_Pair = {0:44, 1:33}
print('Init_Dict_With_Mutiple_Pair = ', Init_Dict_With_Mutiple_Pair)
