#!/usr/bin/python

try:
    import psyco
    psyco.full()
except:
    pass

import ometahtest
import ometahstats

t = ometahtest.Test()
t.setArgs('-e 32 -p Ackley -d 2')
t.setNbRuns(30)
t.start()

v = ometahtest.Test()
v.setArgs('-e 64  -p Ackley -d 2')
v.setNbRuns(30)
v.start()

u = ometahtest.Test()
u.setArgs('-e 128 -i 128 -p Ackley -d 2')
u.setNbRuns(30)
u.start()

p = [ t.getPath(), v.getPath(), u.getPath() ]
ometahstats.compare(p)
