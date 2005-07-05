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
        arg = 0

    if arg == 0:
        """ Usage """
        print 'Usage: python demo.py <i>'
        print 'with i the number of a demo, from 1 to 6, see file content for demos details.'
        return
    
    import ometahtest
    import ometahstats    
    import time

    emit = time.time()

    if arg == 1:

        t = ometahtest.Test()
        t.setArgs('-e 30 -p Ackley -P 0.01')
        t.setNbRuns(15)
        t.setOmetahPath('../ometah/ometah')
        t.start()
        
        u = ometahtest.Test()
        u.setArgs('-e 60  -p Ackley -P 0.01')
        u.setNbRuns(15)
        u.setOmetahPath('../ometah/ometah')
        u.start()
    
        paths = [ t.getPath(), u.getPath() ]
        ometahstats.stat(paths)

    elif arg == 2:

        t = ometahtest.Test()
        t.setArgs('-e 30 -p Ackley -d 5 -P 0.01 ')
        t.setNbRuns(20)
        t.setOmetahPath('../ometah/ometah')
        t.start()

        u = ometahtest.Test()
        u.setArgs('-e 60  -p Ackley -d 5 -P 0.01')
        u.setNbRuns(20)
        u.setOmetahPath('../ometah/ometah')
        u.start()
    
        paths = [ t.getPath(), u.getPath() ]
        ometahstats.stat(paths)

    elif arg == 3:
    
        t = ometahtest.Test()
        t.setArgs('-e 60 -p Sphere -d 10 -P 0.01')
        t.setNbRuns(30)
        t.start()
    
        u = ometahtest.Test()
        u.setArgs('-e 120  -p Sphere -d 10 -P 0.01')
        u.setNbRuns(30)
        u.start()
        
        v = ometahtest.Test()
        v.setArgs('-e 240 -i 100 -p Sphere -d 10 -P 0.01')
        v.setNbRuns(30)
        v.start()
        
        p = [ t.getPath(), u.getPath(), v.getPath() ]
        ometahstats.stat(p)

    elif arg == 4:        
    
        t = ometahtest.Test()
        t.setArgs('-e 60 -p Rosenbrock -d 2 -P 0.01')
        t.setNbRuns(30)
        t.start()
    
        u = ometahtest.Test()
        u.setArgs('-e 120  -p Rosenbrock -d 2 -P 0.01')
        u.setNbRuns(30)
        u.start()
        
        v = ometahtest.Test()
        v.setArgs('-e 240 -i 100 -p Rosenbrock -d 2 -P 0.01')
        v.setNbRuns(30)
        v.start()
        
        p = [ t.getPath(), u.getPath(), v.getPath() ]
        ometahstats.stat(p)
 

    elif arg == 5:
  
        t = ometahtest.Test()
        t.setArgs('-e 50 -p Griewank -P 0.000001')
        t.setNbRuns(50)
        t.setOmetahPath('../ometah/ometah')
        t.start()
        
        u = ometahtest.Test()
        u.setArgs('-e 100 -p Griewank -m CEDA -P 0.000001')
        u.setNbRuns(50)
        u.setOmetahPath('../ometah/ometah')
        u.start()
        
        v = ometahtest.Test()
        v.setArgs('-e 200 -p Griewank -P 0.000001')
        v.setNbRuns(50)
        v.start()
        
        w = ometahtest.Test()
        w.setArgs('-e 500 -i 150 -p Griewank -P 0.000001')
        w.setNbRuns(50)
        w.setOmetahPath('../ometah/ometah')
        w.start()
        
        y = ometahtest.Test()
        y.setArgs('-e 1000 -i 1000 -p Griewank -P 0.000001')
        y.setNbRuns(50)
        y.start()

        p = [ t.getPath(), u.getPath(), v.getPath(), w.getPath(), y.getPath()]
        ometahstats.stat(p)

    
    elif arg == 6:
        
        t = ometahtest.Test()
        t.setArgs('-e 50 -p Schwefel  -P 0.000001 -d 3')
        t.setNbRuns(50)
        t.setOmetahPath('../ometah/ometah')
        t.start()
        
        u = ometahtest.Test()
        u.setArgs('-e 100 -p Schwefel -m CEDA  -P 0.000001 -d 3')
        u.setNbRuns(50)
        u.setOmetahPath('../ometah/ometah')
        u.start()
        
        v = ometahtest.Test()
        v.setArgs('-e 200 -i 200 -p Schwefel  -P 0.000001 -d 3')
        v.setNbRuns(50)
        v.start()
        
        w = ometahtest.Test()
        w.setArgs('-e 500 -i 300  -p Schwefel  -P 0.000001 -d 3')
        w.setNbRuns(50)
        w.setOmetahPath('../ometah/ometah')
        w.start()
        
        y = ometahtest.Test()
        y.setArgs('-e 1000 -i 1000 -p Schwefel  -P 0.000001 -d 3')
        y.setNbRuns(50)
        y.start()

        p = [ t.getPath(), u.getPath(), v.getPath(), w.getPath(), y.getPath()]
        ometahstats.stat(p)

    elif arg == 7:
    
        t = ometahtest.Test()
        t.setArgs('-e 40 -r 13')
        t.start()    
        
        p = [t.getPath()]        
        ometahstats.stat(p)

    elif arg == 8:

        t = ometahtest.Test()
        t.setArgs('-e 1000 -p Schwefel -m CEDA  -P 0.000001 -d 10')
        t.setNbRuns(20)
        t.setOmetahPath('../ometah/ometah')
        t.start()
        
        u = ometahtest.Test()
        u.setArgs('-e 3000 -p Schwefel -m CEDA  -P 0.000001 -d 30')
        u.setNbRuns(20)
        u.setOmetahPath('../ometah/ometah')
        u.start()
        
        v = ometahtest.Test()
        v.setArgs('-e 5000 -i 100 -p Schwefel -m CEDA -P 0.000001 -d 50')
        v.setNbRuns(20)
        v.start()
        
        p = [ t.getPath(), u.getPath(), v.getPath()]
        ometahstats.stat(p)


    elif arg == 9:

        t = ometahtest.Test()
        t.setArgs('-e 100 -p Griewank -P 0.01 -d 2')
        t.setNbRuns(20)
        t.setOmetahPath('../ometah/ometah')
        t.start()
        
        u = ometahtest.Test()
        u.setArgs('-e 100 -p Griewank -m CEDA  -P 0.01 -d 2')
        u.setNbRuns(20)
        u.setOmetahPath('../ometah/ometah')
        u.start()
        
        v = ometahtest.Test()
        v.setArgs('-e 100 -i 100 -p Griewank -m CEDA -P 0.01 -d 2')
        v.setNbRuns(20)
        v.start()
        
        w = ometahtest.Test()
        w.setArgs('-e 100 -i 100  -p Griewank -m CEDA -P 0.01 -d 2')
        w.setNbRuns(20)
        w.setOmetahPath('../ometah/ometah')
        w.start()
        
        y = ometahtest.Test()
        y.setArgs('-e 100 -i 100 -p Griewank -m CEDA -P 0.01 -d 2')
        y.setNbRuns(20)
        y.start()

        p = [ t.getPath(), u.getPath(), v.getPath(), w.getPath(), y.getPath()]
        ometahstats.stat(p)

                
    
    elif arg == 10:
        
        u = ometahtest.Test()
        u.setArgs('-e 300 -i 200 -p Griewank -m CEDA -s 10 -P 0.01 -d 3')
        u.setNbRuns(40)
        u.setOmetahPath('../ometah/ometah')
        u.start()
        
        v = ometahtest.Test()
        v.setArgs('-e 300 -i 200 -p Griewank -m CEDA -s 20 -P 0.01 -d 3')
        v.setNbRuns(40)
        v.start()
        
        w = ometahtest.Test()
        w.setArgs('-e 300 -i 200  -p Griewank -m CEDA -s 30 -P 0.01 -d 3')
        w.setNbRuns(40)
        w.setOmetahPath('../ometah/ometah')
        w.start()

        p = [ u.getPath(), v.getPath(), w.getPath()]
        ometahstats.stat(p)

        
    print 'Time: ', time.time()-emit
    
    return


if __name__ == '__main__':
    main()
