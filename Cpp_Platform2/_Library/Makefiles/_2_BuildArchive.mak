

# 載入公用的變數
include _0_Env.mak

# 載入個別 .o 檔的 Makefile
#include $(mfile_all)
include $(mfile_lib_mgr)
include $(mfile_lib)

.PHONY: Build_All Build_All_Obj Create_Archive

Build_All: Build_All_Obj Create_Archive

Build_All_Obj: $(obj_lib_mgr) $(obj_lib)

Create_Archive: LibPF2.a

LibPF2.a: $(obj_lib_mgr) $(obj_lib)
	ar -r $(archive_dir)/LibPF2.a $(OBJS)
