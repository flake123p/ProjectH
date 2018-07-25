

class LLNode(object):
	def __init__(self, var=95, str='dummy'):
		self.var = var
		self.str = str
		self.next = None
		self.child = None
		self.numOfChild = 0
		self.parent = None
	
	def __str__(self):
		return '[LLNode] var=%d, str=%s' % (self.var, self.str)

	def addNextNode(self, newNextNode):
		self.next = newNextNode
		self.next.parent = self.parent
		#
		if self.parent != None:
			self.parent.numOfChild = 1 + self.parent.numOfChild
		#
		#update current node pointer
		return self.next
	
	def addChildNode(self, newChildNode):
		self.child = newChildNode
		self.child.parent = self
		#
		self.numOfChild = 1 + self.numOfChild
		#
		#update child node pointer
		return self.child

	def dump(self, level=0, curr=None):
		if curr == None:
			curr = self
		if curr.parent == None:
			parentStr = 'None'
		else:
			parentStr = curr.parent.str
		print(level * '	', curr.var, '-', curr.str, ',parent:', parentStr, ',child NO:', curr.numOfChild)
		if curr.child != None:
			level = level + 1
			self.dump(level, curr.child)
			level = level - 1
		if curr.next != None:
			self.dump(level, curr.next)
	
	def dumpToFile(self, fp, level=0, curr=None):
		if curr == None:
			curr = self
		if curr.parent == None:
			parentStr = 'None'
		else:
			parentStr = curr.parent.str
		tempStr = level * '	' + str(curr.var) + ' ' + curr.str + '\n'
		fp.write(tempStr)
		if curr.child != None:
			level = level + 1
			self.dumpToFile(fp, level, curr.child)
			level = level - 1
		if curr.next != None:
			self.dumpToFile(fp, level, curr.next)

class LLT1Mgr(object):
	def __init__(self):
		self.t1List = []
		self.t1Head = None
		self.t1Curr = None
		self.completePathList = []
	
	def __str__(self):
		return '[LLMgr] t1List len = %d, completePathList len = %d' % (len(self.t1List), len(self.completePathList))

	def addT1NextNode(self, var, str):
		newNode = LLNode(var, str)
		self.t1List.append(newNode)
		if self.t1Head == None:
			self.t1Head = newNode
		else:
			self.t1Curr.addNextNode(newNode)
		self.t1Curr = newNode
		return self.t1Curr

	def updateCompletePathList(self):
		history = [] #node workthrough history
		history.append(self.t1List[0])
		curr = self.t1List[0]
		reverting = 0
		end = 0
		while end != 1:
			if reverting == 0:
				if curr.child != None:
					history.append(curr.child)
					curr = curr.child
				else:
					# end of depth path
					dumpList = []
					for m in range(len(history)):
						dumpList.append(history[m].str)
					##print(dumpList)
					self.completePathList.append(list(history))
					#
					if curr.next == None:
						reverting = 1
					else:
						history.pop()
						history.append(curr.next)
						curr = curr.next
			else:
				history.pop()
				if len(history) == 0:
					end = 1
				else:
					curr = history[-1]
					if curr.next != None:
						curr = curr.next
						history.pop()
						history.append(curr)
						reverting = 0

	def printCompletePathList(self):
		for m in range(len(self.completePathList)):
			dumpList = []
			for n in range(len(self.completePathList[m])):
				dumpList.append(self.completePathList[m][n].str)
			print(dumpList)

def Demo():
	t1Mgr = LLT1Mgr()
	t1Mgr.addT1NextNode(0, 'No0')
	print('t1Curr = ', t1Mgr.t1Curr)
	t1Mgr.addT1NextNode(1, 'no1')
	print('t1Curr = ', t1Mgr.t1Curr)
	
	node_no1 = t1Mgr.t1Curr
	# child
	node_no1.addChildNode(LLNode(11, 'no11'))
	# grand child
	node_no1.child.addChildNode(LLNode(111, 'no111'));
	# child
	node_no1.child.addNextNode(LLNode(12, 'no12'));
	# child
	node_no1.child.next.addNextNode(LLNode(13, 'no13'));
	# grand child
	node_no1.child.next.next.addChildNode(LLNode(131, 'No131'))
	# grand child
	node_no1.child.next.next.child.addNextNode(LLNode(132, 'No132'))
	
	t1Mgr.addT1NextNode(2, 'no2')
	print('t1Curr = ', t1Mgr.t1Curr)
	
	print('	' * 2, 'Demo linked list done...');
	
	t1Mgr.t1Head.dump()
	print(t1Mgr)
	t1Mgr.updateCompletePathList()
	print(t1Mgr.completePathList)
	t1Mgr.printCompletePathList()

	for m in range(len(t1Mgr.completePathList)):
		dumpList = []
		for n in range(len(t1Mgr.completePathList[m])):
			dumpList.append(t1Mgr.completePathList[m][n].str)
		if 'no1' in dumpList:
			print('m = ', m, ', no1 exist')
		else:
			print('m = ', m, ', no1 not exist')
#
# main()
#
