
from Gui import *

g = Gui()
g.title('Gui Title')

# 1ST kind of TEXT
entry = g.en(text='Default text.')

# 2ND kind of TEXT
text = g.te(width=100, height=5)

text.insert(END, 'abc')
text.insert(1.1, 'xyz') # row.column

# Get function:
# text.get(0.0, END)
# text.delete(1.2, END)

g.mainloop()