a = [1, 2, 3]
b = [4, 5, 6]
c = a + b
print(c)
print(a)
a.append(7)
print(a)
print(sum(a))

t = ['d', 'c', 'e', 'b', 'a']
t.sort()
print(t)
x = t.pop(1)
#also using : del t[1]
#if doesn't know index : t.remove('b')
print(t)
print(x)
