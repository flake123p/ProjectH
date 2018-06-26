@echo off

echo ====== Clean Library ... ======
del .\Makefiles\*.d /F /Q
del .\build\*.o     /F /Q
del .\archive\*.a   /F /Q

if "%1" NEQ "--DisablePause" (
	pause
)
