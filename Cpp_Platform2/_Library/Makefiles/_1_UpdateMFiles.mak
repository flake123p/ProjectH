

# 載入公用變數
include _0_Env.mak

# 為每個 .cpp .c 檔 製作個別的 Makefile
.PHONY: Update_MakeFiles
Update_MakeFiles: $(mfile_lib_mgr_cpp) $(mfile_lib_mgr_c) $(mfile_lib_cpp) $(mfile_lib_c)

$(mfile_lib_mgr_cpp): %.d: $(LIB_MGR_DIR)/%.cpp
	$(CXX) $(INC) -MM $(CXXFLAGS) $< > $(mfile_dir)/$@
	echo $(MyTab)$(CXX) $(INC) -c $(CXXFLAGS) $< -o $(build_dir)/$(@:.d=.o) >> $(mfile_dir)/$@

$(mfile_lib_mgr_c): %.d: $(LIB_MGR_DIR)/%.c
	$(CXX) $(INC) -MM $(CXXFLAGS) $< > $(mfile_dir)/$@
	echo $(MyTab)$(CXX) $(INC) -c $(CXXFLAGS) $< -o $(build_dir)/$(@:.d=.o) >> $(mfile_dir)/$@

$(mfile_lib_cpp): %.d: $(LIB_DIR)/%.cpp
	$(CXX) $(INC) -MM $(CXXFLAGS) $< > $(mfile_dir)/$@
	echo $(MyTab)$(CXX) $(INC) -c $(CXXFLAGS) $< -o $(build_dir)/$(@:.d=.o) >> $(mfile_dir)/$@

$(mfile_lib_c): %.d: $(LIB_DIR)/%.c
	$(CXX) $(INC) -MM $(CXXFLAGS) $< > $(mfile_dir)/$@
	echo $(MyTab)$(CXX) $(INC) -c $(CXXFLAGS) $< -o $(build_dir)/$(@:.d=.o) >> $(mfile_dir)/$@
