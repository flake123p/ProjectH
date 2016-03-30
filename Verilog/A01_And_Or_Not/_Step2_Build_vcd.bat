@ECHO OFF
CALL _env.bat

:iverilog  test_tb.v  test.v
vvp  test.vvp
