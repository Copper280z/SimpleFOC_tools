#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Jan 11 10:23:29 2024

@author: bob
"""
import numpy as np
from matplotlib import pyplot as plt

class PRBS():
    def __init__(self):
        self.a = 0x02
class wiki(PRBS):
    length = 32767
    def __call__(self):
        newbit = (((self.a >> 14) ^ (self.a >> 13)) & 1)
        self.a = ((self.a << 1) | newbit) & 0x7fff
        return newbit

class elwin(PRBS):
    length = 2047
    def __call__(self):
        newbit = (((self.a >> 5) ^ (self.a >> 7)) & 1)
        self.a = ((self.a >> 1) | newbit<<15)
        return newbit

prbs_gen = elwin()
prbs_gen.a = 7654
prbs = []
for i in range(prbs_gen.length):
    a = prbs_gen()
    prbs.append(a)
    print(f'{a}')

prbs = np.asarray(prbs)

print(f'mean of generated array: {np.mean(prbs-0.5)}')

ft = np.fft.fft(prbs)
plt.plot(20*np.log10(np.abs(ft[1:])))
plt.show()
