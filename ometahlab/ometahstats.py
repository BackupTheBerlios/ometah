#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
#  File : ometahstats.py
#  This module is used to generate graphical and text output from several Ometah executions.
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

from rpy import *
import ometahtest


def stat(paths):
    """ Generate text report and graphic representation in postscript format,
    comparing the previous Ometah execution which directories' paths are given as a list of strings,
    ometahtest's function getPath can be used to get those strings. See demoscript for usage example."""

    c = Comparison(paths)
    c.check()
    c.plot()
    c.writeLatex()
    print '\nResults in %s\n' % c.getDir()


class Comparison:
    """ A comparison between several Ometah executions (Test instances), giving text and graphical reports. """

    def __init__(self, paths):
        """ Constructor, paths is a list of strings, which are paths to Ometah executions directories."""
        import pickle
        import string
        import os
        # each each Test has different parameters

        # color used in graphic plottings
        self.__color = 'grey85'
        
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
                q = os.path.join(p, 'TEST')
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

        self.__pointsIter = []
        for test in self.__tests:
            self.__pointsIter.append(test.pointsIterations)
            
        self.__optimaIter = []
        for test in self.__tests:
            self.__optimaIter.append(test.optimaIterations)
        # Note : each serial.points is a list of sublists, where each
        # sublist corresponds to an iteration

        # create working directory (comp_%i)
        i = 1
        ok = 0
        while not ok:
            ok = 1
            dir = 'results_%i' % i
            dir = os.path.join( string.split(paths[0], sep='/')[0], dir)
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
                self.__warning('tests have different problem dimensions.')
            elif test.parameters.sampleSize != sample:
                self.__warning('tests have different sample sizes.')
            elif test.runsNb != runsNb:
                self.__warning('tests have different numbers of runs.')
        
    def __fatal(self, msg):
        import sys
        print 'FATAL ERROR : %s\n' % msg
        sys.exit(-1)

    def __warning(self, msg):
        print "WARNING : %s" % msg

    def __plot_1(self):
        """ Plot frequency distribution of optima for each test, that is each Test instance,
        each one having its own sublist of optimas in self.__optimas. We have one optimum for each run of the test. """
        fileName = os.path.join(self.__dir, 'distribution_optima.ps')        
        breaks = 10 # number of breaks in the histogram, may be reduced if not enough points
        r.postscript(fileName, paper='letter')
        i = 0
        for points in self.__optimas:
            vlist = [p.value for p in points]
            txt = '%s\nOptima distribution' % self.__tests[i].args
            r.hist(vlist, breaks, col=self.__color, main=txt, xlab='Value', ylab='Frequency') 
            r.grid(nx=10)
            i += 1
        r.dev_off()
        
    def __plot_2(self):
        """ Show one quantile box of optima for each Test. """
        fileName = os.path.join(self.__dir, 'testboxes_optima.ps')
        r.postscript(fileName, paper='letter')
        vlist = [[p.value for p in points ] for points in self.__optimas ]
        r.boxplot(vlist, style='quantile', col=self.__color, main='Optimas list', xlab='Test index', ylab='')
        r.grid(nx=10, ny=40)
        r.dev_off()

    def __plot_3(self):
        """ Plot the graph of optimas' values, to finally have one Point for each Test, we present 3 selections :
        - best optimum
        - worst optimum
        - median optimum value"""
        fileName = os.path.join(self.__dir, 'graph_optima.ps')        
        r.postscript(fileName, paper='letter')
        
        # make best optima list (minima)
        olist = [(min([p.value for p in points])) for points in self.__optimas ]

        # make worst optima list (maxima)
        wlist = [(max([p.value for p in points])) for points in self.__optimas ]

        # make media optima list (sort and take (len/2)'s index value)        
        [ points.sort() for points in self.__optimas ]
        # all sublist should have the same length, that is NB_RUN
        length = len(self.__optimas[0])
        medianIndex = length/2 # integer division, ok with first index = zero
        mlist = [ points[medianIndex].value for points in self.__optimas]
        r.plot(olist, type='n', main='Bests optima evolution', xlab='Test index', ylab='Optima value')
        r.lines(olist, lty='dotted')
        r.points(olist, bg = 'white', pch = 21)
        r.grid(nx=10, ny=40)
        r.plot(wlist, type='n', main='Worsts optima evolution', xlab='Test index', ylab='Optima value')
        r.lines(wlist, lty='dotted')
        r.points(wlist, bg ='white', pch = 21)
        r.grid(nx=10, ny=40)
        r.plot(mlist, type='n', main='Median optima evolution', xlab='Test index', ylab='Optima value')        
        r.lines(mlist, lty='dotted')
        r.points(mlist, bg ='white', pch = 21)
        r.grid(nx=10, ny=40)
        r.dev_off()


    def __plot_4(self):
        """ For each Test, plot the sequence of iterations (for any run) as a set of quantile boxes. So we have one graphic for each Test. """
        fileName = os.path.join(self.__dir, 'convergence_points.ps')
        r.postscript(fileName, paper='letter')
        i = 0
        for metalist in self.__pointsIter:
            vlist = [[p.value for p in points] for points in metalist ]
            txt = '%s\nConvergence of all points' % self.__tests[i].args
            r.boxplot(vlist, style='quantile', col=self.__color, main=txt, xlab='Iteration index', ylab='Point value')
            r.grid(nx=10, ny=40)
            i += 1
        r.dev_off()


    def __plot_5(self):
        """ Same than plot_4, but instead of showing box for any point of the iteration, we only select the optimum for each run."""
        fileName = os.path.join(self.__dir, 'convergence_optima.ps')
        r.postscript(fileName, paper='letter')
        i = 0
        for metalist in self.__optimaIter:
            vlist = [[p.value for p in points] for points in metalist ]
            txt = '%s\nConvergence of optima' % self.__tests[i].args
            r.boxplot(vlist, style='quantile', col=self.__color, main=txt, xlab='Iteration index', ylab='Optima value')
            r.grid(nx=10, ny=40)
            i += 1
        r.dev_off()

    def __plot_6(self):
        """ Plot the graph of success rates, considering a Test as a success when the given precision of the problem is reached."""
        fileName = os.path.join(self.__dir, 'success_graph.ps')
        r.postscript(fileName, paper='letter')
        slist = []
        for test in self.__tests:
            slist.append(test.succRate*100)
        r.plot(slist, type='n', main='Success rate for each test', xlab='Test index', ylab='Rate (%)')
        r.points(slist, pch = 21, type='h')
        r.points(slist, pch = 21)
        r.grid(nx=10, ny=40)
        r.dev_off()        

    def __plot_7(self):
        """ Plot graph of convergences, that is for each Test the view of all runs convergence over iterations."""
        import random
        fileName = os.path.join(self.__dir, 'convergence_graph.ps')
        r.postscript(fileName, paper='letter')
        olist = []
        # to give a color to each run        
        # new page for each test
        for test in self.__tests:
            # initialize plotting window with first run
            lineType = 1 # cf R, help(par), line type option
            olist = [sub[0].value for sub in test.optimaIterations]
            txt = '%s\nConvergence of the runs over iterations' % test.args
            r.plot(olist, type='o', lty=lineType, ylim=[0,0.5], main=txt, ylab='Value', xlab='Run index')            
            # then iter over the runsNb, plotting their graphs
            for i in range(test.runsNb)[1:]:
                if i < 7:
                    lineType += 1
                else:
                    lineType = 0
                olist = [sub[i].value for sub in test.optimaIterations]
                r.lines(olist, type='o', lty=lineType)
            r.grid(nx=10, ny=40)
        r.dev_off()
    
    
    def __plot_8(self):
        """ Plot optima and the optimum in their neighborhood plan
        for dimension 2
        => use ACP when dim > 2 ! """

        fileName = os.path.join(self.__dir, 'solutions_space.ps') 
        r.postscript(fileName, paper='letter')            
        for t in self.__tests:        
            if t.problem.dimension < 2:

                x = range(len(t.optima))
                y = []

                for p in t.optima:
                    y.append(p.coords[0])
                y.sort()
                
                xlimm = [0, len(t.optima)]
                ylimm = [t.problem.optimum[0].coords[0]-.1, max(y) ]
                txt = '%s\nPoints ordered positions' % t.args
                r.plot(x, y, xlim=xlimm, ylim=ylimm, xlab = 'Points, from best to worst', ylab='Position', \
                       main=txt, bg='white', pch=21, type='o')
                opt = t.problem.optimum[0].coords[0]                
                r.lines(xlimm, [opt, opt], col='black', type='o')
                r.grid(nx=10, ny=40)
                
            elif t.problem.dimension == 2:
                import matrix
                a = matrix.ACP()
                co = [p.coords for p in t.optima]
                a.setMatrix(co)
                x = []
                y = []
                for i in range(len(co)):
                    x.append(a.reduceDim(i,2)[0])
                    y.append(a.reduceDim(i,2)[1])
                xlimm = [t.problem.min_bound[0].coords[0], t.problem.max_bound[0].coords[0]]
                ylimm = [t.problem.min_bound[0].coords[1], t.problem.max_bound[0].coords[1]]
                txt = '%s\nSolutions positions' % t.args
                r.plot(x,y, bg='white', pch=21, xlab='X', ylab='Y', \
                       main=txt, xlim=xlimm, ylim=xlimm)                
                r.points([t.problem.optimum[0].coords[0]], \
                         [t.problem.optimum[0].coords[1]], \
                         bg='black', pch=21)
                r.grid(nx=10, ny=40)
            elif t.problem.dimension > 2:
                # use ACP to fill x and y with principal comp'
                x = []
                y = []
                for p in t.optima:
                    x.append(p.coords[0])
                    y.append(p.coords[1])
                xlimm = [t.problem.min_bound[0].coords[0], t.problem.max_bound[0].coords[0]]
                ylimm = [t.problem.min_bound[0].coords[1], t.problem.max_bound[0].coords[1]]
                txt = '%s\nSolutions positions using PCA' % t.args
                r.plot(x,y, bg='white', pch=21, xlab='X', ylab='Y', \
                       main=txt, xlim=xlimm, ylim=xlimm)                
                r.points([t.problem.optimum[0].coords[0]], \
                         [t.problem.optimum[0].coords[1]], \
                         bg='black', pch=21)
                r.grid(nx=10, ny=40)
        r.dev_off()


    def plot(self):
        """ Plot results as postscript files. """
        ## plot frequency distributions
        self.__plot_1()

        ## plot a box for each sublist
        self.__plot_2()
        
        ## plot the graph of optimas, selecting the best among #runs of each Test, to finally have one Point for each Test.       
        self.__plot_3()
        
        ## plot convergence boxes for all points in iterations
        self.__plot_4()

         ## plot convergence boxes for optima points in iterations
        self.__plot_5()
        
        ## plot success rates
        self.__plot_6()

        ## plot convergence graphs superposed for each run, for each test
        self.__plot_7()

        # plot points in plan
        self.__plot_8()

    def writeLatex(self):
        """ Write Latex report """
        path = os.path.join(self.__dir, 'report.tex')
        fd = open(path, 'w')

        fd.write('\\documentclass{report}\n\\begin{document}\n')
        fd.write('\\section*{OMETAHLAB REPORT}\n\\subsection*{Tests}\n') 
        for test in self.__tests:
            txt = '%s\\\\\n' % test.args
            fd.write(txt)

        for test in self.__tests:
            txt = '\\section*{%s}\n' % test.args
            fd.write(txt)
            # Problem subsection
            fd.write('\\subsection*{Problem}\n\\begin{description}\n')
            txt = '\t\\item[Name:] %s\n\t\\item[Dimension:] %i\n\t\\item[Optimum value:] %s\n' \
                  % (test.problem.name, test.problem.dimension, min([x.value for x in test.problem.optimum]))
            fd.write(txt)
            txt = '\t\\item[Accuracy:] %s\n\\end{description}\n' % test.problem.accuracy
            fd.write(txt)
            # Metaheuristic subsection
            fd.write('\\subsection*{Metaheuristic}\n\\begin{description}\n')
            txt = '\t\\item[Key:] %s \n\t\\item[Name:] %s\n\t\\item[Description:] %s\n\t\\item[Family:] %s\n' \
                  % (test.metah.key, test.metah.name, test.metah.description, test.metah.family)                        
            fd.write(txt)
            fd.write('\\end{description}\n')
            # Parameters subsection
            fd.write('\\subsection*{Parameters}\n\\begin{description}\n')
            txt = '\t\\item[Runs:] %i \n\t\\item[Sample size:] %s\n\t\\item[Treshold:] %s\n\t\\item[Random seed:] %s\n' \
                  % (test.runsNb, test.parameters.sampleSize, \
                     test.parameters.treshold, test.parameters.randomSeed)                        
            fd.write(txt)
            fd.write('\\end{description}\n')
            # Results subsection
            vals = [p.value for p in test.optima]
            fd.write('\\subsection*{Results}\n\\begin{description}\n')
            txt = '\t\\item[Optima mean value:] %f \n\t\\item[Optima standard deviation:] %f\n' \
                  % (r.mean(vals), r.sd(vals))                        
            fd.write(txt)
            txt = '\t\\item[Optima value found:] %f \n\t\\item[Success rate:] %f\n' \
                  % (min(vals), 100*test.succRate)                        
            fd.write(txt)            
            fd.write('\\end{description}\n')

        fd.write('\\end{document}\n')
        fd.close()


        
    def getDir(self):
        """ Returns the path of the directory containing files created."""
        return self.__dir



if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
