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
class end_optimum_position(Plugin):

    def __init__(self,data):
        # call the plugin basic initializations
        Plugin.__init__(self,data)

        # set the name of your plugin
        self.setName("end_optimum_position","Plot optima and the optimum in their neighborhood plan, PCA used if dimension > 2")
        
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
            if t.problem.dimension < 2:
                # uncomment this line if you use a R output
                self.outputInit()  

                (x, y) = (xrange(len(t.optima)), [])

                y = [ p.coords[0] for p in t.optima ]
                y.sort()
                
                xlimm = [0, len(t.optima)]
                ylimm = [t.pb_optimum[0].coords[0]-.1, max(y) ]
                txt = '%s\nPoints ordered positions' % t.args
                r.plot(x, y, xlim=xlimm, ylim=ylimm, xlab = 'Points, from best to worst', ylab='Position', \
                       main=txt, bg='white', pch=21, type='o')
                opt = t.pb_optimum[0].coords[0]                
                r.lines(xlimm, [opt, opt], col='black', type='o')
                r.grid(nx=10, ny=40)
                
                # uncomment this line if you use a R output
                self.outputEnd()

            else:
                (x, y) = ([], [])                                
                if t.problem.dimension == 2:
                    x = [ p.coords[0] for p in t.optima ]
                    y = [ p.coords[1] for p in t.optima ]
                    op = t.pb_optimum[0]
                    for p in t.pb_optimum:
                        if p.value < op.value:
                            op = p
                    xoptim = op.coords[0]
                    yoptim = op.coords[1]
                    xmin = t.pb_min_bound[0].coords[0]
                    ymin = t.pb_min_bound[0].coords[1]
                    xmax = t.pb_max_bound[0].coords[0]
                    ymax = t.pb_max_bound[0].coords[1]
                    m = max(max(x), xoptim)
                    if m < xmax:
                        xmax = 0.8 * m + 0.2 * xmax
                    m = max(max(y), yoptim)
                    if m < ymax:
                        ymax = 0.8 * m + 0.2 * ymax
                    m = min(min(x), xoptim)
                    if m > xmin:
                        xmin = 0.8 * m + 0.2 * xmin
                    m = min(min(y), yoptim)
                    if m > ymin:
                        ymin = 0.8 * m + 0.2 * ymin
                    
                    xlimm = [xmin, xmax]
                    ylimm = [ymin, ymax]
                    
                else:
                    import matrix
                    a = matrix.PCA()
                    # append solution Points
                    co = [p.coords for p in t.optima]
                    # also optimum
                    op = t.pb_optimum[0]
                    
                    for p in t.pb_optimum:
                        if p.value < op.value:
                            op = p
                    co.append(op.coords)
                    
                    a.setMatrix(co)
                    self.__eigenv.append(a.getEigenVectors())
                
                    x = [a.reduceDim(i,2)[0] for i in xrange(len(co) - 1) ]
                    y = [a.reduceDim(i,2)[1] for i in xrange(len(co) - 1) ]
                    res = a.reduceDim(len(co) - 2, 2)
                    xoptim = res[0]
                    yoptim = res[1]

                    del(a)
                    # add optimum points to point list, if it has an extrema value 
                    x.append(xoptim)
                    y.append(yoptim)

                    xlimm = [min(x), max(x)]
                    ylimm = [min(y), max(y)]
                
                # uncomment this line if you use a R output
                self.outputInit()  
                
                txt = '%s\nSolutions positions' % t.args
                r.plot(x,y, bg='white', pch=21, xlab='X', ylab='Y', \
                       main=txt, xlim=xlimm, ylim=ylimm)                

                r.points([xoptim], \
                         [yoptim], \
                         bg='black', pch=21)
                r.grid(nx=10, ny=40)
           
        
                # uncomment this line if you use a R output
                self.outputEnd()
