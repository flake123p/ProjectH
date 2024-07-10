#!/usr/bin/python

#
# Include library
#
import os
import sys
sys.path.insert(0, '../../../Python/__PF/_Lib')
from LibEverything import *

# Usage: mod_list_parser.py  <input file (mod list)>  <output file 1(mod tree)>  <output file 2(all modules)>
# argv:  argv[0]             argv[1]                  argv[2]                    argv[3]

import inspect
def lineno():
	"""Returns the current line number in our program."""
	return inspect.currentframe().f_back.f_lineno

def OpenFile(fileName, mode = 'r'): # mode : 'r', 'w', ...
	try:
		fp = open(fileName, mode)
	except OSError as err:
		print("OS error: {0}".format(err))
		sys.exit(1)
	except:
		print("Unexpected error:", sys.exc_info()[0])
		sys.exit(1)
	return fp

def OpendFileAndGetList(fileName, mode = 'r'): # mode : 'r', 'w', ...
	fp = OpenFile(fileName, mode)
	list = []
	for each_line in fp:
		list.append(each_line.strip())
	fp.close()
	return list

def RecursiveCheck(mainModNode):
	if mainModNode.child == None:
		return
	depth = 1
	reverting = 0
	workList = []
	workList.append(mainModNode.str)
	curr = mainModNode.child
	while depth != 0:
		if reverting == 0:
			#if curr.str == None:
			#	print('[ERROR]depth=', depth)
			#	sys.exit(1)
			if curr.str in workList:
				if curr.str == 'mod1':
					print('debugXX workList=', workList)
				curr.var = 1
				currParent = curr.parent
				while currParent != None:
					if currParent.str == curr.str:
						currParent.var = 1
					currParent = currParent.parent
			if curr.child != None:
				workList.append(curr.str)
				curr = curr.child
				depth = 1 + depth
			elif curr.next != None:
				curr = curr.next
			else:
				reverting = 1
		else: #if reverting == 0:
			curr = curr.parent
			workList.pop()
			depth = depth - 1
			if curr.next != None:
				curr = curr.next
				reverting = 0

#
# Main
#
if len(sys.argv) != 4:
	print("Arguments Number Error. It should be 4.")
	sys.exit(1)

#
# File IO sample code
#
finModList   = OpenFile(str(sys.argv[1]))
foutModTree  = OpenFile(str(sys.argv[2]), 'w')
foutModAll   = OpenFile(str(sys.argv[3]), 'w')

modListFile = str(sys.argv[1])
t1Mgr = LibLinkedList.LLT1Mgr()

# folder parsing
currPath = os.getcwd()
splitPath = os.path.split(currPath)
#print(splitPath)
prevPath = splitPath[0]
currFolder = splitPath[1]
t1Mgr.addT1NextNode(0, currFolder)

#
# read to tree and recursive check
#
currNode = t1Mgr.t1Head;
depth = 1
reverting = 0
while depth != 0:
	if reverting == 0:
		#print('debug, depth=', depth, 'currNode.str=', currNode.str)
		#t1Mgr.t1Head.dump()
		
		#recursive check
		RecursiveCheck(t1Mgr.t1Head)

		# add children if there is no recursive
		if currNode.var == 0 and len(currNode.str) != 0:
			#import child
			targetFile = prevPath + '\\\\' + currNode.str + '\\\\' + modListFile

			modList = OpendFileAndGetList(targetFile, 'r')
			workNode = currNode
			firstChildCreated = 0
			for m in range(len(modList)):
				newNode = LibLinkedList.LLNode(0, modList[m])
				if firstChildCreated == 0:
					workNode = workNode.addChildNode(newNode)
					firstChildCreated = 1
				else:
					workNode = workNode.addNextNode(newNode)
			#print('modList=', modList, 'currNode.str=', currNode.str)
		if currNode.child != None:
			depth = 1 + depth
			currNode = currNode.child
		elif currNode.next != None:
			currNode = currNode.next
		else:
			reverting = 1
	else: #if reverting == 0:
		currNode = currNode.parent
		#if currNode == None:
		#	print('Assert error in:', lineno(), 'depth=', depth)
		#	sys.exit(1)
		depth = depth - 1
		if currNode != None and currNode.next != None:
			currNode = currNode.next
			reverting = 0

# debug dump
t1Mgr.t1Head.dump()
t1Mgr.t1Head.dumpToFile(foutModTree)
t1Mgr.updateCompletePathList()
t1Mgr.printCompletePathList()

'''
allMod = []
for m in range(len(t1Mgr.completePathList)):
	for n in range(len(t1Mgr.completePathList[m])):
		if t1Mgr.completePathList[m][n].str not in allMod:
			allMod.append(t1Mgr.completePathList[m][n].str)
print('allMod list=', allMod)
'''
allMod2 = []
maxDepth = 0
for m in range(len(t1Mgr.completePathList)):
	currDepth = len(t1Mgr.completePathList[m])
	if currDepth > maxDepth:
		maxDepth = currDepth
print('maxDepth=', maxDepth)

while maxDepth != 0:
	for m in range(len(t1Mgr.completePathList)):
		if len(t1Mgr.completePathList[m]) == maxDepth:
			if t1Mgr.completePathList[m][0].str not in allMod2:
				allMod2.append(t1Mgr.completePathList[m][0].str)
			t1Mgr.completePathList[m].pop(0)
	maxDepth = maxDepth - 1
print('allMod2 list=', allMod2)

for m in range(len(allMod2)):
	# skip first mod, because it's itself
	if m != 0:
		foutModAll.write(allMod2[m] + '\n')

#
# end
#
finModList.close()
foutModTree.close()
foutModAll.close()
