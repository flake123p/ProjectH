

# 載入公用變數
include _0_Env.mak

# 為每個 .cpp .c 檔 製作個別的 Makefile
.PHONY: Update_MakeFiles
Update_MakeFiles: $(mfile_cpp) $(mfile_c)

$(mfile_cpp): %.d: $(src_dir)/%.cpp
	$(CXX) $(INC_FOR_COMPILE) -MM $(CXXFLAGS) $< > $(mfile_dir)/$@
	echo $(MyTab)$(CXX) $(INC_FOR_COMPILE) -c $(CXXFLAGS) $< -o $(build_dir)/$(@:.d=.o) >> $(mfile_dir)/$@

$(mfile_c): %.d: $(src_dir)/%.c
	$(CXX) $(INC_FOR_COMPILE) -MM $(CXXFLAGS) $< > $(mfile_dir)/$@
	echo $(MyTab)$(CXX) $(INC_FOR_COMPILE) -c $(CXXFLAGS) $< -o $(build_dir)/$(@:.d=.o) >> $(mfile_dir)/$@
