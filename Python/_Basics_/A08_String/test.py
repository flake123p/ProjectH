
myStr = 'Texi'
print('myStr     = ', myStr)
print('myStr[1]  = ', myStr[1])
print('myStr[-1] = ', myStr[-1])
print('myStr[-2] = ', myStr[-2])
print('myStr[1:3] = ', myStr[1:3])
print('myStr[:3]  = ', myStr[:3])
print('myStr[1:]  = ', myStr[1:])
print('len(myStr) = ', len(myStr))

print('for myChar in myStr:')
for myChar in myStr:
	print(myChar)

# Strings are immutable
# myStr[0] = 'X'   <= ILLEGAL!!
myStr2 = 'xx' + myStr[:3]
print('myStr2         = ', myStr2)
print('myStr2.upper() = ', myStr2.upper())
print('myStr2.lower() = ', myStr2.lower())
print('myStr2.find(\'x\') = ', myStr2.find('x'))
print('myStr2.find(\'x\', 1) = ', myStr2.find('x', 1))
print('myStr2.find(\'x\', 2, 4) = ', myStr2.find('x', 2, 4), '(Not include 4, so not found!)')
print('myStr2.find(\'x\', 2, 5) = ', myStr2.find('x', 2, 5))

if 'a' in myStr2:
	print('\'a\' in myStr2: True')
else:
	print('\'a\' in myStr2: False')

if 'amy' > 'bob':
	print('amy > bob')
else:
	print('amy < bob')

if 'amy' > 'Bob':
	print('amy > Bob')
else:
	print('amy < Bob')

#string compair
if 'amy' == 'amy':
	print('amy == amy')