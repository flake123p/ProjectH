
"""
A list is a sequence
	1.Can be any type
	2.The values in a list are called elements or sometimes items
	3.Declare with square brackets: [ ]
	4.Can be nested. [x, y, [z1, z2]]
"""

myList = ['aa', 1.2, [88.8, 99]]
print('myList = ', myList)

myList[0] = 'xx'
print('myList = ', myList, '  (List is mutable, unlike string.)')

print('\'xx\' in myList = ', 'xx' in myList)
print('99   in myList    = ', 99 in myList)
print('99   in myList[2] = ', 99 in myList[2])

print('type(myList) = ', type(myList))

for i in range(len(myList)):
	print('myList[', i, '] = ', myList[i])
print('myList[2][1] = ', myList[2][1])

myList2 = [0, 10, 20]
print('myList2 = ', myList2)
print('myList2 + myList2  = ', myList2 + myList2)
print('myList2 * 3        = ', myList2 * 3)
print('myList2[:2]        = ', myList2[:2])
myList2.append(90) #Can't execute inside a print function
print('myList2.append(90) = ', myList2)
myList3 = [66, 77]
print('myList3 = ', myList3)
myList2.extend(myList3) #Can't execute inside a print function
print('myList2.extend(myList3) = ', myList2)
myList2.sort() #Can't execute inside a print function
print('myList2.sort()          = ', myList2)
print('sum(myList2)            = ', sum(myList2))
myList2.pop(1)
print('myList2.pop(1)          = ', myList2)
myList2.pop()
print('myList2.pop()           = ', myList2, '  (pop last)')
del myList2[2:4]
print('del myList2[2:4]        = ', myList2)
myList2.remove(20)
print('myList2.remove(20)      = ', myList2)

myStr = 'aa bb-cc'
print('myStr = ', myStr)
myList4 = list(myStr)
print('myList4 = list(myStr)   = ', myList4)
myList5 = myStr.split()
print('myList5 = myStr.split() = ', myList5)
myList6 = myStr.split('-') #delimiter '-'
print('myList6 = myStr.split(\'-\') = ', myList6)
myDelimiter = '@'
print('myDelimiter.join(myList6)  = ', myDelimiter.join(myList6))

# exist
testList = ['aa', 'bb', 'cc']
if 'aa' in testList:
	print('aa exist in list')
if 'dd' not in testList:
	print('dd not exist in list')

# find & index
print('index of aa in testList is:', testList.index('aa'))
#print('index of dd in testList is:', testList.index('dd')) # ==> run error, because dd is not in list
