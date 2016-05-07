#
# <targets ...> : <target-PATTERN> : <prereq-PATTERNS ...>
#     <commands>
#     ...
#
# In commands:
#     $< is as prerequisite !!!
#     $@ is as target !!!!!!!!!
#
.PHONY: amy.hello bob.hello
.PHONY: amy.cpp bob.cpp

all : amy.hello bob.hello

amy.hello bob.hello : %.hello : %.cpp
	@echo PREREQ is $<,  TARGET is $@

# EQUAL TO:
# amy.hello : amy.cpp
#     @echo PREREQ is amy.cpp,  amy.hello
# bob.hello : bob.cpp
#     @echo PREREQ is bob.cpp,  bob.hello