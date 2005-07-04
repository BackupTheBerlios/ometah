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
import qparser


class Test:
    """ A Test is a set of executions (runs) of the same command line. """

    # turned to 1 when problem string updated
    _INFO_PB = False
    # turned to 1 to archive XML
    _XML_ARCH = False

    def __init__(self):        
        """ Constructor. """
        import os
        # list of the N optimas, have to sort it after
        self.optima = []
        # list of sublists, one for each run, containing Point objects divided in lists of iterations
        self.__points = []
        # list of Points sorted by iteration, for any points of all runs
        self.pointsIterations = []
        # list of Points sorted by iteration,
        # each one containing the optimum for each run
        self.optimaIterations = []
        # the initial argv as a list
        self.__argv = []
        # and as a string
        self.args = ''
        # directory where to put report, plottings...
        self.__dir = "."
        # one log for each test, grouping all runs informations
        self.__logfile = "run.log"
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
        # list of effective nb of evaluations (size = runsNb)
        self.evaluations = []
        
        try: # create results dir if do not exists
            os.listdir(self.__results_dir)
        except:
            os.mkdir(self.__results_dir)
        
        self.problem = None
        self.parameters = None
        self.metah = None

        # set default arguments
        self.setArgs('')


    def __init(self, runb):
        """ Initialize a Test, which can be sawn as a 'metarun', a set of several runs (default : 25).
        Run runb-th Ometah run  with given argv arguments, logFile as the name of the log file.
        Then returns the Interface instance used.
        """
        import os
        slog = "\nRun %i :\n" % (runb)
        self.__log(slog)

        try:
            fd = os.popen(self.__argv)
        except:
            self.__fatal('can\'t execute ometah, maybe you gave a wrong path, or ometah was not compiled.')

        if self._XML_ARCH:
            xmlName = 'run%i' % (runb)
            fileOut = self.__copyToDisk(fd, filename=xmlName)
            fd.close()
            fd = open(fileOut, 'r')        

        # check that ometah did not return usage infos
        if fd.readline().find('xml-version="1.0"') < 0:
            self.__fatal('ometah failed to create XML\n')
    
        # Loading bar        
        for i in xrange(self.runsNb):
            print '\b\b', # delete previous bar
        for i in xrange(runb):
            print '\b|',  # write as char as runNumber
        for i in xrange(self.runsNb - 1 - runb):
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
            self.opt_val = min( [ p.value for p in self.problem.optimum ] ) 
            self._INFO_PB = True

        self.__points.append(q.getPoints())
        self.evaluations.append(q.getEvaluations())
        #self.optima.append(self.__getOptimum(runb))
        fd.close()        
    
    
    def __metarun(self):
        """ Make a test, that is running Ometah RunsNb times with the same command line arguments.
        A serialized object of the self instance is created in the test directory """        
        import cPickle
        
        #print "\nRunning ometah", ''.join(self.__argv)
        print '\nRunning %s' % self.args
        
        # to initialize loading bar
        for j in xrange(self.runsNb-1): 
            print '\b-',

        # make the runsNb runs
        for i in xrange(self.runsNb):
            self.__init(i)
        
        if self._XML_ARCH:
            self.__archiveXml()        

        (i, ok) = (1, 0)        
        while not ok:
            ok = 1
            self.__dir = '%s_%s_d%i_e%s_r%s_%i' \
                  % (self.problem.name, self.metah.key, \
                     self.problem.dimension, self.parameters.maxEvaluations, \
                     self.parameters.randomSeed, i)
            self.__dir = os.path.join(self.__results_dir, self.__dir)
            try:
                os.mkdir(self.__dir)
            except:
                (i, ok) = (i+1, 0)            

        self.__argv = None
                
        try:
            import copy            
            cPickle.dump( copy.deepcopy(self), open('TEST', 'w'))
        except:
            self.__fatal('pickle failed [Test.metarun]')

        # move files, including XML if was created
        files = ['TEST', self.__logfile]
        if self._XML_ARCH:            
            files.append('xml.tar.gz')
        for src in files:
            os.rename(src, os.path.join(self.__dir, src) )
            

    def setIterationLists(self):
        """ Defines the iteration lists. """
        
        size = self.parameters.sampleSize        
        # initialize the max nb of iterations, some runs may have stopped before reaching max #iter,
        # len(run) is the number of sublists in run, and there is one sublist for each run
        maxiters = max( [ len(run) for run in self.__points ] ) 
        
        self.pointsIterations = [ [] for i in xrange(maxiters) ]
        self.optimaIterations = [ [] for i in xrange(maxiters) ]

        for iter in range(maxiters):
            for run in self.__points:                
                # find the optimum for the current run in the current iteration
                minp = qparser.Point()
                minp.value = 1000
                if len(run) > iter:
                    for point in run[iter]:
                        # add error attribute value
                        point.error = point.value - self.opt_val
                        # add each point to pointsIterations
                        if point.value < minp.value:
                            minp = point
                        self.pointsIterations[iter].append(point)
                    # only add optimum to optimaIterations
                    self.optimaIterations[iter].append(minp)


    def __getOptimum(self, i):
        """ Returns the Point object which has the smallest value (minimum) for run i. """
        optim = self.__points[i][0][0]
        for iteration in self.__points[i]:
            for point in iteration:
                if point.value < optim.value:
                    optim = point
        return optim

    def setOptimaList(self):
        """ """
        for i in xrange(self.runsNb):
            optim = self.__points[i][0][0]
            for iteration in self.__points[i]:
                for point in iteration:
                    if point.value < optim.value:
                        optim = point
            self.optima.append(optim)

    def calculSuccessRates(self):
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

    def __copyToDisk(self, rfd, filename="xml"):
        """ Copy the file opened with rfd to a new file on disk, is used to copy XML output on disk. """
        filename += '.xml'
        xfile = os.path.join(self.__path, filename)
        try:
            wfd = open(xfile, 'w')
        except:
            self.log('ERROR : cannot create file, maybe directory does not exists... [Test.copyToDisk]\n')
        try:
            wfd.write(rfd.read())
        except:
            self.log('ERROR : cannot write file [Test.copyToDisk]\n')
        wfd.close()
        return xfile

    def __archiveXml(self):
        """ Create a compressed tar archive of XML files. """
        try:
            import tarfile
            path = 'xml.tar.gz'
            tf = tarfile.open(name=path, mode='w:gz')
            for s in os.listdir(self.__path):
                if s[-4:] == '.xml':
                    f = os.path.join(self.__path, s)
                    tf.add(f)
                    os.remove(f)
            tf.close()
        except:
            self.__fatal('cannot create XML archive')


    def __log(self, astring):
        """ Log the given string in the log file, appending to the end of the file. """    
        fd = open(self.__logfile, 'a')
        fd.write(astring)
        fd.close()

    def __fatal(self, msg):
        import sys
        print 'FATAL ERROR: %s\n' % msg
        sys.exit(-1)

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
        self.args = 'ometah ' + args
        self.__argv = "%s %s" % (self.__ometah_path, ''.join([''] + [args]))

    def setOmetahPath(self, path):
        """ Set the path to Ometah binary file, which is set to '../ometah/ometah' as default value."""
        self.__ometah_path = path

    def getPath(self):
        """ Returns the path of the working directory created, which can then be given to ometahstats.compare function."""
        return str(self.__dir)

    def start(self):
        """ Start the test, making a metarun, which is running Ometah NB_RUN  times with the same command line arguments."""
        self.__metarun()

    def getDict(self):
        return self.__dict__

    def __getState__(self):
        """ Returns the object to pickle, without unused parameters. """
        import copy
        d = self.__dict__.copy()
        del d['_Test_argv']
        del d['_Test_logfile']
        return d

if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
