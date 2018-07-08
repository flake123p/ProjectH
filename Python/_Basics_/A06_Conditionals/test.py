
#
# if - else
#
x = 3;
if x > 0:
	print('x > 0')
elif x < 0:
	print('x < 0')
else:
	print('x == 0')
# if x == 0:
# if x != 0:

#
# Use with: and, or
#
if x > 0 or x < 0:
	print('x != 0')

#
# while
#
while x > 0:
	print('while x > 0 (x=', x, ')')
	if x == 2:
		print('if x == 2, break')
		break
	x = x - 1

#
# for
#
print('for i in range(4):')
for i in range(4):
	print('i = ', i)

#
# switch case
#
# If you'd like defaults you could use the dictionary get(key[, default]) method:
def f(x):
    return {
        'a': 1,
        'b': 2,
    }.get(x, 9)    # 9 is default if x not found

print('f(\'a\') = ', f('a'))
print('f(\'k\') = ', f('k'))