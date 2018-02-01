
fin = open('log/rev_list.txt')

i = 0
next_rev = ''
for each_line in fin:
	i = i + 1
	if i == 2:
		next_rev = each_line

if i != 1:
	fout = open('log/check_update.txt', 'w')
	fout.write(next_rev)