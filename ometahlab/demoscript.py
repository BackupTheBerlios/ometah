#!/usr/bin/python

try:
    import psyco
    psyco.full()
except:
    pass

import ometahtest
import ometahstats

t = ometahtest.Test()
t.setArgs('-e 50')
t.setNbRuns(30)
t.start()

u = ometahtest.Test()
u.setArgs('-e 70')
u.setNbRuns(30)
u.start()


v = ometahtest.Test()
v.setArgs('-e 100')
v.setNbRuns(30)
v.start()


w = ometahtest.Test()
w.setArgs('-e 150')
w.setNbRuns(30)
w.start()


p = [ t.getPath(), u.getPath(), v.getPath(), w.getPath() ]
ometahstats.compare(p)
