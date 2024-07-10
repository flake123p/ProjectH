
# 目前全面使用預設的 CXX 來編譯 , 也就是 g++

CXXFLAGS = -g -Wall -std=c++11 -static
# 靜態連結的執行檔是給沒裝 pdcurses library 的電腦用的
#CXXFLAGS = -g -Wall -static

SRC_DIR = src
LIB_MGR_DIR = lib/lib_mgr
ifeq ($(OS),Windows_NT)
LIB_DIR = lib/lib_win
LIB += -lpdcurses
else
LIB_DIR = lib/lib_linux
LIB += -lcurses
endif

# For g++
INC = -Iinclude -Ilib/lib_include
# For g++, link 時要放在 .o 後面才有用喔
# LIB += -lpdcurses
# LIB += -lpthread

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

# 使用 wildcard 函式馬上攤開所有的 .cpp 與 .c
lib_mgr_cpp = $(notdir $(wildcard $(LIB_MGR_DIR)/*.cpp))
lib_mgr_c   = $(notdir $(wildcard $(LIB_MGR_DIR)/*.c))
lib_mgr_all = $(lib_mgr_cpp) $(lib_mgr_c)

# 使用 wildcard 函式馬上攤開所有的 .cpp 與 .c
lib_cpp = $(notdir $(wildcard $(LIB_DIR)/*.cpp))
lib_c   = $(notdir $(wildcard $(LIB_DIR)/*.c))
lib_all = $(lib_cpp) $(lib_c)

# ============================================================================================
# 以所有的 .cpp 與 .c 命名 .o 檔
obj_cpp = $(src_cpp:.cpp=.o)
obj_c   = $(src_c:.c=.o)
obj_all = $(obj_cpp) $(obj_c)

obj_lib_mgr_cpp = $(lib_mgr_cpp:.cpp=.o)
obj_lib_mgr_c   = $(lib_mgr_c:.c=.o)
obj_lib_mgr     = $(obj_lib_mgr_cpp) $(obj_lib_mgr_c)

obj_lib_cpp = $(lib_cpp:.cpp=.o)
obj_lib_c   = $(lib_c:.c=.o)
obj_lib     = $(obj_lib_cpp) $(obj_lib_c)

# ============================================================================================
# 以所有的 .cpp 與 .c 命名個別的 Makefile
mfile_cpp = $(src_cpp:.cpp=.d)
mfile_c   = $(src_c:.c=.d)
mfile_all = $(mfile_cpp) $(mfile_c)

mfile_lib_mgr_cpp = $(lib_mgr_cpp:.cpp=.d)
mfile_lib_mgr_c   = $(lib_mgr_c:.c=.d)
mfile_lib_mgr     = $(mfile_lib_mgr_cpp) $(mfile_lib_mgr_c)

mfile_lib_cpp = $(lib_cpp:.cpp=.d)
mfile_lib_c   = $(lib_c:.c=.d)
mfile_lib     = $(mfile_lib_cpp) $(mfile_lib_c)

# ============================================================================================
# 個別 Makefile 的放置資料夾
mfile_dir = Makefiles
# .o檔的放置資料夾
build_dir = build
# 把所有的 .o檔 加上 build output資料夾的路徑
OBJS = $(patsubst %,$(build_dir)/%,$(obj_all))
OBJS += $(patsubst %,$(build_dir)/%,$(obj_lib_mgr))
OBJS += $(patsubst %,$(build_dir)/%,$(obj_lib))

# 為這些中間檔加入 Make 的"依賴目標"描掃路徑 才不會每次都重新產生
vpath %.d $(mfile_dir)
vpath %.o $(build_dir)

