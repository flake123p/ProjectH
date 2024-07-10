
fin = open('log/rev_list_temp.txt')
fout = open('log/rev_list.txt', 'w')

def isThisLineRevision(each_word_list):
	if len(each_word_list) >= 2:
		if each_word_list[0][0] == 'r':
			if each_word_list[1] == '|':
				return True
	return False

# for each line in file
for each_line in fin:
	each_word_list = each_line.split()
	if isThisLineRevision(each_word_list):
		fout.write(each_word_list[0][1:] + '\n')
