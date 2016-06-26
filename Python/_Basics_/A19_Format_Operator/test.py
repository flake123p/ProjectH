'''
	%d for integer
	%g for floating-point
	%s for string
'''
x = 123
y = 1.0001

print('%d_%g'     % (x, y))
print('%8d_%8g'   % (x, y))
print('%-8d_%-8g' % (x, y))