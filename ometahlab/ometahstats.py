#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
# Test module for Open Metaheuristic, main file
# Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
# Started 06/2005
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
import ometah_test

#!!!  to put in ometah_stat module
def compare(paths):
    """ paths is a list of paths to metaruns directories
    compare() checks their integrity and run comparison b/w them
    -> plottings, report """

    """
    instancier une Comparaison c
    s.ser = [ Serializeds loadés de path ]
    check cohérence
    optimas = ( (optima1) (optima2) ) ...
    points ( ( (iter1 pr NB_RUN) (iter2) ..) ( () () ) )
    ( => pour plotter un grah de boxes pour chaque objet serialié)

    """
    c = Comparison(paths)
    c.check()
    c.plot()
    c.writeReport()
    print '\nComparison successful, output in', c.getDir()


class Comparison:

    def __init__(self, paths):
        """ """
        import pickle
        import string
        import os
        # each metarun (each Serialized) has different parameters
        
        # list of Serialized objects, one for each metarun
        self.__runs = []
        # working directory
        self.__dir = ''
        # report file name
        self.__report = 'REPORT'

        # pickle.load serialized objects in paths
        for p in paths:
            try:
                q = "%s/SERIALIZED" % p
                fd = open(q, 'r')
                s = pickle.load(fd)
                self.__runs.append(s)
            except:
                msg = 'error while loading %s' % q
                self.fatal(msg)

        # initialize optimas and points
        #
        self.__optimas = []
        for serial in self.__runs:
            self.__optimas.append(serial.optima)

        self.__points = []
        # sample size 
        for serial in self.__runs:
            self.__points.append(serial.points)
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
        """ check the coherence of given runs, ie :
        - same problem
        - same dimensions
        - same sampleSize
        and identify FATAL divergences... """
        pass

    def start(self):
        # plot et report dans comp_%i
        self.plot()
        self.writeReport()
        pass
        
    def fatal(self, msg):
        import sys
        print 'FATAL ERROR: ', msg, '\n'
        sys.exit(-1)

    def plot(self):
        """ plot results """
        file1 = os.path.join(self.__dir, 'valuesBoxes.ps')
        file2 = os.path.join(self.__dir, 'valuesGraph.ps')

        # plot frequency distributions
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
        
        # plot a box for each sublist
        r.postscript(file1, paper='letter')
        vlist = [[p.value for p in points ] for points in self.__optimas ]
        r.boxplot(vlist, style='quantile', col='yellow', main='Optimas list', xlab='Metarun index', ylab='')
        r.grid(nx=10, ny=40)
        r.dev_off()

        # plot the nbMetarun optimas ~ we select the best among the 25 for each metarun       
        r.postscript(file2, paper='letter')
        olist = [(min([p.value for p in points])) for points in self.__optimas ]
        r.plot(olist, type='o', col='green', main='Optima evolution', xlab='Metarun index', ylab='Optima value')
        r.grid(nx=10, ny=40)
        r.dev_off()
        
        # plot convergence boxes (one graph for each metarun)
        file3 = os.path.join(self.__dir, 'valuesConvergences.ps')
        r.postscript(file3, paper='letter')
        for metalist in self.__points:
            vlist = [[p.value for p in points] for points in metalist ]
            r.boxplot(vlist, style='quantile', col='red', main='Iterations convergence', xlab='Iteration index', ylab='Optima value')
            r.grid(nx=10, ny=40)
        r.dev_off()

    def writeReport(self):
        """ write report in working directory"""
        path = os.path.join(self.__dir, self.__report)
        fd = open(path, 'w')

        txt = '\n---------------------/\n'
        txt += ' REPORT'
        txt += '\n---------------------/\n\n'
        fd.write(txt)
        txt = 'Number of command lines (metaruns) : %i\n\n' % (len(self.__runs))
        fd.write(txt)
        i = 0
        for run in self.__runs:
            i = i + 1
            txt = '\tRun %i\n-----------------/\n\n' % i
            fd.write(txt)
            txt = '\tNumber of runs : %i\n' % (run.nbRuns)
            fd.write(txt)
            txt = '\tSample size : %s\n' % run.parameters.sampleSize
            fd.write(txt)
            txt = '\tTreshold  : %s\n' % run.parameters.treshold
            fd.write(txt)
            if run.parameters.randomSeed == 0:
                txt = '\tRandom seed : Unknown (from time)\n'
            else:
                txt = '\tRandom seed  : %s\n' % run.parameters.randomSeed
            fd.write(txt)
            txt = '\tProblem : %s\n\tDimension : %i\n' \
                  % (run.problem.name, run.problem.dimension)
            fd.write(txt)
            txt = '\tOptimum value : %s\n' \
              % str(run.problem.optimum[0].value)
            fd.write(txt)
            txt = '\tOptimum solution : %s\n' \
                  % ''.join([str(i) for i in run.problem.optimum[0].coords])
            fd.write(txt)
            vals = [p.value for p in run.optima]
            txt = '\tCommand line : %s\n' % (run.args)            
            fd.write(txt)
            txt = '\tOptima mean value : %f\n' % (r.mean(vals))
            fd.write(txt)
            txt = '\tOptima standard deviation : %f\n' % (r.sd(vals))
            fd.write(txt)
            txt = '\tOptimum value : %f\n' % (min(vals))
            fd.write(txt)
            fd.write('\n\n')
            i = i + 1
        fd.close()

    def getDir(self):
        return self.__dir
