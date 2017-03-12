@ECHO OFF

call build.bat %1 %2

IF %result1% EQU 0 (
IF %result2% EQU 0 (
IF EXIST aout.exe (
	goto EXECUTE_AOUT
)
)
)
:ELSE
echo NOT execute aout.exe ...
goto EXIT_NORMAL

rem
rem  Within a block statement (a parenthesised series of statements), the entire block is parsed and then executed.
rem  So move aout.exe out to read ERRORLEVEL correctly
rem
:EXECUTE_AOUT
if "%3" NEQ "--redirect" (
	echo "aout.exe"
	aout.exe
) ELSE (
	echo "aout.exe>123.txt"
	aout.exe>123.txt
)

echo ====== aout.exe Done!! Errorlevel = %ERRORLEVEL% ======

if "%2" NEQ "--DisablePause" (
	pause
)
	
:EXIT_NORMAL
