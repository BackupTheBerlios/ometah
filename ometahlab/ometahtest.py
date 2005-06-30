#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
#  File : ometahtest.py
#  This module contains the Test class, which corresponds to a set of ometah
#  executions with the same command line arguments.
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


class Test:
    """ A Test is a set of executions (runs) of the same command line. """

    # turned to 1 when problem string updated
    _INFO_PB = False
    # turned to 1 to archive XML
    _XML_ARCH = False

    def __init__(self):        
        """ Constructor. """
        # list of the N optimas, have to sort it after
        self.optima = []
        # list of sublist, one for each run, containing Point objects
        self.__points = []
        # list of Points sorted by iteration, for any points of all runs
        self.pointsIterations = []
        # list of Points sorted by iteration,
        # each one containing the optimum for each run
        self.optimaIterations = []
        # the initial argv as a list
        self.__argv = ['']
        # and as a string
        self.__args = ''
        # directory where to put report, plottings...
        self.__dir = "."
        # one log for each test, grouping all runs informations
        self.__logName = "ometahrun.log"
        # default location of ometah
        self.__ometah_path = os.path.join('..', os.path.join('ometah', 'ometah'))
        # common dir for all results
        self.__results_dir = "results"
        # real optimum value (min of the optima given for the pb)
        self.opt_val = 0
        # success rate
        self.succRate = 0
        # success performance = mean(FES for successful run * #run)/#succRuns
        self.succPerf = 0
        # number of runs for the test
        self.runsNb = 25

        self.problem = None
        self.parameters = None
        self.metah = None

    def __init(self, runNumber):
        """ Initialize a Test, which can be sawn as a 'metarun', a set of several runs (default : 25).
        Run runNumber-th Ometah run  with given argv arguments, logFile as the name of the log file.
        Then returns the Interface instance used.
        """
        import qparser, os
        import interface
        intfc = interface.Interface()
        intfc.setLog(self.__logName)

        slog = "\nRun %i :\n" % (runNumber)
        intfc.log(slog)

        cmd = "%s %s" % (self.__ometah_path, ''.join(['']+self.__argv[1:]))
        try:
            fd = os.popen(cmd)
        except:
            intfc.log('ERROR : wrong path to ometah [Interface.getXmlFromExecOmetah]\n')            
            intfc.fatal('(see log file)')

        if self._XML_ARCH:
            xmlName = 'run%i' % (runNumber)
            fileOut = intfc.copyToDisk(fd, filename=xmlName)
            fd.close()
            fd = open(fileOut, 'r')        
        
        if not 'xml-version="1.0"' in fd.readline():
            intfc.log('ERROR : ometah failed to create XML\n')
            intfc.fatal('(see log file)')

        # Loading bar        
        for i in xrange(self.runsNb):
            print '\b\b', # delete previous bar

        for i in xrange(runNumber):
            print '\b|',  # write as char as runNumber
        for i in xrange(self.runsNb - 1 - runNumber):
            print '\b-',  # padd with default char
        sys.stdout.flush()
        
        q = qparser.Qparser()
        q.setFd(fd)
    
        # get Test informations, only once (same header for all runs)
        if not self._INFO_PB:
            header = q.getHeader()
            self.problem = header.problem
            self.parameters = header.parameters
            self.metah = header.metah
            vlist = [ p.value for p in self.problem.optimum ]
            self.opt_val = min(vlist) 
            self._INFO_PB = True
            
        self.__points.append(q.getPoints(self.opt_val + self.problem.accuracy))
        fd.close()        

        return intfc
    
    
    def __metarun(self):
        """ Make a test, that is running Ometah RunsNb times with the same command line arguments.
        A serialized object of the self instance is created in the test directory """        
        import pickle, qparser

        try:
            os.listdir(self.__results_dir)
        except:
            os.mkdir(self.__results_dir)

        print "\nRunning ometah", ''.join(self.__argv)
        for j in xrange(self.runsNb-1): # to initialize loading bar
            print '\b-',
            
        for i in xrange(self.runsNb):
            intf = self.__init(i)
            self.optima.append(self.__getOptimum(i))            

        if self._XML_ARCH:
            intf.archiveXml()        
        
        (i, ok) = (1, 0)        
        while not ok:
            ok = 1
            dir = '%s_%s_d%i_e%s_r%s_%i' \
                  % (self.problem.name, self.metah.key, \
                     self.problem.dimension, self.parameters.maxEvaluations, \
                     self.parameters.randomSeed, i)
            dir = os.path.join(self.__results_dir, dir)
            try:
                os.mkdir(dir)
            except:
                (i, ok) = (0, i+1)            
        self.__dir = dir

        self.__setIterationLists()
        self.__calculSuccessRates()

        
        # empty variables for pickle reduction
        self.__points = []
        self.__argv = self.__args = None
        
        fd = open('TEST', 'w')
        try:
            pickle.dump(self,fd)
        except:
            intf.fatal('pickle failed [Test.metarun]')
        fd.close()

        files = ['TEST', self.__logName]
        if self._XML_ARCH:
            files.append('xml.tar.gz')
        for src in files:
            tar = os.path.join(self.__dir, src)
            os.rename(src, tar)

    def __setIterationLists(self):
        """ !!!!!!!!!  CRITIC  -> MODULARISER !!? !!!!!!!!!!! """
        import qparser
        # one sublist for each iteration, containing all points of the N runs
        ###### !!!!!
        # PB : assume that all runs have the same #iteratiosn
        # => PB if optimum found and points stopped
        size = self.parameters.sampleSize
        # initialize the length of s.points
        iters = len(self.__points[0]) / size        
        for i in xrange(iters):
            self.pointsIterations.append([])       
            self.optimaIterations.append([])
        
        subindex = -1 # sublist index in followin iteration
        # for each sublist (each run)
        for sublist in self.__points:
            # nb of iterations = nb of points / sample size
            subindex += 1
            #it is current iteration, c counts points until reach sample size
            c = it = 0             
            minp = qparser.Point()
            minp.value = 1000
            for p in sublist:
                p.error = p.value - self.problem.optimum[0].value
                self.pointsIterations[it].append(p)
                if p.value < minp.value:
                    minp = p
                c += 1
                # when sample size reached, select the optimum for this iteration
                if c == size:
                    self.optimaIterations[it].append(minp)
                    (c, it) = (0, it + 1)                    
        """ !!!!!!!!!  CRITIC  !!!!!!!!!!! """

    def __getOptimum(self, i):
        """ Returns the Point object which has the smallest value (minimum) for run i. """
        optim = self.__points[i][0]
        for point in self.__points[i]:
            if point.value < optim.value:
                optim = point
        return optim

    def __calculSuccessRates(self):
        """ Update succRate & succPerf values, according to the current optima list and problem instance """
        total = self.runsNb
        success = 0
        successIndex = []
        for point in self.optima:
            if self.__success(point):
                success += 1
                successIndex.append(point.index)
        if success != 0:
            self.succRate = float(success) / float(total)
            self.succPerf = r.mean(successIndex)*float(total) / float(success)
        else:
            self.succRate = 0
        
    def __success(self, point):
        """ Returns true if the point given matches problem's optima,
        with a precision of problem's accuracy, returns false otherwise """
        if (point.value - self.opt_val) > self.problem.accuracy:
            return False
        return True

    def setXmlArchive(self, bool):
        """ Set _XML_ARCH var."""
        if bool:
            self._XML_ARCH = True
        else:
            self._XML_ARCH = False

    def setNbRuns(self, n):
        """ Set the number of runs of the test to n. Default value is 25."""
        self.runsNb = n

    def setArgs(self, args):
        """ Set the arguments of Ometah execution, as a string. Ie give '-p Sphere -e 50'."""
        self.__argv = [''] + [args]
        self.args = 'ometah ' + ''.join(self.__argv)

    def setOmetahPath(self, path):
        """ Set the path to Ometah binary file, which is set to '../ometah/ometah' as default value."""
        self.__ometah_path = path

    def getPath(self):
        """ Returns the path of the working directory created, which can then be given to ometahstats.compare function."""
        return str(self.__dir)

    def start(self):
        """ Start the test, making a metarun, which is running Ometah NB_RUN  times with the same command line arguments."""
        self.__metarun()


if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
