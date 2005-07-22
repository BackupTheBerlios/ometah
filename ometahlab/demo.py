#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
#  File: demo.py
#  This module is a set of scripts to show how to use ometahlab,
#  it should be used directly with $ python demo.py <i>, where i stands
#  for the index of the script used.
#
###
#
#  Open Metaheuristic is a Library aimed at the conception of metaheuristics 
#  for difficult optimization.
#   
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#

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
        """ Usage """
        print 'Usage: python demo.py <i>'
        print 'with i the number of a demo, from 0 to (?), see file content for demos details.'
        return
    
    import ometahtest
    import ometahstats    
    import time

    emit = time.time()    

    if arg == 0:
        
        u = ometahtest.Test()
        u.setArgs('-e 200  -p Schwefel -P 0.00001 -d 1 -m JGEN')
        u.setNbRuns(25)
        u.setOmetahPath('../ometah/ometah')
        u.start()
        
        v = ometahtest.Test()
        v.setArgs('-e 200  -p Schwefel -P 0.00001 -d 1 -m SGEN')
        v.setNbRuns(25)
        v.setOmetahPath('../ometah/ometah')
        v.start()
        
        paths = [ u.getPath(), v.getPath() ]
        ometahstats.stat(paths)

    elif arg == 1:

        t = ometahtest.Test()
        t.setArgs('-e 100  -p Griewank -P 0.01 -d 10 -m CEDA -s 10')
        t.setNbRuns(25)
        t.setOmetahPath('../ometah/ometah')
        t.start()
        
        u = ometahtest.Test()
        u.setArgs('-e 100  -p Griewank -P 0.01 -d 10 -m CHEDA -s 10')
        u.setNbRuns(25)
        u.setOmetahPath('../ometah/ometah')
        u.start()

        z = ometahtest.Test()
        z.setArgs('-e 100  -p Griewank -P 0.01 -d 10 -m HCIAC -s 10')
        z.setNbRuns(25)
        z.setOmetahPath('../ometah/ometah')
        z.start()

        a = ometahtest.Test()
        a.setArgs('-e 100  -p Griewank -P 0.01 -d 10 -m JGEN -s 10')
        a.setNbRuns(25)
        a.setOmetahPath('../ometah/ometah')
        a.start()

        c = ometahtest.Test()
        c.setArgs('-e 100  -p Griewank -P 0.01 -d 10 -m NMS -s 10')
        c.setNbRuns(25)
        c.setOmetahPath('../ometah/ometah')
        c.start()

        b = ometahtest.Test()
        b.setArgs('-e 100  -p Griewank -P 0.01 -d 10 -m SGEN -s 10')
        b.setNbRuns(25)
        b.setOmetahPath('../ometah/ometah')
        b.start()        
         
        paths = [ t.getPath(), u.getPath(), z.getPath(), a.getPath(), c.getPath(), b.getPath() ]
        ometahstats.stat(paths)

    elif arg == 2:

        v = ometahtest.Test()
        v.setArgs('-e 100 -p Rosenbrock -P 0.01 -d 10 -m CEDA -s 20')
        v.setNbRuns(30)
        v.setOmetahPath('../ometah/ometah')
        v.start()
        
        w = ometahtest.Test()
        w.setArgs('-e 100  -p Rosenbrock -P 0.01 -d 10 -m CHEDA -s 20')
        w.setNbRuns(30)
        w.setOmetahPath('../ometah/ometah')
        w.start()

        z = ometahtest.Test()
        z.setArgs('-e 100  -p Rosenbrock -P 0.01 -d 10 -m HCIAC -s 20')
        z.setNbRuns(30)
        z.setOmetahPath('../ometah/ometah')
        z.start()

        a = ometahtest.Test()
        a.setArgs('-e 100  -p Rosenbrock -P 0.01 -d 10 -m JGEN -s 20')
        a.setNbRuns(30)
        a.setOmetahPath('../ometah/ometah')
        a.start()
        
        paths = [ v.getPath(), w.getPath() ]
        ometahstats.stat(paths)

    elif arg == 3:

        a = ometahtest.Test()
        a.setArgs('-e 200 -p Rastrigin -P 0.01 -d 3 -m CEDA -s 20')
        a.setNbRuns(25)
        a.setOmetahPath('../ometah/ometah')
        a.start()        

        b = ometahtest.Test()
        b.setArgs('-e 200 -p Rastrigin -P 0.01 -d 3 -m CHEDA -s 20')
        b.setNbRuns(25)
        b.setOmetahPath('../ometah/ometah')
        b.start()

        c = ometahtest.Test()
        c.setArgs('-e 200 -p Rastrigin -P 0.01 -d 3 -m HCIAC -s 20')
        c.setNbRuns(25)
        c.setOmetahPath('../ometah/ometah')
        c.start()
        
        d = ometahtest.Test()
        d.setArgs('-e 200 -p Rastrigin -P 0.01 -d 3 -m JGEN -s 20')
        d.setNbRuns(25)
        d.setOmetahPath('../ometah/ometah')
        d.start()

        e = ometahtest.Test()
        e.setArgs('-e 200 -p Rastrigin -P 0.01 -d 3 -m NMS -s 20')
        e.setNbRuns(25)
        e.setOmetahPath('../ometah/ometah')
        e.start()        
        
        f = ometahtest.Test()
        f.setArgs('-e 200 -p Rastrigin -P 0.01 -d 3 -m RA -s 20')
        f.setNbRuns(25)
        f.setOmetahPath('../ometah/ometah')
        f.start()
        
        g = ometahtest.Test()
        g.setArgs('-e 200  -p Rastrigin -P 0.01 -d 3 -m SGEN -s 20')
        g.setNbRuns(25)
        g.setOmetahPath('../ometah/ometah')
        g.start()

        paths = [ a.getPath(), b.getPath(), c.getPath(), d.getPath(), e.getPath(), f.getPath(), g.getPath() ]
        ometahstats.stat(paths)

    elif arg == 4:

        t = ometahtest.Test()
        t.setArgs('-e 500 -i 200 -p Ackley -P 0.01 -d 10 -m NMS -s 20')
        t.setNbRuns(25)
        t.setOmetahPath('../ometah/ometah')
        t.start()        

        v = ometahtest.Test()
        v.setArgs('-e 500 -i 200 -p Ackley -P 0.01 -d 10 -m JGEN -s 20')
        v.setNbRuns(25)
        v.setOmetahPath('../ometah/ometah')
        v.start()
        
        w = ometahtest.Test()
        w.setArgs('-e 500 -i 200 -p Ackley -P 0.01 -d 10 -m HCIAC -s 20')
        w.setNbRuns(25)
        w.setOmetahPath('../ometah/ometah')
        w.start()

        paths = [ t.getPath(), v.getPath(), w.getPath() ]
        ometahstats.stat(paths)
        

    elif arg == 5:
    
        t = ometahtest.Test()
        t.setArgs('-e 220 -p Sphere -d 3 -P 0.00001 -m CEDA')
        t.setNbRuns(30)
        t.start()
    
        u = ometahtest.Test()
        u.setArgs('-e 220  -p Sphere -d 3 -P 0.00001 -m HCIAC')
        u.setNbRuns(30)
        u.start()
        
        v = ometahtest.Test()
        v.setArgs('-e 220 -i 100 -p Sphere -d 3 -P 0.00001 -m JGEN')
        v.setNbRuns(30)
        v.start()
        
        p = [ t.getPath(), u.getPath(), v.getPath() ]
        ometahstats.stat(p)
        
    print 'Time: ', time.time()-emit
    
    return


if __name__ == '__main__':
    main()
