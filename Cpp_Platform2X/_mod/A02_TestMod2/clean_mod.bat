@echo off


for %%I in (.) do set CurrDirName=%%~nxI
echo ====== Clean Module [%CurrDirName%] ======
del .\Makefiles\*.d              /F /Q
del .\Makefiles\_00_Ext_Mods.mak /F /Q
del .\build\*.o                  /F /Q
del .\archive\*.a                /F /Q

if "%1" NEQ "--DisablePause" (
	pause
)
