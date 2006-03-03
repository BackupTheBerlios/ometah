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
class iteration_all_value_quantilebox(Plugin):

    def __init__(self,data):
        # call the plugin basic initializations
        Plugin.__init__(self,data)

        # set the name of your plugin
        self.setName("iteration_all_value_quantilebox",
        "For each Test, plot the sequence of iterations (for any run) as a set \
        of quantile boxes. So we have one graphic for each Test.")
        

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
        for metalist in self.data.pointsIter:
            # uncomment this line if you use a R output
            self.outputInit()
            vlist = [[p.value for p in points] for points in metalist ]
            txt = '%s\nConvergence of all points' % self.data.tests[i].args
            zero = False
            for l in vlist:
                if 0 in l:
                    zero = True
            if zero:
                r.boxplot(vlist, style='quantile', col=self.data.color, main=txt, xlab='Iteration index', ylab='Point value')
            else:
                try:
                    r.boxplot(vlist, style='quantile', col=self.data.color, log="y", main=txt, xlab='Iteration index', ylab='Point value')
                except:
                    if self.__LOG:
                        self.__LOG = 0
                        print 'Cannot use logarithmic scale 2'
                    r.boxplot(vlist, style='quantile', col=self.data.color, main=txt, xlab='Iteration index', ylab='Point value')
            r.grid(nx=10, ny=40)
            i += 1
        
            # uncomment this line if you use a R output
            self.outputEnd()
