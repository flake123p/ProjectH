
xtarget: Makefiles/var.mak

# Usage: var_maker.py  <input file>  <output file>
Makefiles/var.mak: var.txt
	python  var_maker.py  var.txt  Makefiles/var.mak