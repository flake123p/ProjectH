#
# <targets ...> : <target-PATTERN> : <prereq-PATTERNS ...>
#     <commands>
#     ...
#
# (1)mod_list.txt : default file in APP
# (2)mod_all.txt  : parsing all mode_list.txt in APP & MOD's
# (3)mod_all.mak : created by (2)
# ===== include mod_all.mak in next makefile
# (4)mod_all.txt : use mod_all.mak to check this file. Because step(2) is create/date-check by mod_list.txt in APP "only"
# (5)mod_all.mke : created by (4)

check_mod_all_by_APP :  mod/mod_all.txt

# (2)
# (3)
mod/mod_all.txt : mod_list.txt
	echo "update mod_all.txt/mak by APP..."
	python  ../../_toolchain/mod_list_parser.py  mod_list.txt  mod/mod_tree.txt  mod/mod_all.txt
	python  ../../_toolchain/list_to_make_var.py  mod/mod_all.txt  mod/mod_all.mak  mod_all
