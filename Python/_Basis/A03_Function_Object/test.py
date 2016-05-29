
def func1(a):
	print('func1 a = ', a)

# Function as argument
def do_twice(f, x):
	f(x)
	f(x+1)


do_twice(func1, 10)
