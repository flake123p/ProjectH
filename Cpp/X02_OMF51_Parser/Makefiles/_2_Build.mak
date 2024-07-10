

# 載入公用的變數
include _0_Env.mak

# 載入個別 .o 檔的 Makefile
include $(mfile_all)
include $(mfile_lib_mgr)
include $(mfile_lib)

.PHONY: Build_All
Build_All: aout.exe

# 將所有 .o 檔與 Library Link 成執行檔
aout.exe: $(obj_all) $(obj_lib_mgr) $(obj_lib)
	$(CXX) $(INC) $(CXXFLAGS) $(OBJS) -o aout.exe $(LIB)
