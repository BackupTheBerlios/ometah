#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
# Ometahlab is a set of Python scripts to make experiments on Ometah.
#####################################################################
# Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
# File : ometahstats.py
# This module is used to generate graphical and text output from several Ometah executions.
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

from rpy import *
import ometahtest

#!!!  to put in ometah_stat module
def compare(paths):
    """ Generate text report and graphic representation in postscript format,
    comparing the previous Ometah execution which directories' paths are given as a list of strings,
    ometahtest's function getPath can be used to get those strings. See demoscript for usage example."""

    c = Comparison(paths)
    c.check()
    c.plot()
    c.writeReport()
    print '\nComparison successful, output in', c.getDir()


class Comparison:
    """ A comparison between several Ometah executions, giving text and graphical reports. """

    def __init__(self, paths):
        """ Constructor, paths is a list of strings, which are paths to Ometah executions directories."""
        import pickle
        import string
        import os
        # each metarun (each Test) has different parameters
        
        # list of Test objects, one for each metarun
        self.__tests = []
        # working directory
        self.__dir = ''
        # report file name
        self.__report = 'REPORT'
        # serialized file name
        self.__serialized = 'TEST'

        # pickle.load serialized objects in paths
        for p in paths:
            try:
                q = "%s/TEST" % p
                fd = open(q, 'r')
                s = pickle.load(fd)
                self.__tests.append(s)
            except:
                msg = 'error while loading %s' % q
                self.__fatal(msg)

        # initialize optimas and points
        #
        self.__optimas = []
        for serial in self.__tests:
            self.__optimas.append(serial.optima)

        self.__points = []
        # sample size 
        for test in self.__tests:
            self.__points.append(test.pointsIterations)
        # Note : each serial.points is a list of sublists, where each
        # sublist corresponds to an iteration

        # create working directory (comp_%i)
        i = 1
        ok = 0
        while not ok:
            ok = 1
            dir = '%s/ometahlab%i' \
                  % (string.split(paths[0], sep='/')[0], i)
            try:
                os.mkdir(dir)
            except:
                ok = 0
                i = i + 1
        self.__dir = dir


    def check(self):
        """ Check the coherence of Ometah executions, ie :
        - same problem
        - same problem dimensions
        - same sampleSize
        - etc...
        A fatal error or a warning message can be given, whether the divergence is fatal or not."""

        pb = self.__tests[0].problem.key
        dim = self.__tests[0].problem.dimension
        sample = self.__tests[0].parameters.sampleSize
        runsNb = self.__tests[0].runsNb

        print ''
        for test in self.__tests:
            if test.problem.key != pb:
                self.__fatal('tests have different problems.')
            elif test.problem.dimension != dim:
                self.__fatal('tests have different problem dimensions.')
            elif test.parameters.sampleSize != sample:
                self.__fatal('tests have different sample sizes.')
            elif test.runsNb != runsNb:
                self.__warning('tests have different numbers of runs.')
            elif test.parameters.randomSeed != '0': # 0 means we use time as a seed
                self.__warning('you should not specify a random seed for a test, all the runs would get the same optimum.')
        
    def __fatal(self, msg):
        import sys
        print 'FATAL ERROR : %s\n' % msg
        sys.exit(-1)

    def __warning(self, msg):
        print "WARNING : %s" % msg

    def plot(self):
        """ Plot results as postscript files :
        - optimaDistributions.ps : the frequency distributions of optimas for each Ometah execution.
        - valueBoxes.ps : each execution's optima shown as quantile boxes.
        - valuesGraph.ps : the optimum (minimal) value for each execution, as a graph.
        - valuesConvergences.ps : show each iteration of each run as a quantile box, showing the evolution of points during the optimization."""
        file1 = os.path.join(self.__dir, 'valuesBoxes.ps')
        file2 = os.path.join(self.__dir, 'valuesGraph.ps')

        ## plot frequency distributions
        breaks = 10        
        i = 1
        file0 = os.path.join(self.__dir, 'optimaDistributions.ps')        
        r.postscript(file0, paper='letter')
        for points in self.__optimas:
            vlist = [p.value for p in points]
            r.hist(vlist, breaks, col='orange', main='Optima distribution', ylab='Frequency') 
            r.grid(nx=10)
            i = i + 1
        r.dev_off()
        
        ## plot a box for each sublist
        r.postscript(file1, paper='letter')
        vlist = [[p.value for p in points ] for points in self.__optimas ]
        r.boxplot(vlist, style='quantile', col='yellow', main='Optimas list', xlab='Test index', ylab='')
        r.grid(nx=10, ny=40)
        r.dev_off()

        ## plot the nbMetarun optimas ~ we select the best among the 25 for each metarun       
        r.postscript(file2, paper='letter')

        # make best optima list (minima)
        olist = [(min([p.value for p in points])) for points in self.__optimas ]

        # make worst optima list (maxima)
        wlist = [(max([p.value for p in points])) for points in self.__optimas ]

        # make media optima list (sort and take (len/2)'s index value)        
        [ points.sort() for points in self.__optimas ]
        # all sublist should have the same length, that is NB_RUN
        length = len(self.__optimas[0])
        print "length: ", length
        medianIndex = length/2 # integer division, ok with first index = zero
        mlist = [ points[medianIndex].value for points in self.__optimas]
        
        r.plot(olist, type='o', col='blue', main='Bests optima  evolution', xlab='Test index', ylab='Optima value')
        r.grid(nx=10, ny=40)
        r.plot(wlist, type='o', col='red', main='Worsts optima  evolution', xlab='Test index', ylab='Optima value')
        r.grid(nx=10, ny=40)
        r.plot(mlist, type='o', col='orange', main='Median optima  evolution', xlab='Test index', ylab='Optima value')        
        r.grid(nx=10, ny=40)
        r.dev_off()
        
        ## plot convergence boxes (one graph for each metarun)
        file3 = os.path.join(self.__dir, 'valuesConvergences.ps')
        r.postscript(file3, paper='letter')
        for metalist in self.__points:
            vlist = [[p.value for p in points] for points in metalist ]
            r.boxplot(vlist, style='quantile', col='red', main='Iterations convergence', xlab='Iteration index', ylab='Optima value')
            r.grid(nx=10, ny=40)
        r.dev_off()

    def writeReport(self):
        """ Write the plain text report, in the file REPORT."""
        path = os.path.join(self.__dir, self.__report)
        fd = open(path, 'w')

        txt = '\n--------------------------------------------/\n'
        txt += ' REPORT'
        txt += '\n--------------------------------------------/\n\n'
        fd.write(txt)
        txt = 'Number of executions : %i\n\n' % (len(self.__tests))
        fd.write(txt)
        for test in self.__tests:
            txt = '\t%s\n' % test.args
            fd.write(txt)
        fd.write('\n')
        i = 0
        for test in self.__tests:
            i = i + 1
            txt = '%s\n--------------------------------------------/\n\n' % test.args
            fd.write(txt)
            txt = '\tNumber of runs : %i\n' % ( test.runsNb )
            fd.write(txt)
            txt = '\tSample size : %s\n' % test.parameters.sampleSize
            fd.write(txt)
            txt = '\tTreshold  : %s\n' % test.parameters.treshold
            fd.write(txt)
            if test.parameters.randomSeed == '0':
                txt = '\tRandom seed : Unknown (from time)\n'
            else:
                txt = '\tRandom seed  : %s\n' % test.parameters.randomSeed
            fd.write(txt)
            txt = '\tProblem : %s\n\tDimension : %i\n' \
                  % ( test.problem.name, test.problem.dimension )
            fd.write(txt)
            txt = '\tOptimum value : %s\n' \
              % str( test.problem.optimum[0].value )
            fd.write(txt)
            txt = '\tOptimum solution : %s\n' \
                  % ''.join( [str(i) for i in test.problem.optimum[0].coords] )
            fd.write(txt)
            vals = [p.value for p in test.optima]
            txt = '\tOptima mean value : %f\n' % (r.mean(vals))
            fd.write(txt)
            txt = '\tOptima standard deviation : %f\n' % (r.sd(vals))
            fd.write(txt)
            txt = '\tOptimum value : %f\n' % (min(vals))
            fd.write(txt)
            txt ='\tSuccess rate : ' + str(100*test.succRate) + ' % \n'
            fd.write(txt)
            fd.write('\n\n')
            i = i + 1
        fd.close()


    def getDir(self):
        """ Returns the path of the directory containing files created."""
        return self.__dir



if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
