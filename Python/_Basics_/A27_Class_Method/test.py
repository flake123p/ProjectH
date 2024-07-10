'''
Full Class Usage Demonstration:
	__init__
	__str__
	__add__
	__lt__
	Type-based dispatch (in __add__) ... Polymorphism
'''

#
# copy for copy object
#
import copy

class MyFixPoint(object):
	def __init__(self, num=95, q=2):
		self.num = num
		self.q   = q
	
	def __str__(self):
		return 'Number=%d, Q=%d ... Float Point=%f' % (self.num, self.q, self.num/(10.0**self.q))
	
	def __add__(self, other):
		if isinstance(other, MyFixPoint):
			otherTemp = copy.deepcopy(other)
			if(self.q > otherTemp.q):
				otherTemp.ModifyQ(self.q)
			else:
				self.ModifyQ(otherTemp.q)
			self.num += otherTemp.num
		else:
			self.ModifyQ(0)
			self.num += other
	
	def __lt__(self, other):
		obj1 = copy.deepcopy(self)
		obj2 = copy.deepcopy(other)
		if(obj1.q > obj2.q):
			obj2.ModifyQ(obj1.q)
		else:
			obj1.ModifyQ(obj2.q)
		return obj1.num < obj2.num
		
	def ModifyQ(self, newQ):
		q_diff = self.q - newQ
		if(q_diff > 0):
			self.num = self.num // 10**q_diff #Floor Division
		else:
			self.num = self.num * 10**(-q_diff)
		self.q = newQ

fp = MyFixPoint()
print('fp  = ', fp)
fp2 = MyFixPoint(100, 1)
print('fp2 = ', fp2)
fp + fp2
print('fp + fp2 = ', fp)
print('fp < fp2 = ', fp < fp2)
fp + 2000
print('fp + 2000 = ', fp)
