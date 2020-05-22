
include _0_Env_LibPara.mak
include _0_Env_ModPara_ForLocal.mak

#
# Use CXX, $(CXX) to build all files. Default CXX is g++
#
CXXFLAGS = -g -Wall -std=c++11 $(OFS)

# Define paths
src_dir = src

# For compile
INC_FOR_COMPILE = -Iinclude -I$(lib_include_path) $(all_mod_includes)

# Define "TAB" character which is different in WIN & LINUX
MyNull =
ifeq ($(MY_OS),WIN)
	MyTab =$(MyNull)	$(MyNull)
else
	MyTab = '\t'
endif

# ============================================================================================
# Use wildcard to get all .c & .cpp file names. (No path)
src_cpp = $(notdir $(wildcard $(src_dir)/*.cpp))
src_c   = $(notdir $(wildcard $(src_dir)/*.c))
src_all = $(src_cpp) $(src_c)


# ============================================================================================
# Create .o file names.
obj_cpp = $(src_cpp:.cpp=.o)
obj_c   = $(src_c:.c=.o)
obj_all = $(obj_cpp) $(obj_c)


# ============================================================================================
# Create .d file names. (Makefile)
mfile_cpp = $(src_cpp:.cpp=.d)
mfile_c   = $(src_c:.c=.d)
mfile_all = $(mfile_cpp) $(mfile_c)


# ============================================================================================
mfile_dir = Makefiles
build_dir = build
archive_dir = archive
# Create .o file names. (With path)
obj_all_with_path = $(patsubst %, $(build_dir)/%, $(obj_all))


# Update the scan paths for prerequisites
vpath %.d $(mfile_dir)
vpath %.o $(build_dir)
vpath %.a $(archive_dir)
