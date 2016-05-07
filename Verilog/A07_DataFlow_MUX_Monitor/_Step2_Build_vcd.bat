@ECHO OFF

if "%1" NEQ "--DisablePathExport" (
	CALL _env.bat
)

:iverilog  test_tb.v  test.v
vvp  test.vvp
