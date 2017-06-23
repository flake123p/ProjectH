@ECHO OFF

SET DST_FOLDER=%CD%

FOR /R %DST_FOLDER% %%F IN (*) DO (
	IF %%~xF EQU .c (
		ECHO %%F
	)
	IF %%~xF EQU .h (
		ECHO %%F
	)
)