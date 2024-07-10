
fin  = open('ascii_control_char.txt')
fout = open('out.txt', 'w')
for each_line in fin:
	each_word_list = each_line.split()
	i = 0
	fout.write('\t{')
	for each_word in each_word_list:
		if i == 0:
			fout.write('{:5s}'.format(each_word+','))
		elif i == 1:
			fout.write('{:8s}'.format('"'+each_word+'",'))
		elif i == 2:
			fout.write('{:8s}'.format('"'+each_word+'",'))
		elif i == 3:
			if each_word == "-":
				fout.write('{:8s}'.format('" ",'))
			else:
				fout.write('{:8s}'.format('"'+each_word+'",'))
		else:
			break
		i += 1
	del each_word_list[0]
	del each_word_list[0]
	del each_word_list[0]
	del each_word_list[0]
	concat_string = " "
	concat_string = concat_string.join(each_word_list)
	fout.write('"'+concat_string+'"},\n')
fin.close()
