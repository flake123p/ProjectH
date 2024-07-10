#
# <targets ...> : <target-PATTERN> : <prereq-PATTERNS ...>
#     <commands>
#     ...
#

ext_mods_gen : Makefiles/_00_Ext_Mods.mak

Makefiles/_00_Ext_Mods.mak : mod_list.txt
	python  ../../_toolchain/list_to_make_var.py  mod_list.txt  Makefiles/_00_Ext_Mods.mak  ext_mod_list