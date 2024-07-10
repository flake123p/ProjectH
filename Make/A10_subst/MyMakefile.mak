#
# $(subst <from>,<to>,<text>)
#
# Replace <from> to <to> in <text>
#

# If Equal
x = $(subst e,E,feet on the street)

all :
	@echo x  = $(x)

