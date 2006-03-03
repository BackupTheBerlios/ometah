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
class end_optimum_solution_histogram2D(Plugin):

    def __init__(self,data):
        # call the plugin basic initializations
        Plugin.__init__(self,data)

        # set the name of your plugin
        self.setName("end_optimum_solution_histogram2D","The positions distribution of optimums, in a 2D space. Use PCA if dimensions > 2..")
        
        # eigen vectors weight for the tests
        self.__eigenv = []

    # necessary method, called when lauching the plugin
    def process(self):
        
        # put your plugin code here
        # the data are in self.data :
        #   self.data.color
        #   self.data.optimas
        #   self.data.pointsIter
        #   self.data.optimaIter
        #   ...
        
        for t in self.data.tests:
            (x, y) = ([], [])
            (x,y) = self.get_endOptimumsPositionDistribution_ACP( t )
            
            
            # uncomment this line if you use a R output
            self.outputInit()
            
            txt = '%s\nSolutions positions distribution' % t.args
            
            # 2D distribution histogram
            r.require('gplots') 
            
            #colors = r.c("white",r.rainbow(16))
            #colors = r.c("white",r.rainbow(16,0.1))
            #colors = r.c("white",r.heat_colors(16))
            #colors = r.c("white",r.terrain_colors(16))
            #colors = r.c("white",r.topo_colors(16))
            #colors = r.c("white",r.cm_colors(16))
            v=[i/1000.0 for i in range(0,1000,125)]+[1]
            v.sort(reverse = True)
            colors = r.gray( v )
            
            ins = int(2 * math.sqrt( 0.5*len(x) )) # magic formula
            
            r.hist2d(x,y,
                     nbins = ins,
                     col = colors,
                     xlab = 'X', ylab='Y', main=txt)
                         
            # points
            r.points([xoptim], \
                     [yoptim], \
                     bg='black', pch=21)
            
            #r.rug(x,side=1) 
            #r.rug(y,side=2) 
            r.box() 
   
            # uncomment this line if you use a R output
            self.outputEnd()
