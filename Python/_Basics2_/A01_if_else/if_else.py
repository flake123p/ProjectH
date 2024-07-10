a, b, c = 3, 5, 7
x = None #create verialbe but doesn't set init value

#the judgement needs add ":"
if a < b:
	if b < c:
		x = c
	else:
		x = b
else:
	if a < c:
		x = c
	else:
		pass #if do nothing, should key "pass"
print(x)
