

class MyClass(object):
	def __init__(self, aa=95, bb=2):
		self.a = aa
		self.b = bb
	
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
