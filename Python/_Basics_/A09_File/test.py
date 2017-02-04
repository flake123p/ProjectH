
fin = open('words.txt')
print('fin = ', fin)

print('fin.readline() = ', fin.readline())
print('fin.readline() = ', fin.readline())

# for each line in file
print('for each_line in fin:')
for each_line in fin:
	print('each_line.strip() = ', each_line.strip(), '  (Remove characters: \\r\\n)')
	#each word
	each_word_list = each_line.split()
	print('each_word_list = ', each_word_list)
	#concat eachword
	concat_string = '-'
	concat_string = concat_string.join(each_word_list)
	print('concat_string = ', concat_string)

fout = open('write.txt', 'w')
fout.write('XXX\n')
fout.write('YYY')

ctr = 13
print('ctr = 0x{:08x}'.format(ctr))
print('ctr = {:08d}'.format(ctr))