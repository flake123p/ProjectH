#
# <targets ...> : <target-PATTERN> : <prereq-PATTERNS ...>
#     <commands>
#     ...
#

all : mod/mod_all.txt

mod/mod_all.txt : mod_list.txt
	python  mod_list_parser.py  mod_list.txt  mod//mod_tree.txt  mod//mod_all.txt
