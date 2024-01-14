#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jan  8 17:51:28 2024

@author: bob
very useful slides: http://publications.pvandenhof.nl/discsysid2020/DISC-2020-7-ClosedLoop.pdf

"""

import numpy as np
from matplotlib import pyplot as plt
import scipy.signal as signal
import scipy.optimize as optimize

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
  
def bode( H, f, name='Data', title='bode plot', show=True):
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
    if show:
        plt.show()

def nyquist( H ,f  , name = '' , title = 'nyquist plot'):
    plt.plot( np.real( H ) , np.imag( H ) , label = name)    # Bode magnitude plot
    tmp = 0.5 * np.exp( -1j * np.arange( 0 , 2*np.pi , 0.01) )-1
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
      
# filename = "screenlog.0_prbs_250k_ds10_4"
filename = "screenlog.0_cl_5"

data = np.loadtxt(filename,delimiter=',')

timesteps = np.diff(data[:,0])*1e-6
Ts = np.mean(timesteps)

plt.figure()
plt.title("timesteps")
plt.plot(timesteps)
plt.plot([0, data.shape[0]],[Ts, Ts])
plt.legend(['Data','Mean'])
plt.show()

U, f = getFFT(data[:,2], 10, Ts)
ERR, _ = getFFT(data[:,1] - data[:,3], 10, Ts)
Y, _ = getFFT(data[:,3], 10, Ts)
R, _ = getFFT(data[:,1], 10, Ts)

C = U / ERR
G = Y/U
S = 1/(1+C*G)

plt.figure()
bode(C, f, name="C", show=False)
bode(G, f, name="G", show=False)
plt.show()

plt.figure()
bode(Y/R, f, name="y/r, Closed Loop TF")
plt.show()

plt.figure()
bode(S, f, name="Sensitivity")
plt.show()

plt.figure()
nyquist(Y/R, f, name="Encoder Frequency Response")

#%%
'''
Take a shot at writing the transfer function of a controller and simulating 
the frequency reponse of the closed loop system
'''
s1 = signal.lti([10*2*np.pi,10*2*np.pi,10*2*np.pi,10*2*np.pi], [11*2*np.pi,11*2*np.pi,11*2*np.pi,11*2*np.pi], 1)
w, H = signal.freqresp(s1, w=f*2*np.pi)

f = w/(2*np.pi)
bode(H, f, title='Controller', name="Closed loop estimate", show=False)

bode(H*G, f, title='Closed Loop')
# nyquist(H*G,f)

'''
Export data to Octave in order to fit transfer function to system.
This is necessary because the released version of Python control does not
currently implement the SLICOT function sb10yd, but Octave (forge) control does.
'''
# if 0:
#     ol_filt = signal.medfilt(np.abs(OL),7) * np.exp(1j*signal.medfilt(np.angle(OL),7))
#     bode(ol_filt[7:],S_f[7:], name="Smoothed data exported for Octave")
    
#     np.savetxt("OL_f_4.csv", S_f[7:], delimiter=",")
#     with open("OL_4.csv","w") as f:
#         for num in ol_filt[7:]:
#             f.write(f"{num}".strip(")").lstrip("(")+"\n")
#%%

# def model(x):
#     # zeros = [x[1]+x[2]*1j,
#     #           x[3]+x[4]*1j,
#     #           x[5]+x[6]*1j,
#     #           x[7]+x[8]*1j,]
#     # poles = [x[9]+x[10]*1j, 
#     #           x[11]+x[12]*1j,
#     #           x[13]+x[14]*1j,
#     #           x[15]+x[16]*1j,]
#     zeros = [x[1]+x[2]*1j,
#               x[5]+x[6]*1j,
#               ]
#     poles = [x[3]+x[4]*1j,
#               x[7]+x[8]*1j]
#     s1 = signal.lti(zeros,poles,x[0])
#     return s1

# def err(x):
#     s1 = model(x)
#     w,H = signal.freqresp(s1,w=G_f*2*np.pi) 
#     # err = np.sum(np.abs(H-OL)**2) + np.sum(np.angle(H-OL)**2)
#     err = np.sqrt(np.abs(OL-H)**2 + np.angle(OL-H)**2 )
#     return np.sum(err)**2
# iteration=0
# def cb(x,f,c):
#     global iteration
#     print(f"{iteration}: fun = {f}")
#     iteration+=1
# def cb_de(xk,*args,**kwargs):
#     global iteration
#     print(f"{iteration}: fun = {err(xk)}")
#     iteration+=1
# # guess = [0.1,
# #           0.1, 0,
# #           0.1, 0,
# #           100, -300,
# #           100, 300,
          
# #           0,0,
# #           0,0,
# #           0, 0,
# #           0.1,0 ]
# # guess = [ 4.37453656e+00, -5.93446510e+02,  1.31247638e+03,  2.88957799e+01,
# #        -4.91518630e+00,  1.63537086e+00, -1.03921041e+03, -3.01382360e+01,
# #         8.11940982e+03, -2.12708610e+01, -1.27987312e+06, -1.71440458e+01,
# #        -3.84411657e+01,  4.37309360e-01, -1.99669018e-01, -3.75105627e+02,
# #         1.35774392e+03]
# guess = [-2.30232633e-02,  1.26880456e+02, -1.00541050e+03, -3.68090084e+02,
#         1.34730519e+03, -5.82724791e+02,  1.31597688e+03, -1.74811659e+01,
#         3.83196406e+00]
# bounds = [(-1e6, 1e6) for q in range(9)]
# # ret = optimize.basinhopping(err, 
# #                             guess, 
# #                             disp=True, 
# #                             minimizer_kwargs={'method':'Nelder-Mead'}, 
# #                             T=100, 
# #                             stepsize=1000)
# # ret = optimize.dual_annealing(err,
# #                               bounds, 
# #                               minimizer_kwargs={'method':'Nelder-Mead'}, 
# #                               callback=cb, 
# #                               maxiter=5000, 
# #                               initial_temp=7000,
# #                               restart_temp_ratio=1e-5)
# ret = optimize.differential_evolution(err, 
#                                       bounds,
#                                       # x0=guess,
#                                       workers=6,
#                                       maxiter=10000,
#                                       callback=cb_de)
# x = ret.x
# # x = guess
# s1 = model(x)
# w,H = signal.freqresp(s1,w=G_f*2*np.pi) 

# f = w/(2*np.pi) 
# bode(H, f, name="Est Response", show=False)

# bode(OL, f, title='Open Loop', name="Measured Response")


