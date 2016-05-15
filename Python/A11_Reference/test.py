
"""
A list is a sequence
	1.Can be any type
	2.The values in a list are called elements or sometimes items
	3.Declare with square brackets: [ ]
	4.Can be nested. [x, y, [z1, z2]]
"""
myStr1 = 'aabbcc'
myStr2 = 'aabbcc'
print('myStr1 = ', myStr1)
print('myStr2 = ', myStr2)
print('myStr1 is myStr2 = ', myStr1 is myStr2, '  (Equivalent + Identical)')

myList1 = [10, 20, 30]
myList2 = [10, 20, 30]
print('myList1 = ', myList1)
print('myList2 = ', myList2)
print('myList1 is myList2 = ', myList1 is myList2, '  (Equivalent + Not Identical)')

print('When you pass a list to a function, the function gets a reference to the list.')
t1 = [1, 2]
t2 = t1.append(3)
t3 = t1 + [3]
print('t1 = [1, 2]')
print('t2 = t1.append(3)')
print('t3 = t1 + [3]')
print('t1 now is ', t1)
print('t2 now is ', t2)
print('t3 now is ', t3)
