rem
rem This file is for test use.
rem
@echo off

if "%2" NEQ "DisablePathSetting" (
	CALL _env.bat
)

python  mod_list_parser.py  mod_list.txt  mod//mod_tree.txt  mod//mod_all.txt

if "%1" NEQ "DisablePrint" (
	pause
)
