
from Gui import *

def make_label():
	g.la(text = 'Thank you.')

g = Gui()
g.title('Gui Title')

button = g.bu(text = 'Press ME.')

label = g.la(text = 'LABEL')

button2 = g.bu(text = 'XYZ', command = make_label)

g.mainloop()