@echo off

@CALL _env.bat

python  mod_list_parser.py  mod_list.txt  mod_tree.txt  mod_all.txt

pause
