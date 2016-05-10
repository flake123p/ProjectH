
# If Equal
x = abc
ifeq ($(x), abc)
	y = 1
else
	y = 0
endif

# If Define
ifdef z
	y += A
else
	y += B
endif

all :
	@echo y  = $(y)

