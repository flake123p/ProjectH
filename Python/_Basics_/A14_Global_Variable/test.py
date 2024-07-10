
global_var = 2.11

print('main():')
print('global_var = ', ++global_var)

def func_use_global():
	print('func_use_global():')
	print('global_var = ', global_var)

func_use_global()

def func_modify_global():
	global global_var # Must declare when to reassign global variables !!!!
	global_var += 1
	print('func_modify_global():')
	print('global_var = ', ++global_var)

func_modify_global()

print('main():')
print('global_var = ', ++global_var)

global_dict = {0:22, 1:44}

#
# If the global value is mutable, you can modify it without declaring it!!  
# (Flake: Think mutable as pointer. In this case, pointer doesn't been modified!!)
#
def func_modify_global_dict_1():
	global_dict[1] = 55;
	global_dict[2] = 66;

func_modify_global_dict_1()
print('global_dict = ', global_dict)

#
# Reassign a dictionary = modify pointer = need declare "global"
#
def func_modify_global_dict_2():
	global global_dict
	global_dict = {0:88, 1:99}

func_modify_global_dict_2()
print('global_dict = ', global_dict)


