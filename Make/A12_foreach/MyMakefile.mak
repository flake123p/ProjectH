#
# $(foreach <var>,<list>,<text>)
#   Extract every item in <list> to <var>, and do <text> part.
#

# If Equal
x = aa bb cc dd
y = $(foreach i,$(x),$(i).c)

all :
	@echo x  = $(x)
	@echo y  = $(y)
