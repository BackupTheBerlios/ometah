#!/usr/bin/python

# try to import psyco to improve speed
try:
    import psyco
    psyco.full()
except:
    pass

# to make ometah tests ...
import ometahtest

# ... and generate stats
import ometahstats

print 'Benchmark in progress...go take a sleep, come back in several hours, if you see black smoke near your CPU, do not worry... ;)\n'

# set all tests, set ometah path ( default is '../ometah/ometah' too)
t = ometahtest.Test()
t.setArgs('-e 50 -p Sphere')
t.setNbRuns(50)
t.setOmetahPath('../ometah/ometah')
t.start()

u = ometahtest.Test()
u.setArgs('-e 100 -p Sphere -m CEDA')
u.setNbRuns(50)
u.setOmetahPath('../ometah/ometah')
u.start()

v = ometahtest.Test()
v.setArgs('-e 200 -p Sphere')
v.setNbRuns(50)
v.start()

w = ometahtest.Test()
w.setArgs('-e 250 -i 150 -p Sphere')
w.setNbRuns(50)
w.setOmetahPath('../ometah/ometah')
w.start()

x = ometahtest.Test()
x.setArgs('-e 300 -i 200 -p Sphere')
x.setNbRuns(50)
x.setOmetahPath('../ometah/ometah')
x.start()

y = ometahtest.Test()
y.setArgs('-e 1000 -i 1000 -p Sphere')
y.setNbRuns(50)
y.start()

# report and plot comparison
p = [ t.getPath(), u.getPath(), v.getPath(), w.getPath(), x.getPath()]
ometahstats.compare(p)
