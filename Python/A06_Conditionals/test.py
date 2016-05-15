
x = 3;
if x > 0:
	print('x > 0')
elif x < 0:
	print('x < 0')
else:
	print('x == 0')

# Use with: and, or
if x > 0 or x < 0:
	print('x != 0')

while x > 0:
	print('while x > 0 (x=', x, ')')
	if x == 2:
		print('if x == 2, break')
		break
	x = x - 1

print('for i in range(4):')
for i in range(4):
	print('i = ', i)