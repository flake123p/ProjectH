
fin = open('log/cur_rev_temp.txt')

author = ''
rev = ''

# for each line in file
for each_line in fin:
	#print('each_line.strip() = ', each_line.strip(), '  (Remove characters: \\r\\n)')
	#each word
	each_word_list = each_line.split()
	#for each_word in each_word_list:
	if len(each_word_list) >= 4:
		if each_word_list[2] == 'Author:':
			author = each_word_list[3]
	if len(each_word_list) >= 4:
		if each_word_list[2] == 'Rev:':
			rev = each_word_list[3]

fout = open('log/cur_rev.txt', 'w')
fout.write(author + '\n')
fout.write(rev + '\n')