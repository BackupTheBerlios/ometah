#!/usr/bin/python

try:
    import psyco
    psyco.full()
except:
    pass

import ometahtest
import ometahstats

t = ometahtest.Test()
t.setArgs('-e 30 -p Ackley -d 2')
t.setNbRuns(15)
t.start()

v = ometahtest.Test()
v.setArgs('-e 60  -p Ackley -d 2')
v.setNbRuns(15)
v.start()

p = [ t.getPath(), v.getPath() ]
ometahstats.compare(p)
