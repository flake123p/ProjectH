
x = aa.o bb.o

all : $(x)

$(x) : %.o : %.c
	@echo [1, Target] $@
	@echo [2, Function Argument] $%
	@echo [3, Prerequisite] $<
	@echo [4, Prerequisite which is newer] $?
	@echo [5, Prerequisite all, remove repeat] $^
	@echo [6, Prerequisite all, reserve repeat] $+
	@echo [7, Target string before pattern] $*

#
# Use with D & F (Equal to function dir and notdir)
#
# if $@ is dir/foo.o
#   $(@D) is dir
#   #(@F) is foo.o
#