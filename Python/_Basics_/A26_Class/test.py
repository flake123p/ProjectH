
#
# copy for copy object
#
import copy

class Point(object):
	''' Test Object'''

class Rect(object):
	''' Test Object'''

print('print(Point) = ', Point)

#
# Instantiation
#
obj = Point()
print('obj = ', obj)

obj.x = 3.0
obj.y = 4.0
print('obj.x = ', obj.x)
print('obj.y = ', obj.y)

obj2 = copy.copy(obj)
print('obj == obj2 = ', obj == obj2)
print('obj is obj2 = ', obj is obj2)
print('obj2.x = ', obj2.x)
print('obj2.y = ', obj2.y)

rect1 = Rect()
rect1.a = 11
rect1.obj = Point()
rect1.obj.x = 5.0
rect1.obj.y = 7.0
print('rect1.a = ', rect1.a)
print('rect1.obj.x = ', rect1.obj.x)
print('rect1.obj.y = ', rect1.obj.y)
# Shallow copy
print('... Shallow Copy ...')
rect2 = copy.copy(rect1)
print('rect1 is rect2 = ', rect1 is rect2)
print('rect1.obj is rect2.obj = ', rect1.obj is rect2.obj) #Note
# This will change rect2.obj & rect1.obj
rect2.obj.y = 999.0
print('rect1.obj.y = ', rect1.obj.y)

# Deep copy
print('... Deep Copy ...')
rect2 = copy.deepcopy(rect1)
print('rect1 is rect2 = ', rect1 is rect2)
print('rect1.obj is rect2.obj = ', rect1.obj is rect2.obj) #Note