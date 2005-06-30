#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
# Test module for Open Metaheuristic, main file
# Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
# Started 06/2005
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
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

import string
S = string

class Qparser:

    ## !!!!!!!!!!!!!!!!!!!!!!!!
    ##
    ## WARNING
    ## get Header first !!! 'cos offset of file shifted !!!
    ##
    ## !!!!!!!!!!!!!!!!!!!!!!!!
    
    def __init__(self):
        """ Constructor with the path to ometah"""
        # list of Point instances
        self.__points = []
        # header object
        self.__header = Header()
        # file descriptor of ometah output
        self.__fd = None        
        # value of a point under which we assum the test as a success, hence points reading is stopped
        self.__tresh = 1
        
    def load(self, path):
        """ Execute ometah, returning the file object of its output """
        import os
        self.__fd = os.popen(path)
                
    def setFd(self, fd):
        self.__fd = fd

    def getPoints(self, treshold):
        """ treshold is the value to go under (or equal) to stop the points reading,
        it's equal to the real optimum value plus the accuracy needed by the pb. """        
        fd = self.__fd
        self.__tresh = treshold
        # reach <optimization> element
        while S.find(fd.readline(), '<optimization>') == -1:
            pass
        pindex = 0    # counter for Point's indexes
        line = fd.readline()
        while 1 == 1:        
            while S.find(line, '<step class="diversification">') == -1:                
                line = fd.readline()
                if line == '': # if EOF reached
                    return self.__points
            # 'while' loop left => <step> found
            fd.readline()      # skip <sample> line
            line = fd.readline()
            while S.find(line, '<point>') != -1:
                stVal = S.find(line,'<values>')
                enVal = S.find(line,'</values>')
                value = float(line[stVal+8:enVal])
                stSol = S.find(line,'<solution>')
                enSol = S.find(line,'</solution>')
                solution = [float(x) for x in S.split(line[stSol+10:enSol])]
                p = Point()
                p.value = value
                p.coords = solution
                p.index = pindex
                self.__points.append(p)

                # test if treshold is reached
                if p.value <= self.__tresh:
                    print 'end at index', pindex
                    return self.__points
                
                pindex += 1
                line = fd.readline()

        return self.__points
    
    def getHeader(self):
        """  """
        fd = self.__fd
        
        # PROBLEM
        #
        PB = self.__header.problem
        # reach <problem>
        line = fd.readline()
        while S.find(line, '<problem>') == -1:
            line = fd.readline()

        # find start and end indexes to get value        
        line = fd.readline()
        st = S.find(line, '<key>')
        en = S.find(line, '</key>')
        PB.key = line[st+5:en]

        line = fd.readline()
        st = S.find(line, '<name>')
        en = S.find(line, '</name>')
        PB.name = line[st+6:en]

        line = fd.readline()
        st = S.find(line, '<description>')
        en = S.find(line, '</description>')
        PB.description = line[st+13:en]

        line = fd.readline()
        st = S.find(line, '<formula>')
        en = S.find(line, '</formula>')
        PB.formula = line[st+9:en]

        line = fd.readline()
        st = S.find(line, '<dimension>')
        en = S.find(line, '</dimension>')
        PB.dimension = int(line[st+11:en])

        line = fd.readline()
        st = S.find(line, '<accuracy>')
        en = S.find(line, '</accuracy>')
        PB.accuracy = float(line[st+10:en])

        optima = []
        line = fd.readline() # skip <optimum>
        line = fd.readline()
        while S.find(line, '<point>') != -1:
            p = Point()            
            st = S.find(line,'<values>')
            en = S.find(line,'</values>')
            p.value = float(line[st+8:en])
            st = S.find(line,'<solution>')
            en = S.find(line,'</solution>')
            p.coords = [float(x) for x in S.split(line[st+10:en])]
            PB.optimum.append(p)
            line = fd.readline()
         # </optimum> read in last find()
            
        fd.readline() # skip <bounds>
        # minimum line
        minb = []
        p = Point()
        line = fd.readline()
        st = S.find(line,'<solution>')
        en = S.find(line,'</solution>')
        p.coords = [float(x) for x in S.split(line[st+10:en])]
        PB.min_bound.append(p)
        # maximum line
        maxb = []
        p = Point()
        line = fd.readline()
        st = S.find(line,'<solution>')
        en = S.find(line,'</solution>')
        p.coords = [float(x) for x in S.split(line[st+10:en])]
        PB.max_bound.append(p)

        fd.readline() # skip </bound>
        line = fd.readline()
        st = S.find(line, '<reference>')
        en = S.find(line, '</reference>')
        PB.reference = line[st+11:en]

        # METAHEURISTIC
        #
        M = self.__header.metah
        # reach <metaheuristic>
        line = fd.readline()
        while S.find(line, '<metaheuristic>') == -1:
            line = fd.readline()

        line = fd.readline()
        st = S.find(line, '<key>')
        en = S.find(line, '</key>')
        M.key = line[st+5:en]

        line = fd.readline()
        st = S.find(line, '<family>')
        en = S.find(line, '</family>')
        M.family = line[st+8:en]

        line = fd.readline()
        st = S.find(line, '<name>')
        en = S.find(line, '</name>')
        M.name = line[st+6:en]

        line = fd.readline()
        st = S.find(line, '<accronym>')
        en = S.find(line, '</accronym>')
        M.acronym = line[st+10:en]

        line = fd.readline()
        st = S.find(line, '<description>')
        en = S.find(line, '</description>')
        M.description = line[st+13:en]

        line = fd.readline()
        st = S.find(line, '<reference>')
        en = S.find(line, '</reference>')
        M.reference = line[st+11:en]

        # PARAMETERS
        #
        PA = self.__header.parameters
        fd.readline() # skip <parameters>
        line = fd.readline()
        st = S.find(line, '<value>')
        en = S.find(line, '</value>')
        PA.sampleSize = int(line[st+7:en])

        line = fd.readline()
        st = S.find(line, '<value>')
        en = S.find(line, '</value>')
        PA.maxIterations = line[st+7:en]

        line = fd.readline()
        st = S.find(line, '<value>')
        en = S.find(line, '</value>')
        PA.maxEvaluations = line[st+7:en]

        line = fd.readline()
        st = S.find(line, '<value>')
        en = S.find(line, '</value>')
        PA.treshold = line[st+7:en]

        line = fd.readline()
        st = S.find(line, '<value>')
        en = S.find(line, '</value>')
        PA.randomSeed = line[st+7:en]

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
        """ Void constructor."""
        pass
    

class Point:
    """ A point has a set of coordinates, a value, an error relative to the problem's optimum, and an index. """

    coords = None
    value = None
    error = None
    index = 0

    def __init__(self):
        """ Point constructor. """
        pass


if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
