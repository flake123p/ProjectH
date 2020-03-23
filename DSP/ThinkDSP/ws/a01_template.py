"""This file contains code for use with "Think Stats",
by Allen B. Downey, available from greenteapress.com

Copyright 2014 Allen B. Downey
License: GNU GPLv3 http://www.gnu.org/licenses/gpl.html
"""

from __future__ import print_function, division

import unittest
import thinkdsp
import matplotlib
import matplotlib.pyplot as pyplot

import numpy as np



if __name__ == "__main__":
    #unittest.main()
	cos_sig = thinkdsp.CosSignal(freq=440, amp=1.0, offset=0)
	sin_sig = thinkdsp.SinSignal(freq=880, amp=0.5, offset=0)
	mix = sin_sig + cos_sig
	#mix = cos_sig
	wave = mix.make_wave(duration=0.007, start=0, framerate=11025)
	wave.plot()
	#pyplot.show()
	wave1 = cos_sig.make_wave(duration=0.007, start=0, framerate=11025)
	wave1.plot()
	wave2 = sin_sig.make_wave(duration=0.007, start=0, framerate=11025)
	wave2.plot()
	pyplot.show()
