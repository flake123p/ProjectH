@ECHO OFF

SET defaultDir=%CD%

rem
rem Use svn command
rem
CD ../
svn log -r BASE:HEAD>AUTO/log/rev_list_temp.txt
CD %defaultDir%

python a02_update_rev_list.py