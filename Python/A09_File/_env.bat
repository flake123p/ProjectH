@ECHO OFF

@SET currentDir=%CD%

REM
REM Search env_win.bat
REM
SET loopMax=6
SET loopCtr=0
:LOOP_FIND_ENV_WIN_START
	REM 
	REM To upper directory
	REM
	CD ../

	IF EXIST env_win.bat (
		echo Find "env_win.bat" in:  %CD%
		GOTO LOOP_FIND_ENV_WIN_END
	)
	
	SET /a loopCtr=%loopCtr%+1
	REM
	REM EQU - equal
	REM NEQ - not equal
	REM LSS - less than
	REM LEQ - less than or equal
	REM GTR - greater than
	REM GEQ - greater than or equal
	REM
	IF %loopCtr% GEQ %loopMax% (
		ECHO Can't find env_win.bat, exit now!!
		ECHO Current directory = %CD%
		PAUSE
		EXIT
	)

	GOTO LOOP_FIND_ENV_WIN_START
:LOOP_FIND_ENV_WIN_END:

CALL env_win.bat
CD %currentDir%
