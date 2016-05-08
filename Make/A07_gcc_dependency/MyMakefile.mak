#
# <targets ...> : <target-PATTERN> : <prereq-PATTERNS ...>
#     <commands>
#     ...
#
# In commands:
#     $< is as prerequisite !!!
#     $@ is as target !!!!!!!!!
#
.PHONY: all

all :
	@echo === LONG DEPENDENCY (-M) =========
	gcc -M main.c
	@echo === COMPACT DEPENDENCY (-MM) =========
	gcc -MM main.c
