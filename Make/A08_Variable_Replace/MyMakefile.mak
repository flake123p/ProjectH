
defVar = aaao bbbo ccco
# Replace at the end of string
newVar = $(defVar:o=.c)
# Use pattern
newVar2 = $(defVar:%ao=%.c)

all :
	@echo defVar  = $(defVar)
	@echo newVar  = $(newVar)
	@echo newVar2 = $(newVar2)
