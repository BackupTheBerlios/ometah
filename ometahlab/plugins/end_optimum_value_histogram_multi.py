#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  Author : Johann Dréo <nojhan@gmail.com>
#  $Id: end_optimum_value_histogram_multi.py,v 1.1 2006/03/16 18:04:16 nojhan Exp $
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

class end_optimum_value_histogram_multi(Plugin):

    def __init__(self,data):
        Plugin.__init__(self,data)

        self.setName("end_optimum_value_histogram_multi",
        "Plot frequency distribution of optima for each test, all in one plot. \
        One optimum for each run of the test.")

    def process(self):
    
        r.require('plotrix')
        
        breaks = 10 # nb breaks in histo, may be reduced if not enough points
        
        self.outputInit()
        
        hlist = []
        legend = []
        i=0
        txt="Optima distributions"
        for points in self.data.optimas:
            
            vlist = [p.value for p in points]
            hlist += [vlist]
            
            #txt += '\n%s' % self.data.tests[i].args
            legend += [self.data.tests[i].args] #["%i" % (i)] 
            
            i+=1
            
        r.multhist(hlist, breaks, main=txt, legend_text=legend, xlab='Value', ylab='Counts') 
        #r.grid(nx=10)
        
        self.outputEnd()
