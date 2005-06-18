#!/usr/bin/python

try:
    import psyco
    psyco.full()
except:
    pass

import ometah_test
import ometah_stats

t = ometah_test.Test()
t.setArgs('-e 50 -p Ackley')
t.setNbRuns(10)
t.start()

u = ometah_test.Test()
u.setArgs('-e 70 -p Ackley')
u.setNbRuns(10)
u.start()


v = ometah_test.Test()
v.setArgs('-e 100 -p Ackley')
v.setNbRuns(10)
v.start()


p = [ t.getPath(), u.getPath(), v.getPath() ]
ometah_stats.compare(p)
