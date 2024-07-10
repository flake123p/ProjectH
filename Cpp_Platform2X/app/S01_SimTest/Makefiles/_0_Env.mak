
lib_make_file = _0_Env_LibPara.mak
mod_make_file = _0_Env_ModPara_ForApp.mak
ofs_make_file = OFS_From_App.mak

include $(lib_make_file)
include $(mod_make_file)
include $(ofs_make_file)

#
# Use CXX, $(CXX) to build all files. Default CXX is g++
#
CXXFLAGS = -g -Wall -std=c++11 $(OFS)
#
# "-static" is for external PC or the PC has no pdcurses library
#
#CXXFLAGS = -g -Wall -static


#
# Define paths
#
SRC_DIR = src
LIB_PATH = ../../../Cpp_Platform2/_Library

LIB_MGR_DIR = $(LIB_PATH)/lib_mgr
ifeq ($(OS),Windows_NT)
LIB_DIR = $(LIB_PATH)/lib_win
EXT_LIB_FOR_LINK += -lpdcurses
else
LIB_DIR = $(LIB_PATH)/lib_linux
EXT_LIB_FOR_LINK += -lcurses -lpthread
endif
# For g++, external libraries are used for linking, and must put them after .o files.
# EXT_LIB_FOR_LINK += -lpdcurses
# EXT_LIB_FOR_LINK += -lpthread


#
# For g++/gcc
#
INC_FOR_COMPILE = -Iinclude -I$(LIB_PATH)/include $(all_mod_includes)


#
# 定義tab，輸出tab在windows與linux要用不同的語法
#
MyNull =
ifeq ($(MY_OS),WIN)
	MyTab =$(MyNull)	$(MyNull)
else
	MyTab = '\t'
endif

# ============================================================================================
# 使用 wildcard 函式馬上攤開所有的 .cpp 與 .c
src_cpp = $(notdir $(wildcard $(SRC_DIR)/*.cpp))
src_c   = $(notdir $(wildcard $(SRC_DIR)/*.c))
src_all = $(src_cpp) $(src_c)


# ============================================================================================
# 以所有的 .cpp 與 .c 命名 .o 檔
obj_cpp = $(src_cpp:.cpp=.o)
obj_c   = $(src_c:.c=.o)
obj_all = $(obj_cpp) $(obj_c)


# ============================================================================================
# 以所有的 .cpp 與 .c 命名個別的 Makefile
mfile_cpp = $(src_cpp:.cpp=.d)
mfile_c   = $(src_c:.c=.d)
mfile_all = $(mfile_cpp) $(mfile_c)

#mfile_lib_mgr_cpp = $(lib_mgr_cpp:.cpp=.d)
#mfile_lib_mgr_c   = $(lib_mgr_c:.c=.d)
#mfile_lib_mgr     = $(mfile_lib_mgr_cpp) $(mfile_lib_mgr_c)

#mfile_lib_cpp = $(lib_cpp:.cpp=.d)
#mfile_lib_c   = $(lib_c:.c=.d)
#mfile_lib     = $(mfile_lib_cpp) $(mfile_lib_c)

# ============================================================================================
# 個別 Makefile 的放置資料夾
mfile_dir = Makefiles
# .o檔的放置資料夾
build_dir = build
archive_dir = archive
lib_build_dir = $(LIB_PATH)/$(build_dir)
# 把所有的 .o檔 加上 build output資料夾的路徑
obj_all_with_path = $(patsubst %,$(build_dir)/%,$(obj_all))
lib_obj_with_path = $(LIB_PATH)/$(archive_dir)/LibPF2.a

# 為這些中間檔加入 Make 的"依賴目標"描掃路徑 才不會每次都重新產生
vpath %.d $(mfile_dir)
vpath %.o $(build_dir)

