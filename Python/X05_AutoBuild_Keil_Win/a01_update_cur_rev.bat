@ECHO OFF

SET defaultDir=%CD%

rem
rem Use svn command
rem
CD ../
svn info>AUTO/log/cur_rev_temp.txt
CD %defaultDir%

python a01_update_cur_rev.py