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

class end_optimum_value_quantilebox(Plugin):

    def __init__(self,data):
        # call the plugin basic initializations
        Plugin.__init__(self,data)

        # set the name of your plugin
        self.setName("end_optimum_value_quantilebox","Show one quantile box of optima for each Test. ")
        

    # necessary method, called when lauching the plugin
    def process(self):
        # uncomment this line if you use a R output
        self.outputInit()
        
        # put your plugin code here
        vlist = [[p.value for p in points ] for points in self.data.optimas ]

        zero = False
        for v in vlist:
            if 0 in v:
                zero = True
        if zero:
            r.boxplot(vlist, style='quantile', col=self.data.color, main='Optimas list', xlab='Test index', ylab='')
        else:
            try:
                r.boxplot(vlist, style='quantile', log="y", col=self.data.color, main='Optimas list', xlab='Test index', ylab='')
            except:
                print 'Cannot use logarithmic scale 0'
                self.__LOG = 0
                r.boxplot(vlist, style='quantile', col=self.data.color, main='Optimas list', xlab='Test index', ylab='')
        r.grid(nx=10, ny=40)
        
        # uncomment this line if you use a R output
        self.outputEnd()
