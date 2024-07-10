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

#
# zip()
#
s = 'xyz'
t = [10,20,30]
x = zip(s, t)
print('type(x) = ', type(x))
print('x = ', x)
# traverse
for i, j in x:
	print('zip() ... i = ', i, ', j = ', j)

#
# enumerate()
#
for i, j in enumerate('zxcv'):
	print('enumerate() ... i = ', i, ', j = ', j)

#
# items() ... dict to list of tuples
#
d = {'a':0, 'b':1, 'c':2}
t = d.items()
print('type(t) = ', type(t))
print('t = ', t)

#
# dict() + zip() ... list of tuples to dict
#
d = dict(zip('abc', range(3)))
print('type(d) = ', type(d))
print('d = ', d)
d['d'] = 3 #update dictionary
print('type(d) = ', type(d))
print('d = ', d)

#
# DSU: Decorate, Sort, Undecorate
#
def sort_by_length(words):
	t = []
	for word in words:
		t.append((len(word), word))
	
	print('before sort: t = ', t)
	t.sort(reverse = True)
	print('after sort:  t = ', t)
	
	res = []
	for length, word in t:
		res.append(word)
	
	return res

result = sort_by_length(['a', 'bb', 'dddd', 'ccc'])
print('result = ', result)