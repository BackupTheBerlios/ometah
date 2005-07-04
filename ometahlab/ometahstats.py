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

try:
    import psyco
    psyco.full()
except:
    pass

from rpy import *


def stat(paths):
    """ Generate text report and graphic representation in postscript format,
    comparing the previous Ometah execution which directories' paths are given as a list of strings,
    ometahtest's function getPath can be used to get those strings. See demoscript for usage example."""
    c = Stater(paths)
    c.check()
    print '\nCreating graphics and report...'
    c.plot()
    c.writeLatex()
    print 'Results in %s\n' % c.getDir()


class Stater:
    """ A comparison between several Ometah executions (Test instances), giving text and graphical reports. """

    def __init__(self, paths):
        """ Constructor, paths is a list of strings, which are paths to Ometah executions directories."""
        import cPickle, os
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
        # eigen vectors weight for the tests
        self.__eigenv = []
        # load serialized objects in paths
        for p in paths:
            try:
                fd = open( os.path.join(p, 'TEST'), 'r' )
                s = cPickle.load( fd )
                self.__tests.append(s)
                fd.close()
            except:
                msg = 'error while loading test'
                self.__fatal(msg)
        self.__LOG = 1

        # initialize Test's Points lists
        for test in self.__tests:
            test.setOptimaList()
            test.setIterationLists()
            test.calculSuccessRates()

        # initialize optimas and points
        self.__optimas = [test.optima for test in self.__tests]
        self.__pointsIter = [test.pointsIterations for test in self.__tests]
        self.__optimaIter = [test.optimaIterations for test in self.__tests]        
        
        # create working directory
        (i, ok) = (1, 0)
        while not ok:
            ok = 1
            dir = os.path.join( paths[0].split('/')[0], 'results_%i' % i)
            try:
                os.mkdir(dir)
            except:
                (i, ok) = (i+1 ,0)
        self.__dir = dir


    def check(self):
        """ Check the coherence of Ometah executions, fatal error or a warning is given."""
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
        breaks = 10 # nb breaks in histo, may be reduced if not enough points
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
        try:
            r.boxplot(vlist, style='quantile', log="y", col=self.__color, main='Optimas list', xlab='Test index', ylab='')
        except:
            print 'Cannot use logarithmic scale'
            self.__LOG = 0
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

        try:
            r.plot(olist, type='n', log="y", main='Bests optima evolution', xlab='Test index', ylab='Optima value')
            r.lines(olist)
            r.points(olist, bg = 'white', pch = 21)
            r.grid(nx=10, ny=40)
            r.plot(wlist, type='n', log="y", main='Worsts optima evolution', xlab='Test index', ylab='Optima value')
            r.lines(wlist)
            r.points(wlist, bg ='white', pch = 21)
            r.grid(nx=10, ny=40)
            r.plot(mlist, type='n', log="y", main='Median optima evolution', xlab='Test index', ylab='Optima value')        
            r.lines(mlist)
            r.points(mlist, bg ='white', pch = 21)
            r.grid(nx=10, ny=40)

            r.matplot(r.cbind(olist, mlist, wlist), log="y", type='n', main='Optima evolution: worst, median, and best', xlab='Test index', ylab='Value')
        except:
            if self.__LOG:
                self.__LOG = 0
                print 'Cannot use logarithmic scale'
            r.plot(olist, type='n', main='Bests optima evolution', xlab='Test index', ylab='Optima value')
            r.lines(olist)
            r.points(olist, bg = 'white', pch = 21)
            r.grid(nx=10, ny=40)
            r.plot(wlist, type='n', main='Worsts optima evolution', xlab='Test index', ylab='Optima value')
            r.lines(wlist)
            r.points(wlist, bg ='white', pch = 21)
            r.grid(nx=10, ny=40)
            r.plot(mlist, type='n', main='Median optima evolution', xlab='Test index', ylab='Optima value')        
            r.lines(mlist)
            r.points(mlist, bg ='white', pch = 21)
            r.grid(nx=10, ny=40)
            r.matplot(r.cbind(olist, mlist, wlist), log="y", type='n', main='Optima evolution: worst, median, and best', xlab='Test index', ylab='Value')

        r.points(olist, bg ='white', pch = 21, type='o')
        r.points(mlist, bg ='white', pch = 22, type='o', lty='dotted')
        r.points(wlist, bg ='white', pch = 23, type='o', lty='dashed')
        r.grid(nx=10, ny=40)
        r.dev_off()


    def __plot_4(self):
        """ For each Test, plot the sequence of iterations (for any run) as a set of quantile boxes.
        So we have one graphic for each Test. """
        fileName = os.path.join(self.__dir, 'convergence_points.ps')
        r.postscript(fileName, paper='letter')
        i = 0
        for metalist in self.__pointsIter:
            vlist = [[p.value for p in points] for points in metalist ]
            txt = '%s\nConvergence of all points' % self.__tests[i].args
            try:
                r.boxplot(vlist, style='quantile', col=self.__color, log="y", main=txt, xlab='Iteration index', ylab='Point value')
            except:
                if self.__LOG:
                    self.__LOG = 0
                    print 'Cannot use logarithmic scale'
                r.boxplot(vlist, style='quantile', col=self.__color, log="y", main=txt, xlab='Iteration index', ylab='Point value')
            r.grid(nx=10, ny=40)
            i += 1
        r.dev_off()


    def __plot_5(self):
        """ Same than plot_4, but instead of showing box for any point of the iteration,
        we only select the optimum for each run."""
        fileName = os.path.join(self.__dir, 'convergence_optima.ps')
        r.postscript(fileName, paper='letter')
        i = 0
        for metalist in self.__optimaIter:
            vlist = [[p.value for p in points] for points in metalist ]
            txt = '%s\nConvergence of optima' % self.__tests[i].args
            try:
                r.boxplot(vlist, style='quantile', col=self.__color, log="y", main=txt, xlab='Iteration index', ylab='Optima value')
            except:
                if self.__LOG:
                    self.__LOG = 0
                    print 'Cannot use logarithmic scale'
                r.boxplot(vlist, style='quantile', col=self.__color, main=txt, xlab='Iteration index', ylab='Optima value')
            r.grid(nx=10, ny=40)
            i += 1
        r.dev_off()

    def __plot_6(self):
        """ Plot the graph of success rates, considering a Test as a success when the given
        precision of the problem is reached."""
        fileName = os.path.join(self.__dir, 'success_graph.ps')
        r.postscript(fileName, paper='letter')
        slist = []
        slist = [t.succRate*100 for t in self.__tests]
        r.plot(slist, type='n', main='Success rate for each test', xlab='Test index', ylab='Rate (%)')
        r.points(slist, pch = 21, type='h')
        r.points(slist, pch = 21)
        r.grid(nx=10, ny=40)
        r.dev_off()        

    def __plot_7(self):
        """ Plot graph of convergences:  for each Test the view of all runs convergence over iterations."""

        """
        fileName = os.path.join(self.__dir, 'convergence_graph.ps')
        r.postscript(fileName, paper='letter')
        olist = []
        for test in self.__tests:
            # initialize plotting window with first run
            lineType = 1 # cf R, help(par), line type option
            it = test.optimaIterations
            olist = [sub[0].value for sub in it]
            txt = '%s\nConvergence of the runs over iterations' % test.args
            r.plot(olist, type='o', lty=lineType, ylim=[0,0.5], main=txt, ylab='Value', xlab='Run index')            
            # then iter over the runsNb, plotting their graphs
            for i in range(test.runsNb)[1:]:
                if i < 7:
                    lineType += 1
                else:
                    lineType = 0                    
                olist = [sub[i].value for sub in it[:end]]
                print i
                r.lines(olist, type='o', lty=lineType)
            r.grid(nx=10, ny=40)
        r.dev_off()
        """
    
    def __plot_8(self):
        """ Plot optima and the optimum in their neighborhood plan, PCA used if dimension > 2 """
        fileName = os.path.join(self.__dir, 'solutions_space.ps') 
        r.postscript(fileName, paper='letter')            
        for t in self.__tests:        
            if t.problem.dimension < 2:

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
                    if max(x) < xmax:
                        xmax = 0.8 * max(x) + 0.2 * xmax
                    if max(y) < ymax:
                        ymax = 0.8 * max(y) + 0.2 * ymax

                    xlimm = [xmin, xmax]
                    ylimm = [ymin, ymax]
                    
                else:
                    import matrix
                    a = matrix.PCA()
                    # append solution Points
                    co = [p.coords for p in t.optima]
                    # also add bounds and optimum
                    co.append(t.pb_min_bound[0].coords)
                    co.append(t.pb_max_bound[0].coords)
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
                    
                    # add optimum points to point list, if it has an extrema value 
                    x.append(xoptim)
                    y.append(yoptim)

                    xlimm = [min(x), max(x)]
                    ylimm = [min(y), max(y)]
                
                txt = '%s\nSolutions positions' % t.args
                r.plot(x,y, bg='white', pch=21, xlab='X', ylab='Y', \
                       main=txt, xlim=xlimm, ylim=ylimm)                

                r.points([xoptim], \
                         [yoptim], \
                         bg='black', pch=21)
                r.grid(nx=10, ny=40)
           
        r.dev_off()


    def __plot_9(self):
        """ Make a non-parametric test over sets of optima errors,
        Mann-Whitney if 2 tests, Kruskal-Whitney if 3 or more.
        p.value < 0.05 => no difference
        p.value > 0.05 => difference
        Returns True if """
        import Numeric
        limit = .05
        # initialize our matrix with as sublists as tests
        emlist = [ [] for i in self.__tests ]
        fileName = os.path.join(self.__dir, 'distribution_errors.ps')        
        r.postscript(fileName, paper='letter')
        breaks = 10
        
        for i in xrange(len(self.__optimas)):
            # for current test's optima list, add their error as a list at emlist[i]
            emlist[i] = [p.value - self.__tests[i].opt_val for p in self.__optimas[i]]
            txt = '%s\nOptima error distribution' % self.__tests[i].args
            r.hist(emlist[i], breaks, col=self.__color, main=txt, xlab='Error', ylab='Frequency')
            r.grid(nx=10)
        r.dev_off()

        if len(self.__tests) == 2:
            # use Mann-Whitney test
            dic = r.wilcox_test(Numeric.array(emlist))
        elif len(self.__tests) > 2:
            # use Kruskal-Wallis test
            dic = r.kruskal_test(emlist)
        else: # only one test
            return
        self.__same_distrib = dic['p.value']


    def __plot_10(self):
        """ Convergence graph over iterations : plot median error of each run for a given iteration,
        using logarithmic scale. """
        fileName = os.path.join(self.__dir, 'convergence_error_all.ps')
        r.postscript(fileName, paper='letter')
        i = 0
        for list in self.__pointsIter:
            elist = [[p.error for p in points] for points in list ]
            errlist = [r.median(list) for list in elist]
            txt = '%s\nConvergence of error of all points' % self.__tests[i].args
            try:
                r.plot(errlist, main=txt, type='o', ylab='Median error', xlab='Iteration', log="y")
            except:
                if self.__LOG:
                    self.__LOG = 0
                    print 'Cannot use logarithmic scale'
                r.plot(errlist, main=txt, type='o', ylab='Median error', xlab='Iteration')
            r.grid(nx=10, ny=40)
            i += 1
        r.dev_off()


    def __plot_11(self):
        """ Convergence graph over iterations : plot median error of each run for a given iteration,
        using logarithmic scale. """
        fileName = os.path.join(self.__dir, 'convergence_error_opt.ps')
        r.postscript(fileName, paper='letter')
        i = 0

        for list in self.__optimaIter:
            elist = [[p.error for p in points] for points in list ]
            errlist = [r.median(list) for list in elist]            
            txt = '%s\nConvergence of error of optima' % self.__tests[i].args
            try:
                r.plot(errlist, main=txt, type='o', ylab='Error', xlab='Iteration', log="y")
            except:
                if self.__LOG:
                    self.__LOG = 0
                    print 'Cannot use logarithmic scale'
                r.plot(errlist, main=txt, type='o', ylab='Error', xlab='Iteration')
            r.grid(nx=10, ny=40)
            i += 1
        r.dev_off()
        
        
    def plot(self):
        """ Plot results as postscript files. """
        # frequency distributions
        self.__plot_1()
        # a quantile box for each sublist
        self.__plot_2()        
        # graph of optimas, selecting the best among #runs of each Test, to finally have one Point for each Test.       
        self.__plot_3()        
        # convergence boxes for all points in iterations
        self.__plot_4()
        # convergence boxes for optima points in iterations
        self.__plot_5()        
        # success rates
        self.__plot_6()
        # convergence graphs superposed for each run, for each test
        #self.__plot_7()
        # points in plan
        self.__plot_8()
        # optima's error distribution
        self.__plot_9()
        # error convergence for all points
        self.__plot_10()
        # error convergence for optima
        self.__plot_11()


    def writeTex(self):
        """ Write Latex report """
        
        
    def writeLatex(self):
        """ Write Latex report """
        path = os.path.join(self.__dir, 'report.tex')
        fd = open(path, 'w')
        W = fd.write

        W('\\documentclass[]{report}\n\\begin{document}\n')
        W('\\section*{OMETAHLAB REPORT}\n')
        
        for test in self.__tests:
            txt = '%s\\\\\n' % test.args
            W(txt)

        W('\\\\\n')
        txt = '\\begin{tabular}[t]{ccccccccc}\n'
        W(txt)
        cols = ['Problem', 'Algo', 'Evaluations', 'Precision', 'Optimum', 'Mean', 'Std', 'Success Rate']
        txt = ''.join(['\\textbf{%s} &' % s for s in cols[:-1]])
        txt = '%s \\textbf{%s}\\\\\n' % (txt, cols[-1])
        W(txt)

        for t in self.__tests:
            vals = [p.value for p in test.optima]
            vmin = str(min(vals))[:5]
            vmean = str(r.mean(vals))[:5]
            vsd = str(r.sd(vals))[:5]
            suc = str(100*t.succRate)[:5]
            txt = '%s & %s & %s & %s & %s & %s & %s & %s \\\\\n' % \
                  (t.problem.name, t.metah.key, t.parameters.maxEvaluations,
                   t.parameters.treshold, vmin, vmean, vsd, suc)
            W(txt)
        W('\\end{tabular}\n')

        if len(self.__tests) > 1:
            txt = '\n\\paragraph{Non-parametric test over optima errors}\n%f\n' % self.__same_distrib
            W(txt)

        if self.__eigenv != []: # <=> if dimension > 2
            W('\n\\paragraph{Eigen vectors:}\n')
            for e in self.__eigenv:
                txt = '%s\\\\\n' % (str(e))
                W(txt)                        

        """
        for test in self.__tests:
            txt = '\\section*{%s}\n' % test.args
            W(txt)
            # Problem subsection
            W('\\subsection*{Problem}\n\\begin{description}\n')
            txt = '\t\\item[Name:] %s\n\t\\item[Dimension:] %i\n\t\\item[Optimum value:] %s\n' \
                  % (test.problem.name, test.problem.dimension, min([x.value for x in test.pb_optimum]))
            W(txt)
            txt = '\t\\item[Accuracy:] %s\n\\end{description}\n' % test.problem.accuracy
            W(txt)
            # Metaheuristic subsection
            W('\\subsection*{Metaheuristic}\n\\begin{description}\n')
            txt = '\t\\item[Key:] %s \n' % (test.metah.key)
            W(txt)
            W('\\end{description}\n')
            # Parameters subsection
            W('\\subsection*{Parameters}\n\\begin{description}\n')
            txt = '\t\\item[Runs:] %i \n\t\\item[Sample size:] %i\n\t\\item[Treshold:] %f\n\t\\item[Random seed:] %i\n' \
                  % (test.runsNb, test.parameters.sampleSize, test.parameters.treshold, test.parameters.randomSeed)                        
            W(txt)
            W('\\end{description}\n')
            # Results subsection
            vals = [p.value for p in test.optima]
            W('\\subsection*{Results}\n\\begin{description}\n')
            txt = '\t\\item[Optima mean value:] %f \n\t\\item[Optima standard deviation:] %f\n' \
                  % (r.mean(vals), r.sd(vals))                        
            W(txt)
            txt = '\t\\item[Optima value found:] %f \n\t\\item[Success rate:] %f \n\t\\item[Mean evaluations:] %f\n' \
                  % (min(vals), 100*test.succRate, r.mean(test.evaluations))                        
            W(txt)            
            W('\\end{description}\n')

        """
        W('\\end{document}\n')
        fd.close()
        
    def getDir(self):
        """ Returns the path of the directory containing files created."""
        return self.__dir


if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
