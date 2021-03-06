#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
#  Author : Johann Dréo <nojhan@gmail.com>
#  $Id: demo_simple.py,v 1.1 2006/04/07 18:03:11 nojhan Exp $
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

# Change this if you have put the ometah binary somewhere else
# or simply change it to 'ometah' if it is installed on your system
path_to_ometah_binary = '../../ometah/ometah'

# Number of run to calculate
# the higher is this number, the longer it takes to process,
# but the better are your results
runs = 10

print 'Launching test runs:'
print '  First test:'
u = ometahtest.Test(path_to_ometah_binary,'-s 10 -i 10 -e 100 -p Rosenbrock -d 2 -m CEDA',runs)
u.start()

print '  Second test:'
v = ometahtest.Test(path_to_ometah_binary,'-s 10 -i 10 -e 100 -p Rosenbrock -d 2 -m RA',runs)
v.start()

print '\nLaunching data process:'
paths = [ u.getPath(), v.getPath() ]
# or with true paths, for example : 
# paths = ['results/Rosenbrock_CEDA_d2_e100_r27461_1', 'results/Rosenbrock_RA_d2_e100_r1433_1']
print '  Using ',paths

# calculate all graphics and generate an html report
ometahstats.process(paths, 'all','html')
