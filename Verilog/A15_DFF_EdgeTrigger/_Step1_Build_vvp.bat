@ECHO OFF

if "%1" NEQ "--DisablePathExport" (
	CALL _env.bat
)

iverilog  -o  test.vvp  test_tb.v  test.v

