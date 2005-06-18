#!/usr/bin/python

try:
    import psyco
    psyco.full()
except:
    pass

import ometah_test
import ometah_stats

t = ometah_test.Test()
t.setArgs('-e 50')
t.setNbRuns(30)
t.start()

u = ometah_test.Test()
u.setArgs('-e 70')
u.setNbRuns(30)
u.start()


v = ometah_test.Test()
v.setArgs('-e 100')
v.setNbRuns(30)
v.start()


w = ometah_test.Test()
w.setArgs('-e 150')
w.setNbRuns(30)
w.start()


p = [ t.getPath(), u.getPath(), v.getPath(), w.getPath() ]
ometah_stats.compare(p)
