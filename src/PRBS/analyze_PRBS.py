#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jan  8 17:51:28 2024

@author: bob
"""

import numpy as np
from matplotlib import pyplot as plt

def getFFT(signal, Ndownsample, Ts):
    j0 = int(1/Ts)
    L = 2047 * Ndownsample
    Naver = int((len(signal)-j0)/L)
    # print( 'Naver =' , Naver )
    SIGNAL = np.fft.fft(signal[j0:j0+L*Naver], axis=0)
    f = np.fft.fftfreq(L*Naver, Ts)
    SIGNAL = SIGNAL[f > 0]
    SIGNAL = 2*SIGNAL[Naver-1::Naver]
    f = f[f > 0]
    f = f[Naver-1::Naver]
    SIGNAL = SIGNAL[f < 1/(2*Ts*Ndownsample)]
    f = f[f < 1/(2*Ts*Ndownsample)]
    return SIGNAL, f
  
def bode( H, f, name='Data', title='bode plot'):
    mag = np.abs(H)
    phase = np.angle(H)
    ax1 = plt.subplot(2, 1, 1)
#    plt.loglog(f, mag)    # Bode magnitude plot
    plt.semilogx(f, 20*np.log10(mag), label=name)    # Bode magnitude plot
    plt.grid(1, 'both', 'both')
    plt.ylabel('Magnitude [dB]')
    plt.title(title)
    plt.legend()
#    ax1.xaxis.set_minor_formatter(ticker.ScalarFormatter())
    ax2 = plt.subplot(2, 1, 2, sharex=ax1)
    plt.plot(f, phase * 180 / np.pi)  # Bode phase plot
    plt.grid(1, 'both')
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Phase [deg]')
    plt.ylim(-180, 180)
    # ax2.yaxis.set_major_locator(plt.LinearLocator(5))
    ax2.yaxis.set_major_locator(plt.MaxNLocator(5, steps=[1,2,2.5,5,9,10]))
    plt.tight_layout()
    plt.show()

def nyquist( H ,f  , name = '' , title = 'nyquist plot'):
    plt.plot( np.real( H ) , np.imag( H ) , label = name)    # Bode magnitude plot
    tmp = 0.5 * np.exp( -1j * np.arange( 0 , 2*np.pi , 0.01) )
    plt.plot( np.real( tmp ) , np.imag( tmp ) , label = '6 dB')    # Bode magnitude plot
    plt.grid( 1 , 'both')
    plt.title(title)
    plt.legend()
    plt.xlabel('Real')
    plt.ylabel('Imaginary')
    plt.xlim( -2 , 2)
    plt.ylim( -2 , 2)
    plt.tight_layout()
    plt.show()
      
filename = "screenlog.0_prbs_250k_ds10_2"

data = np.loadtxt(filename,delimiter=',')

timesteps = np.diff(data[:,0])*1e-6
Ts = np.mean(timesteps)

plt.figure()
plt.title("timesteps")
plt.plot(timesteps)
plt.plot([0, data.shape[0]],[Ts, Ts])
plt.legend(['Data','Mean'])
plt.show()

S, S_f = getFFT(data[:,1], 10, Ts)
G, G_f = getFFT(data[:,2], 10, Ts)

plt.figure()
bode(S, S_f, name="Input Signal")
bode(G, G_f, name="Response")
plt.show()

plt.figure()
bode(G/S, S_f, name="Encoder Frequency Response")
plt.show()

plt.figure()
nyquist(G/S, S_f, name="Encoder Frequency Response")
# s1 = signal.ZerosPolesGain([200,  301, 371, 555], [0, 305, 344, 498], [5])
# w,H = signal.freqresp(s1,w=G_f*2*np.pi)

# f = w/(2*np.pi)
# bode(H, f, name="Est Response")
