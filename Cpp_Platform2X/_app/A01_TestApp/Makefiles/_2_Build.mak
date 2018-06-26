

# 載入公用的變數
include _0_Env.mak

# 載入個別 .o 檔的 Makefile
include $(mfile_all)
#include $(mfile_lib_mgr)
#include $(mfile_lib)

include _ModuleList.mak

.PHONY: Build_All Build_Mod
Build_All: Build_Mod aout.exe

Build_Mod: $(mod_list)

ifeq ($(OS),Windows_NT)
$(mod_list): %:
	echo $(base_mod_dir)/$@/mod_build_pure.bat --DisablePathExport --DisablePause
else
$(mod_list): %:
	echo TBD
endif

# 將所有 .o 檔與 Library Link 成執行檔
aout.exe: $(obj_all) $(lib_obj_with_path)
	$(CXX) $(INC_FOR_COMPILE) $(CXXFLAGS) $(obj_all_with_path) $(lib_obj_with_path) -o aout.exe $(EXT_LIB_FOR_LINK)
