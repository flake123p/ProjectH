#
# $(dir <names...>)
#   extract directory path name
#
# $(notdir <names...>)
#   extract non-directory path name
#

# If Equal
x = aaa/bbb/ccc.d
y = $(dir $(x))
z = $(notdir $(x))

all :
	@echo    dir of x  = $(y)
	@echo notdir of x  = $(z)

