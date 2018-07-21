

class LLNode(object):
	def __init__(self, var=95, str='dummy'):
		self.var = var
		self.str = str
		self.next = None
		self.child = None
	
	def __str__(self):
		return '[LLNode Dump] var=%d, str=%s' % (self.var, self.str)
	
	def dump(self, level=0, curr=None):
		if curr == None:
			curr = self
		print(level * '	', curr.var, '-', curr.str)
		if curr.child != None:
			level = level + 1
			self.dump(level, curr.child)
			level = level - 1
		if curr.next != None:
			self.dump(level, curr.next)

def Demo():
	head = LLNode(0, 'no0')
	curr = head
	print('curr = ', curr)
	
	newNode = LLNode(1, 'no1')
	curr.next = newNode
	curr = curr.next
	print('curr = ', curr)
	
	# child
	newNode = LLNode(11, 'no11')
	curr.child = newNode
	# grand child
	newNode = LLNode(111, 'no111')
	curr.child.child = newNode
	# child
	newNode = LLNode(12, 'no12')
	curr.child.next = newNode
	# child
	newNode = LLNode(13, 'no13')
	curr.child.next.next = newNode
	
	newNode = LLNode(2, 'no2')
	curr.next = newNode
	curr = curr.next
	print('curr = ', curr)
	
	print('	' * 2, 'Demo linked list done...');
	
	head.dump()
#
# main()
#
