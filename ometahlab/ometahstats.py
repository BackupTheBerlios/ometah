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

import glob
from rpy import *

OMETAHLAB_PLUGIN_MARK = 'isOmetahLabPlugin'

def process(paths,plugs=['end_optimum_distribution'],output_type='.ps'):
    """ Generate text report and graphic representation in postscript format,
    comparing the previous Ometah execution which directories' paths are given as a list of strings,
    ometahtest's function getPath can be used to get those strings. See demoscript for usage example."""
    
    print '  Initialization of data structures...'
    s = DataManager(paths)
    print '  Checking data...'
    s.check()
    
    print '  Loading plugins...'
    p = PluginManager(s,'plugins')
    print '    Available: '," ".join( p.available )
    for i in plugs:
        p.load(i)
    print '    Loaded :', " ".join( p.loaded.keys() )
    
    print '  Launching plugins on data...'
    for i in p.loaded:
        p.loaded[i].setFileType(output_type)
        p.loaded[i].process()
    
    print '  Results are in: %s' % s.getDir()


class DataManager:
    """ A comparison between several Ometah executions (Test instances), giving text and graphical reports. """

    def __init__(self, paths):
        """ Constructor, paths is a list of strings, which are paths to Ometah executions directories."""
        import cPickle, os
        # color used in graphic plottings
        self.color = 'grey85'        
        # list of Test objects, one for each metarun
        self.tests = []
        # working directory
        self.dir = ''
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
                self.tests.append(s)
                fd.close()
            except:
                msg = 'error while loading test'
                self.__fatal(msg)
        self.__LOG = 1

        # initialize Test's Points lists
        for test in self.tests:
            test.setOptimaList()
            test.setIterationLists()

            # if optimum is known, calcul success rate
            if test.opt_val != 'Unknown':
                test.calculSuccessRates()

        # initialize optimas and points
        self.optimas = [test.optima for test in self.tests]
        self.pointsIter = [test.pointsIterations for test in self.tests]
        self.optimaIter = [test.optimaIterations for test in self.tests]        
        
        # create working directory
        (i, ok) = (1, 0)
        while not ok:
            ok = 1
            dir = os.path.join( paths[0].split('/')[0], 'results_%i' % i)
            try:
                os.mkdir(dir)
            except:
                (i, ok) = (i+1 ,0)
        self.dir = dir


    def check(self):
        """ Check the coherence of Ometah executions, fatal error or a warning is given."""
        pb = self.tests[0].problem.key
        dim = self.tests[0].problem.dimension
        sample = self.tests[0].parameters.sampleSize
        runsNb = self.tests[0].runsNb
        for test in self.tests:
            if test.problem.key != pb:
                self.__warning('tests have different problems.')
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
        


# TODO in plugins :

    def np_test(self, testIndex1, testIndex2):
        """ Return non-parametric test """
        import Numeric
        tests = [ testIndex1, testIndex2 ]
        for i in tests:
            tlist = [ p.value - self.tests[i].opt_val for p in self.optimas[i] ]
        return r.wilcox_test(Numeric.array(tlist))['p.value']
        


    def writeLatex(self):
        """ Write Latex report """
        path = os.path.join(self.dir, 'report.tex')
        fd = open(path, 'w')
        W = fd.write

        W('\\documentclass[]{report}\n\\begin{document}\n')
        W('\\section*{OMETAHLAB REPORT}\n')
        
        for test in self.tests:
            txt = '%s\\\\\n' % test.args
            W(txt)

        W('\\\\\n')
        txt = '\\begin{tabular}[t]{ccccccccc}\n'
        W(txt)
        cols = ['Problem', 'Algo', 'Evaluations', 'Precision', 'Optimum', 'Mean', 'Std', 'Success Rate']
        txt = ''.join(['\\textbf{%s} &' % s for s in cols[:-1]])
        txt = '%s \\textbf{%s}\\\\\n' % (txt, cols[-1])
        W(txt)

        for t in self.tests:
            vals = [p.value for p in t.optima]
            vmin = str( ( "%.6f" % ( min(vals)) ) )[:10]
            vmean = str(r.mean(vals))[:5]
            vsd = str(r.sd(vals))[:5]
            suc = str(100*t.succRate)[:5]
            txt = '%s & %s & %s & %s & %s & %s & %s & %s \\\\\n' % \
                  (t.problem.name, t.metah.key, t.parameters.maxEvaluations,
                   t.parameters.treshold, vmin, vmean, vsd, suc)
            W(txt)
        W('\\end{tabular}\n')

        if len(self.tests) > 1:
            txt = '\n\\paragraph{Non-parametric test on optima error over all tests}\n%f\n' % self.__same_distrib
            W(txt)

        W('\n\\paragraph{Non parametric tests on optima error over for tests pairs}')
        for i in xrange(len(self.tests)):
            for j in xrange(len(self.tests)):
                if ( i < j ):
                    txt = '\n( %d, %d)\\\\\n%f\\\\' % ( i, j, self.np_test(i, j) )
                    W(txt)

        if self.__eigenv != []: # <=> if dimension > 2
            W('\n\\paragraph{Eigen vectors:}\n')
            for e in self.__eigenv:
                txt = '%s\\\\\n' % (str(e))
                W(txt)                        

        W('\\end{document}\n')
        fd.close()
        
    def getDir(self):
        """ Returns the path of the directory containing files created."""
        return self.dir



class PluginManager:
    """Management of the plugins"""
    def __init__(self,data,pluginPath):
        # available plugins
        self.available = []
        
        # loaded plugins
        self.loaded={}
        
        # the data object to pass to the plugin
        self.data = data
        
        # path where plugins are stored
        self.pluginPath = pluginPath
        
        # look for available plugins
        self.find()
        
        
    def isPlugin(self,f):
        """Test if a file is a plugin, and add it to the available list"""
        #open the source file
        file = open(f,'r')
        code = file.readlines()
        code = str.join(' ',code)
        # if there is the mark
        if code.find(OMETAHLAB_PLUGIN_MARK) > -1 and f != 'plugin.py':
            return True
        return False


    def find(self):
        """Look for plugins in the path"""
        # list python files
        pyFiles = glob.glob( os.path.join(self.pluginPath,'*.py') )
        
        for f in pyFiles:
            if self.isPlugin(f):
                # add to the list
                # (only consider the file name)
                self.available += [os.path.basename(f).split('.')[0]]
       
       
    def load(self,pluginName):
        """Load a plugin"""
        
        if pluginName not in self.available:
            raise "PluginError","Plugin not available : "+pluginName

        if self.loaded.has_key(pluginName):
            return

        # import the plugin
        #print os.path.join(self.pluginPath,pluginName)
        plugin = __import__(os.path.join(self.pluginPath,pluginName))
        pluginClass = getattr(plugin, pluginName)
        self.loaded[pluginName] = pluginClass(self.data)


    def unload(self,pluginName):
        try:
            del self.loaded[pluginName]
        except IndexError:
            print 'No plugin named ' + str(pluginName)



if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
