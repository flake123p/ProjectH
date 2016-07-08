#!/usr/bin/env python

import sys
from PyQt4.QtGui import *

app = QApplication(sys.argv)

widget = QWidget()
widget.resize(200, 150)
widget.move(500, 400)
widget.setWindowTitle("Hello World")
widget.show()

app.exec_()
