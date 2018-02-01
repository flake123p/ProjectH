@ECHO OFF

IF EXIST log\check_update.txt (
	DEL /F /Q log\check_update.txt
)

python a03_check_update.py


IF EXIST log\check_update.txt (
	for /F "tokens=*" %%A in (log\check_update.txt) do SET nextRev=%%A
) ELSE (
	ECHO nothing to update ...
	GOTO END
)

rem
rem Start update
rem
SET defaultDir=%CD%

CD ../../../
svn revert --quiet --non-interactive -R %CD%
ECHO start update to rev: %nextRev%
svn update -r %nextRev%
CD %defaultDir%

:END