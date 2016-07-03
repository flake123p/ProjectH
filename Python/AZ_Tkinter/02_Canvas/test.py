
from Gui import *

g = Gui()
g.title('Gui Title')

canvas = g.ca(width=600, height=500)

canvas.config(bg='black') # white black red green blue cyan yellow magenta

# first two args = coordinate, third args = radius
item = canvas.circle([0,0], 100, fill = 'red')
item.config(fill='yellow', outline='white', width=10)

canvas.rectangle([[0,0], [200, 100]], fill='blue', outline='orange', width=10)

canvas.oval([[0,0], [200, 100]], fill='blue', outline='red', width=10)

canvas.line([[0,100], [100, 200]], fill='blue', width=10)

canvas.polygon([[0,0], [-100,-200], [-200, -100]], fill='yellow', outline='cyan', width=10)

g.mainloop()