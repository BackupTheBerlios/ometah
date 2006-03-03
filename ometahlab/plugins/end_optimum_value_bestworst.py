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
# use the singular
# for example : end_optimum_distribution
class end_optimum_value_bestworst(Plugin):

    def __init__(self,data):
        # call the plugin basic initializations
        Plugin.__init__(self,data)

        # set the name of your plugin
        self.setName("end_optimum_value_bestworst",
        "Plot the graph of optimas' values, to finally have one Point for each \
        Test, we present 3 selections :\
        1- best optimum\
        2- worst optimum\
        3- median optimum value")
        

    # necessary method, called when lauching the plugin
    def process(self):
        # uncomment this line if you use a R output
        self.outputInit()
        
        # put your plugin code here
        # the data are in self.data :
        #   self.data.color
        #   self.data.optimas
        #   self.data.pointsIter
        #   self.data.optimaIter
        #   ...
        
        # make best optima list (minima)
        olist = [(min([p.value for p in points])) for points in self.data.optimas ]
        # make worst optima list (maxima)
        wlist = [(max([p.value for p in points])) for points in self.data.optimas ]
        # make media optima list (sort and take (len/2)'s index value)        
        [ points.sort() for points in self.data.optimas ]
        # all sublist should have the same length, that is NB_RUN
        length = len(self.data.optimas[0])
        medianIndex = length/2 # integer division, ok with first index = zero
        mlist = [ points[medianIndex].value for points in self.data.optimas]

        # to avoid crash when a value = 0
        zero = False
        # only consider bests
        if 0 in olist:
            zero = True
        if zero:
            r.plot(olist, type='n', main='Bests optima evolution', xlab='Test index', ylab='Optima value')
            r.lines(olist)
            r.points(olist, bg = 'white', pch = 21)
            r.grid(nx=10, ny=40)
            r.plot(wlist, type='n', main='Worsts optima evolution', xlab='Test index', ylab='Optima value')
            r.lines(wlist)
            r.points(wlist, bg ='white', pch = 21, lty='dashed')
            r.grid(nx=10, ny=40)
            r.plot(mlist, type='n', main='Median optima evolution', xlab='Test index', ylab='Optima value')        
            r.lines(mlist)
            r.points(mlist, bg ='white', pch = 21, lty='dotted')
            r.grid(nx=10, ny=40)
            r.matplot(r.cbind(olist, mlist, wlist), type='n', main='Optima evolution: worst, median, and best', xlab='Test index', ylab='Value')

        else:
            r.plot(olist, type='n', log="y", main='Bests optima evolution', xlab='Test index', ylab='Optima value')
            r.lines(olist)
            r.points(olist, bg = 'white', pch = 21)
            r.grid(nx=10, ny=40)
            r.plot(wlist, type='n', log="y", main='Worsts optima evolution', xlab='Test index', ylab='Optima value')
            r.lines(wlist)
            r.points(wlist, bg ='white', pch = 21, lty='dashed')
            r.grid(nx=10, ny=40)
            r.plot(mlist, type='n', log="y", main='Median optima evolution', xlab='Test index', ylab='Optima value')        
            r.lines(mlist)
            r.points(mlist, bg ='white', pch = 21, lty='dotted')
            r.grid(nx=10, ny=40)
            r.matplot(r.cbind(olist, mlist, wlist), log="y", type='n', main='Optima evolution: worst, median, and best', xlab='Test index', ylab='Value')

        r.points(olist, bg ='white', pch = 21, type='o')
        r.points(mlist, bg ='white', pch = 22, type='o', lty='dotted')
        r.points(wlist, bg ='white', pch = 23, type='o', lty='dashed')

        r.grid(nx=10, ny=40)
        
        # uncomment this line if you use a R output
        self.outputEnd()
