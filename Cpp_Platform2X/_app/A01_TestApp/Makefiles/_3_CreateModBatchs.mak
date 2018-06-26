
include _0_Env.mak

ifeq ($(OS),Windows_NT)
build_3_ext_mods_batchs.bat: $(mfile_dir)/$(mod_make_file)
	@ECHO ff $(ext_mod_list)> build_3_ext_mods_batchs.bat
else
build_3_ext_mod_batchs.sh: $(mfile_dir)/$(mod_make_file)
	echo TBD
endif
