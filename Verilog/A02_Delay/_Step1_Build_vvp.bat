@ECHO OFF
CALL _env.bat

iverilog  -o  test.vvp  test_tb.v  test.v

