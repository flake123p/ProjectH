

# 載入公用的變數
include _0_Env.mak

# 載入個別 .o 檔的 Makefile
include $(mfile_all)
#include $(mfile_lib_mgr)
#include $(mfile_lib)

.PHONY: Build_All

Build_All: aout.exe

# 將所有 .o 檔與 Library Link 成執行檔
aout.exe: $(obj_all) $(lib_archive_full_name) $(all_mod_all_level_archives)
	$(CXX) $(INC_FOR_COMPILE) $(CXXFLAGS) $(obj_all_with_path) $(all_mod_all_level_archives) $(lib_archive_full_name) -o aout.exe $(EXT_LIB_FOR_LINK)
