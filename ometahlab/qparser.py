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

import parser
P = parser
import string
S = string

class Qparser:

    ## !!!!!!!!!!!!!!!!!!!!!!!!
    ##
    ## WARNING
    ## get Header first !!! 'cos offset of file shifted !!!
    ##
    ##
    ## Cast in FLOAT / INT when needed !
    ##
    ## !!!!!!!!!!!!!!!!!!!!!!!!
    
    def __init__(self):
        """ Constructor with the path to ometah"""
        # list of Point instances
        self.__points = []
        # header object
        self.__header = P.Header()
        # file descriptor of ometah output
        self.__fd = None        

    def load(self, path):
        """ Execute ometah, returning the file object of its output """
        import os
        self.__fd = os.popen(path)
        
        
    def setFd(self, fd):
        self.__fd = fd

    def getPoints(self):
        """ """        
        fd = self.__fd
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
                self.__readPoint(line, pindex)
                pindex += 1
                line = fd.readline()

        return self.__points
    
    def getHeader(self):
        """ """
        fd = self.__fd
        
        # PROBLEM
        #
        line = fd.readline()
        while S.find(line, '<problem>') == -1:
            line = fd.readline()

        # find start and end indexes to get value        
        line = fd.readline()
        st = S.find(line, '<key>')
        en = S.find(line, '</key>')
        key = line[st+5:en]

        line = fd.readline()
        st = S.find(line, '<name>')
        en = S.find(line, '</name>')
        name = line[st+6:en]

        line = fd.readline()
        st = S.find(line, '<description>')
        en = S.find(line, '</description>')
        desc = line[st+13:en]

        line = fd.readline()
        st = S.find(line, '<formula>')
        en = S.find(line, '</formula>')
        forml = line[st+9:en]

        line = fd.readline()
        st = S.find(line, '<dimension>')
        en = S.find(line, '</dimension>')
        dim = int(line[st+11:en])

        line = fd.readline()
        st = S.find(line, '<accuracy>')
        en = S.find(line, '</accuracy>')
        acc = float(line[st+10:en])

        optima = []
        line = fd.readline() # skip <optimum>
        line = fd.readline()
        while S.find(line, '<point>') != -1:
            p = P.Point()            
            st = S.find(line,'<values>')
            en = S.find(line,'</values>')
            p.value = float(line[st+8:en])
            st = S.find(line,'<solution>')
            en = S.find(line,'</solution>')
            p.coords = [float(x) for x in S.split(line[st+10:en])]
            optima.append(p)
            line = fd.readline()
         # </optimum> read in last find()
            
        fd.readline() # skip <bounds>
        # minimum line
        minb = []
        p = P.Point()
        line = fd.readline()
        st = S.find(line,'<solution>')
        en = S.find(line,'</solution>')
        p.coords = [float(x) for x in S.split(line[st+10:en])]
        minb.append(p)
        # maximum line
        maxb = []
        p = P.Point()
        line = fd.readline()
        st = S.find(line,'<solution>')
        en = S.find(line,'</solution>')
        p.coords = [float(x) for x in S.split(line[st+10:en])]
        maxb.append(p)

        fd.readline() # skip </bound>
        line = fd.readline()
        st = S.find(line, '<reference>')
        en = S.find(line, '</reference>')
        ref = line[st+11:en]

        PB = self.__header.problem
        PB.key = key
        PB.name = name
        PB.description = desc
        PB.formula = forml
        PB.dimension = dim
        PB.optimum = optima
        PB.min_bound = minb
        PB.max_bound = maxb
        PB.reference = ref
        PB.accuracy = acc

        # METAHEURISTIC

        # reach <metaheuristic>
        line = fd.readline()
        while S.find(line, '<metaheuristic>') == -1:
            line = fd.readline()

        line = fd.readline()
        st = S.find(line, '<key>')
        en = S.find(line, '</key>')
        key = line[st+5:en]

        line = fd.readline()
        st = S.find(line, '<family>')
        en = S.find(line, '</family>')
        fam = line[st+8:en]

        line = fd.readline()
        st = S.find(line, '<name>')
        en = S.find(line, '</name>')
        name = line[st+6:en]

        line = fd.readline()
        st = S.find(line, '<accronym>')
        en = S.find(line, '</accronym>')
        accr = line[st+10:en]

        line = fd.readline()
        st = S.find(line, '<description>')
        en = S.find(line, '</description>')
        desc = line[st+13:en]

        line = fd.readline()
        st = S.find(line, '<reference>')
        en = S.find(line, '</reference>')
        ref = line[st+11:en]

        M = self.__header.metah
        M.key = key
        M.name = name
        M.family = fam
        M.acronym = accr
        M.description = desc
        M.reference = ref

        # PARAMETERS
        
        fd.readline() # skip <parameters>
        line = fd.readline()
        st = S.find(line, '<value>')
        en = S.find(line, '</value>')
        sampleSize = line[st+7:en]

        line = fd.readline()
        st = S.find(line, '<value>')
        en = S.find(line, '</value>')
        maxIter = line[st+7:en]

        line = fd.readline()
        st = S.find(line, '<value>')
        en = S.find(line, '</value>')
        maxEval = line[st+7:en]

        line = fd.readline()
        st = S.find(line, '<value>')
        en = S.find(line, '</value>')
        tresh = line[st+7:en]

        line = fd.readline()
        st = S.find(line, '<value>')
        en = S.find(line, '</value>')
        seed = line[st+7:en]

        PA = self.__header.parameters
        PA.sampleSize = sampleSize
        PA.maxIterations = maxIter
        PA.maxEvaluations = maxEval
        PA.treshold = tresh
        PA.randomSeed = seed

        return self.__header
        

    def __readPoint(self, line, pindex):
        """ Create an instance of Point and append it to the list,
        line argument is a string of a line read """
        import sys
        stVal = S.find(line,'<values>')
        enVal = S.find(line,'</values>')
        value = float(line[stVal+8:enVal])

        stSol = S.find(line,'<solution>')
        enSol = S.find(line,'</solution>')
        solution = [float(x) for x in S.split(line[stSol+10:enSol])]

        p = P.Point()
        p.value = value
        p.coords = solution
        p.index = pindex
        
        self.__points.append(p)        
    
if __name__ == '__main__':

    q = Qparser()
    q.load('../ometah/ometah')
#    q.getHeader()
    pp = q.getPoints()
 
