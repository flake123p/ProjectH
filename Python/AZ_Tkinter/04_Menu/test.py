
from Gui import *

def set_color(color):
	mb.config(text=color)
	print(color)
	
g = Gui()
g.title('Gui Title')

g.la('Select a color:')
colors = ['red', 'green', 'blue']
mb = g.mb(text=colors[0]) # mb creates Menubutton

for color in colors:
	g.mi(mb, text=color, command=Callable(set_color, color))

g.mainloop()