#!/usr/bin/python
# -*- coding: iso-8859-1 -*-

###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
#  File: qparser.py
#  This file contains an experimental class to read ometah's XML output,
#  much faster than formal XML parsing with xml module.
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

class Qparser:
    """ Read the XML output of ometah and get information in appropriate structues.
    Warning : getHeader must be used BEFORE getPoints, because of sequential reading. """
    
    def __init__(self):
        """ Constructor with the path to ometah"""
        self.__points = []         # list of Point instances
        self.__header = Header()   # Header object, returned but getHeader
        self.__fd = None           # file descriptor of ometah output
        self.__tresh = 0           # value under which test is a success, hence points reading is stopped
        self.__evaluations = 0     # effective nb of evaluations done before reaching success.
        
    def load(self, path):
        """ Execute ometah, returning the file object of its output """
        import os
        self.__fd = os.popen(path)
                
    def setFd(self, fd):
        self.__fd = fd

    def getPoints(self, treshold):
        """ treshold is the value to go under (or equal) to stop the points reading,
        it's equal to the real optimum value plus the accuracy needed by the pb. """        
        self.__tresh = treshold
        (fd, line) = (self.__fd, '')
        
        def f(s):
            return line.find(s) 
        
        while fd.readline().find('<optimization>') < 0:
            pass
        
        # counter for Point's indexes
        pindex = 0    
        line = fd.readline()
        while True:        
            while f('<step class="diversification">') < 0:                
                line = fd.readline()
                if line == '': # if EOF reached
                    # no success reached, max evaluations done
                    self.__evaluations = eval
                    return self.__points
                
            # 'while' loop left => <step> found
            fd.readline() # skip <sample>
            line = fd.readline()
            
            while f('<point>') != -1:
                p = Point()
                p.value = float(line[f('<values>')+8:f('</values>')])
                p.coords = [float(x) for x in line[f('<solution>')+10:f('</solution>')].split()]
                p.index = pindex
                self.__points.append(p)          
                pindex += 1
                line = fd.readline()

            # get current #evaluations
            while f('<evaluations>') < 0:
                line = fd.readline()
            eval = int( line[f('<evaluations>')+13:f('</evaluations>')] )

        # should not get there
        return self.__points

    def getEvaluations(self):
        """ Returns """
        return self.__evaluations
    
    def getHeader(self):
        """ Return Header instance with info from XML file. """
        (fd, line) = (self.__fd, '')
        
        def f(s):
            return line.find(s) 
        
        # PROBLEM
        PB = self.__header.problem
        line = fd.readline()
        while f('<problem>') < 0: # reach <problem>
            line = fd.readline()
            
        line = fd.readline()
        PB.key = line[f('<key>')+5:f('</key>')]
        line = fd.readline()
        PB.name = line[f('<name>')+6:f('</name>')]
        line = fd.readline()
        PB.description = line[f('<description>')+13:f('</description>')]
        line = fd.readline()
        PB.formula = line[f('<formula>')+9:f('</formula>')]
        line = fd.readline()
        PB.dimension = int(line[f('<dimension>')+11:f('</dimension>')])
        line = fd.readline()
        PB.accuracy = float(line[f('<accuracy>')+10:f('</accuracy>')])

        optima = []
        fd.readline() # skip <optimum>
        line = fd.readline()
        while f('<point>') != -1:
            p = Point()
            p.value = float(line[f('<values>')+8:f('</values>')])
            p.coords = [float(x) for x in line[f('<solution>')+10:f('</solution>')].split()]                                 
            PB.optimum.append(p)
            line = fd.readline()
            
        fd.readline() # skip <bounds>
        minb = []
        line = fd.readline()
        p = Point()
        p.coords = [float(x) for x in line[f('<solution>')+10:f('</solution>')].split()]
        PB.min_bound.append(p)
        maxb = []
        line = fd.readline()
        p = Point()
        p.coords = [float(x) for x in line[f('<solution>')+10:f('</solution>')].split()]
        PB.max_bound.append(p)

        fd.readline() # skip </bound>
        line = fd.readline()
        PB.reference = line[f('<reference>')+11:f('</reference>')]

        # METAHEURISTIC
        M = self.__header.metah
        # reach <metaheuristic>
        line = fd.readline()
        while f('<metaheuristic>') < 0:
            line = fd.readline()

        line = fd.readline()
        M.key = line[f('<key>')+5:f('</key>')]
        line = fd.readline()
        M.family = line[f('<family>')+8:f('</family>')]
        line = fd.readline()
        M.name = line[f('<name>')+6:f('</name>')]
        line = fd.readline()
        M.acronym = line[f('<accronym>')+10:f('</accronym>')]
        line = fd.readline()
        M.description = line[f('<description>')+13:f('</description>')]
        line = fd.readline()
        M.reference = line[f('<reference>')+11:f('</reference>')]

        # PARAMETERS
        PA = self.__header.parameters
        fd.readline() # skip <parameters>
        line = fd.readline()
        PA.sampleSize = int( line[f('<value>')+7:f('</value>')] )
        line = fd.readline()
        PA.maxIterations = int( line[f('<value>')+7:f('</value>')] )
        line = fd.readline()
        PA.maxEvaluations = int( line[f('<value>')+7:f('</value>')] )
        line = fd.readline()
        PA.treshold = float( line[f('<value>')+7:f('</value>')] )
        line = fd.readline()
        PA.randomSeed = int( line[f('<value>')+7:f('</value>')] )

        return self.__header
        

class Problem:
    """ Descriptive informations of a problem. """

    key = None
    name = None
    description = None
    formula = None
    dimension = None
    optimum = []   # list of Point
    min_bound = [] # idem
    max_bound = [] # idem
    reference = None
    accuracy = 0.0
    
    def __init__(self):
        """ Void constructor."""
        pass


class Metaheuristic:
    """ Descriptive informations of a metaheuristic. """

    key = None
    name = None
    family = None
    acronym = None
    description = None
    reference = None
    
    def __init__(self):
        """ Void constructor."""
        pass


class Parameters:
    """ The set of parameters specified into <parameters> elemens in XML output """

    sampleSize = None
    maxIterations = None
    maxEvaluations = None
    treshold = None
    randomSeed = None

    def __init__(self):
        pass


class Header:
    """ Additional informations in XML file concerning problem, metaheuristic, and parameters."""

    problem = Problem()
    metah = Metaheuristic()
    parameters = Parameters()

    def __init__(self):
        pass
    

class Point:
    """ A point has a set of coordinates, a value, an error relative to the problem's optimum, and an index. """

    coords = None
    value = None    
    error = None
    index = None

    def __init__(self):
        pass


if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
