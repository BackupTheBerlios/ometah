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
class end_optimum_distribution_position2D_kernelestimation(Plugin):

    def __init__(self,data):
        # call the plugin basic initializations
        Plugin.__init__(self,data)

        # set the name of your plugin
        self.setName("end_optimum_distribution_position2D_kernelestimation",
        "Gaussian kernel estimation of end optimums distribution in the solution space.")
        
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
            self.outputInit()
            (x, y) = ([], [])
            if t.problem.dimension < 2:
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
            
            txt = '%s\nSolutions positions distribution' % t.args
            
            # 2D distribution histogram
            r.require('MASS') 
            
            r.assign('x',x)
            r.assign('y',y)
            r('d <- kde2d(x,y,n=50) ')

            r('kde2dplot <- function(d,ncol=50,zlim=c(0,max(z)),nlevels=20,theta=30,phi=30){z   <- d$z;nrz <- nrow(z);ncz <- ncol(z);couleurs  <- tail(topo.colors(trunc(1.4 * ncol)),ncol) ;fcol      <- couleurs[trunc(z/zlim[2]*(ncol-1))+1] ;dim(fcol) <- c(nrz,ncz) ;fcol      <- fcol[-nrz,-ncz];par(mfrow=c(1,2),mar=c(0.5,0.5,0.5,0.5)) ;persp(d,col=fcol,zlim=zlim,theta=theta,phi=phi,zlab="density") ;par(mar=c(2,2,2,2)) ;image(d,col=couleurs) ;contour(d,add=T,nlevels=nlevels) ;box()}')

            r('kde2dplot(d)')
            
            #~ d = r.kde2d(x,y,n=50) 
            #~ ncol=50          # the number of colors to use 
            #~ zlim=c(0,max(z)) # limits in z coordinates 
            #~ nlevels=20       # see option nlevels in contour 
            #~ theta=30         # see option theta in persp
            #~ phi=30           # see option phi in persp
                                
            #~ z = r.d$z
            #~ nrz <- nrow(z) 
            #~ ncz <- ncol(z) 
            
            #~ couleurs  <- tail(topo.colors(trunc(1.4 * ncol)),ncol) 
            #~ fcol      <- couleurs[trunc(z/zlim[2]*(ncol-1))+1] 
            #~ dim(fcol) <- c(nrz,ncz) 
            #~ fcol      <- fcol[-nrz,-ncz]
            
            #~ par(mfrow=c(1,2),mar=c(0.5,0.5,0.5,0.5)) 
            #~ persp(d,col=fcol,zlim=zlim,theta=theta,phi=phi,zlab="density") 
            
            #~ par(mar=c(2,2,2,2)) 
            #~ image(d,col=couleurs) 
            #~ contour(d,add=T,nlevels=nlevels) 
            #~ box() 

            #~ kde2dplot(d)
   
            # uncomment this line if you use a R output
            self.outputEnd()
