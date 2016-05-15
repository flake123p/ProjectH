
fin = open('words.txt')
print('fin = ', fin)

print('fin.readline() = ', fin.readline())
print('fin.readline() = ', fin.readline())

print('for x in fin:')
for x in fin:
	print('x.strip() = ', x.strip(), '  (Remove characters: \\r\\n)')

fout = open('write.txt', 'w')
fout.write('XXX\n')
fout.write('YYY')