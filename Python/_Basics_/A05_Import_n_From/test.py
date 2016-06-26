
import math

print('>>> import math')
print('math        = ', math)
print('math.pi     = ', math.pi)
print('math.sin(1) = ', math.sin(1))

from math import pi
print('>>> from math import pi')
print('pi = ', pi, ' (Don\'t need to use \'math.pi\')')

from math import *
print('>>> from math import *')
print('sin(1) = ', sin(1), ' (Don\'t need to use \'math.sin(1)\')')
