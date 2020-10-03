

#
include _0_Env.mak

# 載入個別 .o 檔的 Makefile
include $(mfile_all)


.PHONY: Build_All Build_All_Obj Create_Archive

Build_All: Build_All_Obj Create_Archive

Build_All_Obj: $(obj_all)

Create_Archive: $(local_mod).a

$(local_mod).a: $(obj_all)
	ar -r $(archive_dir)/$(local_mod).a $(obj_all_with_path)
