pkg load control
hold on

H = dlmread("OL_4.csv")
w = csvread("OL_f_4.csv")

w=w*pi/180

ol_sys = frd(H,w);

[sys,n] = fitfrd(ol_sys,20)

[mag, ph,freq] = bode(sys,w);

figure()
hold on
loglog(w*180/pi,abs(H))
ax = gca()
loglog(ax,freq*180/pi,mag)

figure()
hold on
h=semilogx(w,angle(H)*180/pi)
semilogx(freq,ph)

figure()
pzmap(sys)

