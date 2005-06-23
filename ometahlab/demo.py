#!/usr/bin/python

try:
    import psyco
    psyco.full()
except:
    pass

from sys import argv

"""
NOTE :
When giving high evaluations (-e ...), make sure that you also give
a high enough max number of iterations. Each iterations contains a sample,
which size (number of evaluations) can also be given with option -s.
All default values can be checked with 'ometah --usage'.
"""

def main():
    
    if len(argv) > 1:
        arg = int(argv[1])
    else:
        arg = 0

    if arg == 0:
        """ Usage """
        print 'Usage: python demo.py <i>'
        print 'with i the number of a demo, from 1 to 3, see file content for demos details.'
        return
    
    import ometahtest
    import ometahstats    

    if arg == 1:
        """ Smallest and fastest script, only two tests run, 15 runs for each one,
        and a maximum of 64 evaluations, results don't give many information. """

        t = ometahtest.Test()
        t.setArgs('-e 30 -p Ackley')
        t.setNbRuns(15)
        t.setOmetahPath('../ometah/ometah')
        t.start()

        u = ometahtest.Test()
        u.setArgs('-e 60  -p Ackley')
        u.setNbRuns(15)
        u.setOmetahPath('../ometah/ometah')
        u.start()
    
        paths = [ t.getPath(), u.getPath() ]
        ometahstats.stat(paths)

    elif arg == 2:
        """ Average length script, to have a good preview of ometahlab results. """
    
        t = ometahtest.Test()
        t.setArgs('-e 60 -p Rastrigin -d 2')
        t.setNbRuns(30)
        t.start()
    
        u = ometahtest.Test()
        u.setArgs('-e 120  -p Rastrigin -d 2')
        u.setNbRuns(30)
        u.start()
        
        v = ometahtest.Test()
        v.setArgs('-e 240 -i 100 -p Rastrigin -d 2')
        v.setNbRuns(30)
        v.start()
        
        p = [ t.getPath(), u.getPath(), v.getPath() ]
        ometahstats.stat(p)

    elif arg == 3:
        """ Only for high frequency CPU, or patient people, we reach 1000 evaluations
        for a set of five tests, each one doing 50 runs. """
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
        ometahstats.stat(p)

    return


if __name__ == '__main__':
    main()
