@ECHO OFF

rem "C:\Program Files (x86)\Subversion\bin\svn.exe" status %CD% -q

rem
rem tokens 1 is always M
rem tokens 2 is modified file name
rem
FOR /F "tokens=1,2 delims= " %%i IN ('"C:\Program Files (x86)\Subversion\bin\svn.exe"  status %CD% -q') DO (
	uncrustify.exe -c defaults.cfg -f %%j -o .\_Beautifier\%%~nxj
	rem @echo %%i %%j
	@echo start "" "C:\Program Files (x86)\WinMerge\WinMergeU.exe" %%j %%~nxj>.\_Beautifier\%%~nxj.bat
	rem @echo %%i %%~pj
	rem @echo %%i %%~nj
	rem @echo %%i %%~xj
)

pause