@ECHO OFF

if "%1" NEQ "--DisablePathExport" (
	CALL _env.bat
)

gtkwave simple.vcd

