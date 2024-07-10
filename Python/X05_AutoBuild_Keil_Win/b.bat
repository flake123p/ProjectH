@ECHO OFF

:LOOP_START
call a01_update_cur_rev.bat
call a02_update_rev_list.bat
call a03_check_update.bat

IF EXIST log\check_update.txt (
	call a04_build_code.bat
	GOTO LOOP_START
)