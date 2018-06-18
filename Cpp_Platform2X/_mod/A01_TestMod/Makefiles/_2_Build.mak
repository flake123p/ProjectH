

# 載入公用的變數
include _0_Env.mak

# 載入個別 .o 檔的 Makefile
#include $(mfile_all)
include $(mfile_lib_mgr)
include $(mfile_lib)

.PHONY: Build_All
Build_All: $(obj_lib_mgr) $(obj_lib)
	
