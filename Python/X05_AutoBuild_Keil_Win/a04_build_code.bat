@ECHO OFF

IF EXIST log\check_update.txt (
	ECHO start build code
) ELSE (
	ECHO nothing to build ...
	GOTO END
)

for /F "tokens=*" %%A in (log\check_update.txt) do SET curRev=%%A
ECHO start build code in rev: %curRev%

SET defaultDir=%CD%
CD ../../../BT_Controller_Project/ROM2/trunk
SET workDir=%CD%

rem
rem File naming rule:
rem   1R  = ROM
rem   2RD = ROM DEBUG
rem   3D  = ROM Develog Mode
rem   4DD = DOM DEBUG Develop Mode
rem
DEL Build\BT5511_HEX\*.* /F /Q
C:\Keil_v5\UV4\UV4.exe -b BT5511.UVproj -t"BT5511_ROM"
CD Build/BT5511_HEX
FOR /R %%x IN (*) DO (
	ECHO %%x
	ECHO %defaultDir%\hex\%curRev%_1R_%%~nxx
	COPY /Y %%x %defaultDir%\hex\%curRev%_1R_%%~nxx
)
CD %workDir%

DEL Build\BT5511_HEX\*.* /F /Q
C:\Keil_v5\UV4\UV4.exe -b BT5511.UVproj -t"BT5511_ROM_DEBUG"
CD Build/BT5511_HEX
FOR /R %%x IN (*) DO (
	ECHO %%x
	ECHO %defaultDir%\hex\%curRev%_2RD_%%~nxx
	COPY /Y %%x %defaultDir%\hex\%curRev%_2RD_%%~nxx
)
CD %workDir%

DEL Build\BT5511_HEX\*.* /F /Q
C:\Keil_v5\UV4\UV4.exe -b BT5511.UVproj -t"BT5511_ROM_DEV_MODE"
CD Build/BT5511_HEX
FOR /R %%x IN (*) DO (
	ECHO %%x
	ECHO %defaultDir%\hex\%curRev%_3D_%%~nxx
	COPY /Y %%x %defaultDir%\hex\%curRev%_3D_%%~nxx
)
CD %workDir%

DEL Build\BT5511_HEX\*.* /F /Q
C:\Keil_v5\UV4\UV4.exe -b BT5511.UVproj -t"BT5511_ROM_DEV_MODE_DEBUG"
CD Build/BT5511_HEX
FOR /R %%x IN (*) DO (
	ECHO %%x
	ECHO %defaultDir%\hex\%curRev%_4DD_%%~nxx
	COPY /Y %%x %defaultDir%\hex\%curRev%_4DD_%%~nxx
)
CD %workDir%

CD %defaultDir%

:END