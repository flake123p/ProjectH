def StringToLowerCase(str):
	return str.lower()

def StringToUpperCase(str):
	return str.upper()

def CharToUpperCase(str, position, count):
	newStr = ""
	i_posi = 0
	i_count = 0
	for char in str:
		if i_posi >= position and i_count < count:
			char = char.upper()
			i_count = i_count + 1
		newStr = newStr + char
		i_posi = i_posi + 1
	return newStr

def FillWithCharInHead(str, minLength, charToFill):
	strlen = len(str)
	if strlen < minLength:
		newStr = ""
		diff = minLength - strlen
		while diff > 0:
			diff -= 1
			newStr += charToFill
		newStr += str
		return newStr
	else:
		return str

def FillWithCharInTail(str, minLength, charToFill):
	strlen = len(str)
	if strlen < minLength:
		newStr = str
		diff = minLength - strlen
		while diff > 0:
			diff -= 1
			newStr += charToFill
		return newStr
	else:
		return str