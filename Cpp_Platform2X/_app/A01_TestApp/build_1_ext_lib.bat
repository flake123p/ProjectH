
rem Define paths
SET LIB_PATH=..\..\..\Cpp_Platform2\_Library
SET LOCAL_PATH=%CD%

CD %LIB_PATH%
CALL build_1_local_lib.bat
CD %LOCAL_PATH%

set build_1_ext_lib_rc=%build_1_local_lib_rc%
