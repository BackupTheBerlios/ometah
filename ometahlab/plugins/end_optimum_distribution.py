#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
#  Author : Johann Dréo <nojhan@gmail.com>
#  $id$
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


# isOmetahLabPlugin

import os
from rpy import *
from plugin import Plugin

class end_optimum_distribution(Plugin):

    def __init__(self,data):
        Plugin.__init__(self,data)

        self.setName("end_optimum_distribution",
        "Plot frequency distribution of optima for each test, that is each Test \
        instance, each one having its own sublist of optimas in self.__optimas. \
        We have one optimum for each run of the test.")

    def process(self):
    
        breaks = 10 # nb breaks in histo, may be reduced if not enough points
        
        i = 0
        for points in self.data.optimas:
            self.outputInit()
            
            vlist = [p.value for p in points]
            txt = '%s\nOptima distribution' % self.data.tests[i].args
            r.hist(vlist, breaks, col=self.data.color, main=txt, xlab='Value', ylab='Frequency') 
            r.grid(nx=10)
            i += 1
        
            self.outputEnd()
