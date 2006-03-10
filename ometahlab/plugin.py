#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  $id$
#  Copyright : Free Software Fundation
#  Author : Johann Dréo <nojhan@gmail.com>
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


import glob
import os

from rpy import *

class Plugin:
    """Base class for plugins"""
    def __init__(self,data):
        
        # link to the data object
        self.data = data
        
        # options for the output
        self.ps_paper = 'letter'
        
        # list of supported outputs
        self.output_available = ['.ps','.png']
        
        # type of the default output file
        self.fileType = ".ps"
        
        # name and description of the plugin
        self.setName("ometalab_unnamed_plugin","none")
        
        # list of written files
        self.fileList = []
        
    def setName(self,name,description="none"):
        self.name = name
        self.description = description
        self.setFileName()
        
        
    def setFileName(self):
        self.fileName = os.path.join(self.data.dir, self.name+self.fileType)
        
        
    def setFileType(self,type):
        if type in self.output_available:
            self.fileType = type
            self.setFileName()
        else:
            raise 'OptionError : unsupported output type : '+type+', not in '+self.output_available
        
        
    def outputInit(self):
        testName = self.fileName
        i=1
        while os.path.isfile(testName):
            testName = "%s_%i%s" % ( os.path.join(self.data.dir, self.name), i, self.fileType )
            
        self.fileName = testName
            
        self.fileList += [self.fileName]
        
        if self.fileType == ".ps":
            r.postscript(self.fileName, paper=self.ps_paper)
        elif self.fileType == ".png":
            r.png(self.fileName)
        else:
            return
        
    
    def outputEnd(self):
        if self.fileType in self.output_available:
            r.dev_off()
        else:
            return
        
        
    def process(self):
        pass

    def get_endOptimumsPositionDistribution_ACP(self,t):
        # ugly fix
        self.__eigenv = []
        
        (x, y) = ([], [])
        if t.problem.dimension < 2:
            print "Warning: problem dimension is < 1, the current plugin cannot be used."
            return
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
            
        return (x,y,xoptim,yoptim,xlimm,ylimm)
