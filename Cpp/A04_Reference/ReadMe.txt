Batch Files/Shell Script簡介:
build.bat
build.sh
	build code, 產生 aout.exe

clean.bat
clean.sh
	清掉中介檔(.o檔, makefiles)與aout.exe

run.bat
run.sh
	執行build.bat 再執行aout.exe 最後顯示程式回傳值

_cmd.bat
	開啟command line，執行環境變數設定

b.bat
	快速版的run.bat(沒有pause)，配合_cmd.bat使用

===========================================================
資料夾簡介:
build
	.o檔的放置處，一般來說程序員不用管這裡

include
	.h .hpp檔的放置處，程序員要管理這裡

Makefiles
	各別.c .cpp的makefile，自動產生，一般來說程序員不用管這裡

src
	.c .cpp檔的放置處，程序員要管理這裡

===========================================================
為什麼這麼複雜:
	因為此系統詳細記錄關聯性，讓重build的時候build最少的code