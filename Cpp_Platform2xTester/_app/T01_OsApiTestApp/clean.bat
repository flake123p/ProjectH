@echo off

echo ====== Clean Application ... ======
del .\Makefiles\*.d              /F /Q
del .\Makefiles\_00_Ext_Mods.mak /F /Q
del .\mod\*.txt                  /F /Q
del .\mod\*.mak                  /F /Q
del .\build\*.o                  /F /Q
del aout.exe                     /F /Q

if "%1" NEQ "--DisablePause" (
	pause
)
