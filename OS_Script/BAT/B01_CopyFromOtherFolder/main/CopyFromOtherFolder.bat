@ECHO OFF

SET SRC_FOLDER="..\trunk"
SET DST_FOLDER=%CD%

FOR /R %DST_FOLDER% %%F IN (*) DO (
	FOR /R %SRC_FOLDER% %%G IN (*) DO (
		IF %%~nxF EQU %%~nxG (
			XCOPY %%G %%F /Y
		)
	)
)

rem PAUSE