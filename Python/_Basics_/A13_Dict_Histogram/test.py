
"""
Dictionary:
	1.key-value pair or item
"""

def histogram(str):
	d = dict()
	for c in str:
		if c not in d:
			d[c] = 1;
		else:
			d[c] += 1;
	return d

def print_hist(h):
	for c in h:
		print(c, h[c])


h = histogram('Banana')
print('histogram = ', h)

print_hist(h)