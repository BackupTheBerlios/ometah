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

try:
    import psyco
    psyco.full()
except:
    pass

from rpy import *


class Serialized:
    """ metarun infos needed for stats functions """
    
    def __init__(self):
        # Problem instance
        self.problem = None
        # Metaheuristic instance
        self.metah = None
        # Parameters instance
        self.parameters = None
        # list of Points
        self.points = []
        # list of Points, only optima
        self.optima = []
        

class Test( object ):
    """ a Test is a set of _NB_RUNS of the same command line """

    # nb of runs for each metarun
    _NB_RUNS = 20

    # turned to 1 when problem string updated
    _INFO_PB = 0

    def __init__(self):        
        """ constructor """
        # list of _NB_RUNS Points
        self.__optima = []
        # list of all Points
        self.__points = []
        # the initial argv
        self.argv = ['']
        # directory where to put report, plottings...
        self.dir = "."
        # one log for each metarun
        self.__logName = "ometahtest.log"
        # location of ometah
        self.__ometah_path = "../ometah/ometah"        

    def init(self, argv, runNumber, logFile):
        """ initialize a metarun  :
        - run ometah
        - parse XML output
        - get Point list and Header instances
        - return the interface created """
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
        print '\b.',
        sys.stdout.flush()
        # parse and read XML
        XmlParser = parser.XMLParser(fd, intfc)
        XmlParser.readXml()
        fd.close()
        intfc.log('parsing XML : readXML() ... OK\n')
        intfc.setPoints(XmlParser.getPoints())
        intfc.log('getting points list : getPoints() ...  OK\n')
        header = XmlParser.getHeader()
        # get Test informations, only once
        if self._INFO_PB == 0:
            self.problem = header.problem
            self.parameters = header.parameters
            self.metah = header.metah
            self._INFO_PB = 1        
        intfc.setTitle(header.toString())

        return intfc
    
    
    def metarun(self):
        """ make a metarun ~ run ometah with argv N times """        
        import pickle
        # list of the N optimas, have to sort it after
        self.__optima = []
        # list of sublist, one for each run, containing Point objects
        self.__points = []

        print "\nRunning ometah", ''.join(self.argv)
        for i in range(self._NB_RUNS):
            intf = self.init(self.argv, i, self.__logName)
            self.__optima.append(intf.getOptimum())
            self.__points.append(intf.getPoints())            

        intf.log(intf.getTitle())
        
        i = 1
        ok = 0
        while not ok:
            ok = 1
            dir = '%s_%s_d%i_e%s_r%s__%i' \
                  % (self.problem.name, self.metah.key, \
                     self.problem.dimension, self.parameters.maxEvaluations, \
                     self.parameters.randomSeed, i)
            try:
                os.mkdir(dir)
            except:
                ok = 0
                i = i + 1

        self.__dir = dir
        vlist = [ x.value for x in self.__optima ]
        slog = "\n----optima results---------\nmean : %f\n std : %f\n\n" \
               % (r.mean(vlist), r.sd(vlist))
        intf.log(slog)

        # Create the Serialized object
        s = Serialized()
        s.nbRuns = self._NB_RUNS
        s.args = 'ometah' + ''.join(self.argv)
        s.problem = self.problem
        s.parameters = self.parameters
        s.metah = self.metah
        s.optima = self.__optima

        # s.points is a set of Point sublists :
        # one sublist for each iteration, containing all points of the N runs
        s.points = []
        ssize = int(self.parameters.sampleSize) / 2
        # initialize the length of s.points
        iters = len(self.__points[0]) / ssize        
        for i in range(iters):
            s.points.append([])
        #print "len(points)", len(self.__points[0]), "iterations : ", iters
        
        # for each sublist (each run)
        for sublist in self.__points:
            # nb of iterations = nb of points / sample size
            iters = len(sublist) / ssize
            it = 0 # current iteration
            c = 0
            for p in sublist:
                s.points[it].append(p)
                c = c + 1
                if c == ssize:
                    it = it + 1
                    c = 0
        
        fd = open('SERIALIZED', 'w')
        try:
            pickle.dump(s,fd)
        except:
            intc.fatal('pickle failed [Test.metarun]')
        fd.close()

        cmd = "mv *.xml SERIALIZED %s %s &> /dev/null" % (self.__logName, self.__dir)
        os.system(cmd)


    def setNbRuns(self, n):
        self._NB_RUNS = n

    def setArgs(self, args):
        self.argv = [''] + [args]

    def getPath(self):
        return str(self.__dir)

    def start(self):
        self.metarun()

if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
