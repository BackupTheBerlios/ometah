#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
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
    _INFO_PB = 0

    def __init__(self):        
        """ Constructor. """
        # list of runsNb Points
        self.optima = []
        # list of all Points
        self.__points = []
        # list of Points sorted by iteration, for any points of all runs
        self.pointsIterations = []
        # list of Points sorted by iteration,
        # each one containing the optimum for each run
        self.optimaIterations = []
        # the initial argv as a list
        self.argv = ['']
        # and as a string
        self.args = ''
        # directory where to put report, plottings...
        self.__dir = "."
        # one log for each test, grouping all runs informations
        self.__logName = "ometahtest.log"
        # default location of ometah
        self.__ometah_path = os.path.join('..', os.path.join('ometah', 'ometah'))
        # common dir for all results
        self.__results_dir = "labResults"
        # precision for success rates calculs
        self.__precision = 0.001
        # success rate
        self.succRate = 0
        # success performance = mean(FES for successful run * #run)/#succRuns
        self.succPerf = 0
        # number of runs for the test
        self.runsNb = 25

        self.problem = None
        self.parameters = None
        self.metah = None

    def __init(self, argv, runNumber, logFile):
        """ Initialize a Test, which can be sawn as a 'metarun', a set of several runs (default : 25).
        Run runNumber-th Ometah run  with given argv arguments, logFile as the name of the log file.
        Then returns the Interface instance used.
        """
        import parser
        import interface
        intfc = interface.Interface(argv)
        intfc.setLog(1)
        intfc.setLogFileName(logFile)
        intfc.log("\n\n---------------------------------------------\n")
        slog = "[Starting run %i ]" % (runNumber)
        intfc.log(slog)
        intfc.log("\n---------------------------------------------\n\n")
        fd = intfc.getXmlFromExecOmetah(self.__ometah_path)
        xmlName = "run%i" % (runNumber)
        fileOut = intfc.copyToDisk(fd, filename=xmlName)
        fd.close()
        fd = open(fileOut, 'r')        
        if not 'xml-version="1.0"' in fd.readline():
            intfc.log('ERROR : ometah failed to create XML\n')
            intfc.fatal('(see log file)')

        # Loading bar
        for i in range(self.runsNb):
            print '\b\b', # delete previous bar

        for i in range(runNumber):
            print '\b|',  # write as char as runNumber
        for i in range(self.runsNb - 1 - runNumber):
            print '\b-',  # padd with default char
        sys.stdout.flush()

        # parse and read XML
        XmlParser = parser.XMLParser(fd, intfc)
        XmlParser.readXml()
        fd.close()
        intfc.log('parsing XML : readXML() ... OK\n')
        intfc.setPoints(XmlParser.getPoints())
        intfc.log('getting points list : getPoints() ...  OK\n')

        # get Test informations, only once (same header for all runs)
        if self._INFO_PB == 0:
            header = XmlParser.getHeader()
            self.problem = header.problem
            self.parameters = header.parameters
            self.metah = header.metah
            self._INFO_PB = 1        

        return intfc
    
    
    def __metarun(self):
        """ Make a test, that is running Ometah RunsNb times with the same command line arguments.
        A serialized object of the self instance is created in the test directory """        
        import pickle
        # list of the N optimas, have to sort it after
        self.optima = []
        # list of sublist, one for each run, containing Point objects
        self.__points = []
        try:
            os.listdir(self.__results_dir)
        except:
            os.mkdir(self.__results_dir)

        print "\nRunning ometah", ''.join(self.argv)
        for j in range(self.runsNb-1): # to initialize loading bar
            print '\b-',
        for i in range(self.runsNb):
            intf = self.__init(self.argv, i, self.__logName)
            self.optima.append(intf.getOptimum())
            self.__points.append(intf.getPoints())            
        intf.archiveXml()
        
        i = 1
        ok = 0
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
                ok = 0
                i = i + 1
        self.__dir = dir
        vlist = [ x.value for x in self.optima ]
        slog = "\n----optima results---------\nmean : %f\n std : %f\n\n" \
               % (r.mean(vlist), r.sd(vlist))
        intf.log(slog)


        # one sublist for each iteration, containing all points of the N runs

        size = int(self.parameters.sampleSize)
        # initialize the length of s.points
        iters = len(self.__points[0]) / size        
        for i in range(iters):
            self.pointsIterations.append([])       
            self.optimaIterations.append([])
        
        subindex = -1 # sublist index in followin iteration
        import parser
        # for each sublist (each run)
        for sublist in self.__points:
            # nb of iterations = nb of points / sample size
            subindex += 1
            it = 0 # current iteration
            c = 0
            minp = parser.Point()
            minp.value = 1000
            for p in sublist:
                p.error = p.value - self.problem.optimum[0].value
                self.pointsIterations[it].append(p)
                if p.value < minp.value:
                    minp = p
                c = c + 1
                # when sample size reached, select the optimum for this iteration
                if c == size:
                    self.optimaIterations[it].append(minp)
                    it = it + 1
                    c = 0        

        # give succRate & succPerf their value
        self.__calculSuccessRates()

        # empty large unuseful variables
        self.__points = []
        
        fd = open('TEST', 'w')
        try:
            pickle.dump(self,fd)
        except:
            int.fatal('pickle failed [Test.metarun]')
        fd.close()
        for src in ['xml.tar.gz', 'TEST', self.__logName]:
            tar = os.path.join(self.__dir, src)
            os.rename(src, tar)

    def __calculSuccessRates(self):
        """ Update succRate & succPerf values, according to the current optima list and problem instance """
        total = self.runsNb
        success = 0
        successIndex = []
        for point in self.optima:
            if self.__success(point):
                success += 1
                successIndex.append(point.index)                
        self.succRate = float(success) / float(total)
        if success != 0:
            self.succPerf = r.mean(successIndex)*float(total) / float(success)
        
    def __success(self, point):
        """ Returns true if the point given matches problem's optima,
        with a precision of self.__precision, returns false otherwise """
        vlist = [ float(p.value) for p in self.problem.optimum ]        
        realOptimum = min(vlist)
        optimumFound = float(point.value)
        if (optimumFound - realOptimum) > self.problem.accuracy:
            return False
        return True

    
    def start(self):
        """ Start the test, making a metarun, which is running Ometah NB_RUN  times with the same command line arguments."""
        self.__metarun()


    def setNbRuns(self, n):
        """ Set the number of runs of the test to n. Default value is 25."""
        self.runsNb = n

    def setArgs(self, args):
        """ Set the arguments of Ometah execution, as a string. Ie give '-p Sphere -e 50'."""
        self.argv = [''] + [args]
        self.args = 'ometah ' + ''.join(self.argv)

    def setOmetahPath(self, path):
        """ Set the path to Ometah binary file, which is set to '../ometah/ometah' as default value."""
        self.__ometah_path = path

    def getPath(self):
        """ Return the path of the working directory created, which can then be given to ometahstats.compare function."""
        return str(self.__dir)


if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
