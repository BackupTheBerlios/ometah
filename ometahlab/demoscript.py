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


# set all tests, set ometah path ( default is '../ometah/ometah' too)
t = ometahtest.Test()
t.setArgs('-e 50 -p Sphere')
t.setNbRuns(50)
t.setOmetahPath('../ometah/ometah')
t.start()

u = ometahtest.Test()
u.setArgs('-e 70 -p Sphere -m CEDA')
u.setNbRuns(50)
u.setOmetahPath('../ometah/ometah')
u.start()

v = ometahtest.Test()
v.setArgs('-e 100 -p Sphere')
v.setNbRuns(50)
v.start()

w = ometahtest.Test()
w.setArgs('-e 150 -p Sphere')
w.setNbRuns(50)
w.setOmetahPath('../ometah/ometah')
w.start()

x = ometahtest.Test()
x.setArgs('-e 200 -p Sphere -r 13')
x.setNbRuns(50)
x.setOmetahPath('../ometah/ometah')
x.start()

# report and plot comparison
p = [ t.getPath(), u.getPath(), v.getPath(), w.getPath(), x.getPath()]
ometahstats.compare(p)
