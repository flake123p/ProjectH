#
# <targets ...> : <target-PATTERN> : <prereq-PATTERNS ...>
#     <commands>
#     ...
#
# (1)mod_list.txt : default file in APP
# (2)mod_all.txt  : parsing all mode_list.txt in APP & MOD's
# (3)mod_all.mak : created by (2)
# ===== include mod_all.mak in next makefile
# (4)mod_all.txt : use mod_all.mak to check this file. Bucase step(2) is create/date-check by mod_list.txt in APP "only"
# (5)mod_all.mke : created by (4)

include mod/mod_all.mak
mod_base_path = ../../mod/
mod_base_file = mod_list.txt
temp = $(patsubst %,$(mod_base_path)%,$(mod_all))
mod_all_list = $(patsubst %,%/$(mod_base_file),$(temp))

ifeq ($(OS),Windows_NT)
curr_os = WIN
mod_build_file = build_all_mod.bat
mod_clean_file = clean_all_mod.bat
else
curr_os = LINUX
mod_build_file = build_all_mod.sh
mod_clean_file = clean_all_mod.sh
endif

check_mod_all_by_MODs :  mod/mod_all.txt  $(mod_build_file)

# (4) - prerequisites changed
# (5) - no chang, just re-check
mod/mod_all.txt : $(mod_all_list)
	echo "update mod_all.txt/mak by MOD's..."
	python  ../../_toolchain/mod_list_parser.py  mod_list.txt  mod/mod_tree.txt  mod/mod_all.txt
	python  ../../_toolchain/list_to_make_var.py  mod/mod_all.txt  mod/mod_all.mak  mod_all

$(mod_build_file) : mod//mod_all.txt
	python  ../../_toolchain/list_to_build_script.py  mod/mod_all.txt  Makefiles/OFS_From_App.mak $(mod_build_file)  $(mod_clean_file)  $(curr_os)
