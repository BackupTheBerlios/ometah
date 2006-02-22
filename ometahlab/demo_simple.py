#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
#  Author : Johann Dréo <nojhan@gmail.com>
#  $Id: demo_simple.py,v 1.5 2006/02/22 14:52:46 nojhan Exp $
#  Copyright : Free Software Fundation
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

import ometahtest
import ometahstats

runs = 1000

print 'Launching test runs:'
print '  First test:'
u = ometahtest.Test()
u.setArgs('-s 10 -i 10 -e 100 -p Rosenbrock -d 2 -m CEDA')
u.setNbRuns(runs)
u.setOmetahPath('../ometah/ometah')
u.start()

print '  Second test:'
v = ometahtest.Test()
v.setArgs('-s 10 -i 10 -e 100 -p Rosenbrock -d 2 -m RA')
v.setNbRuns(runs)
v.setOmetahPath('../ometah/ometah')
v.start()

print '  Third test:'
w = ometahtest.Test()
w.setArgs('-s 100 -i 10 -e 10000 -p Rosenbrock -d 1 -m HCIAC')
w.setNbRuns(runs)
w.setOmetahPath('../ometah/ometah')
w.start()

print '\nLaunching data process:'
paths = [ u.getPath(), v.getPath(), w.getPath() ]
#paths = ['results/Rosenbrock_CEDA_d2_e100_r27461_1', 'results/Rosenbrock_RA_d2_e100_r1433_1']
print '  Using ',paths

ometahstats.process(paths,
    [
    'end_optimum_distribution',
    'end_optimum_quantilebox',
    'end_optimum_bestworst',
    'iteration_all_quantilebox',
    'iteration_optimum_quantilebox',
    'end_optimum_successrate',
    'end_optimum_position',
    'end_optimum_kruskal',
    'iteration_median_logarithmic',
    'end_optimum_distribution_position2D'
    ],
    '.png') # output type
