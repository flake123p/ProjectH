

class MyClass(object):
	def __init__(self, a=95, b=2):
		self.a = a
		self.b = b
	
	def __str__(self):
		return '[MyClass]a=%d, b=%d' % (self.a, self.b)

# Parent class in parenthesis
class MyClass2(MyClass):
	def __str__(self):
		return '[MyClass2]a=%d, b=%d' % (self.a, self.b)
		
obj1 = MyClass()
print('obj1 => ', obj1)
obj2 = MyClass2()
print('obj2 => ', obj2)
