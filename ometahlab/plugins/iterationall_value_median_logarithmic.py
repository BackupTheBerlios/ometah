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


# This is a string marking the file as a ometahlab plugin
# isOmetahLabPlugin

# necessary base class
from plugin import Plugin

# import rpy if you use R
from rpy import *

# name your plugin according to the following notation :
# (end|iteration)_(optimum|all)_(type)
# for example : end_optimum_distribution
class iteration_all_value_median_logarithmic(Plugin):

    def __init__(self,data):
        # call the plugin basic initializations
        Plugin.__init__(self,data)

        # set the name of your plugin
        self.setName("iteration_all_value_median_logarithmic",
        "Convergence graph over iterations : plot median error of each run for \
        a given iteration, using logarithmic scale.")
        

    # necessary method, called when lauching the plugin
    def process(self):
        
        # put your plugin code here
        # the data are in self.data :
        #   self.data.color
        #   self.data.optimas
        #   self.data.pointsIter
        #   self.data.optimaIter
        #   ...
        
        i = 0
        for list in self.data.pointsIter:
            # uncomment this line if you use a R output
            self.outputInit()
            elist = [[p.error for p in points] for points in list ]
            errlist = [r.median(list) for list in elist]
            txt = '%s\nConvergence of median error of all points' % self.data.tests[i].args
            try:
                r.plot(errlist, main=txt, type='o', ylab='Median error', xlab='Iteration', log="y")
            except:
                if self.__LOG:
                    self.__LOG = 0
                    print 'Cannot use logarithmic scale 4'
                r.plot(errlist, main=txt, type='o', ylab='Median error', xlab='Iteration')
            r.grid(nx=10, ny=40)
            i += 1
        
            # uncomment this line if you use a R output
            self.outputEnd()
