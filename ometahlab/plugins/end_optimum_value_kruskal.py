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
class end_optimum_value_kruskal(Plugin):

    def __init__(self,data):
        # call the plugin basic initializations
        Plugin.__init__(self,data)

        # set the name of your plugin
        self.setName("end_optimum_value_kruskal",
        "Make non-parametric over ALL tests.")
        

    # necessary method, called when lauching the plugin
    def process(self):
        
        # put your plugin code here
        # the data are in self.data :
        #   self.data.color
        #   self.data.optimas
        #   self.data.pointsIter
        #   self.data.optimaIter
        #   ...
        import Numeric
        limit = .05
        # initialize our matrix with as sublists as tests
        emlist = [ [] for i in self.data.tests ]
        breaks = 10
        
        for i in xrange(len(self.data.optimas)):
            # for current test's optima list, add their error as a list at emlist[i]
            emlist[i] = [p.value - self.data.tests[i].opt_val for p in self.data.optimas[i]]
            txt = '%s\nOptima error distribution' % self.data.tests[i].args
            #r.hist(emlist[i], breaks, col=self.data.color, main=txt, xlab='Error', ylab='Frequency')
            #r.grid(nx=10)

        
        if len(self.data.tests) == 2:
            # use Mann-Whitney test
            dic = r.wilcox_test(Numeric.array(emlist))
        elif len(self.data.tests) > 2:
            # use Kruskal-Wallis test
            dic = r.kruskal_test(emlist)
        else: # only one test
            print "Cannot use %s on one test only" % (self.name)
            return
        
        # TODO : find a way to automate the LaTeX output in a plugin
        #self.__same_distrib = dic['p.value']
        print "Non-parametric test on optima error over all tests: ",dic['p.value']
