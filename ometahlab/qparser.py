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

    def __init__(self, path):
        """ Constructor with the path to ometah"""
        # path to ometah binary
        self.__opath = path
        # list of Point instances
        self.__points = None
        # file descriptor for ometah output
        self.__fd = None        

    def __load(self):
        """ Execute ometah, returning the file object of its output """
        import os
        fd = os.popen(self.__opath)
        
        return fd

    def qparse(self):
        """ """        
        self.__fd = self.__load()
        fd = self.__fd

        # reach <optimization>
        while S.find(fd.readline(), '<optimization>') == -1:
            pass

        # counter for Point's indexes
        pindex = 0

        # initialize line to first line read
        line = fd.readline()

        while 1 == 1:
            # reach next step
            while S.find(line, '<step class="diversification">') == -1:                
                line = fd.readline()
                if line == '':
                    print '!!! EOF !!!'
                    return 1
            # out of the while => <step> found
            # skip <sample> line
            fd.readline()
            # for all point definitions
            line = fd.readline()
            while S.find(line, '<point>') != -1:
                self.__readPoint(line, pindex)
                pindex += 1
                line = fd.readline()


    def __readPoint(self, line, pindex):
        """ Create an instance of Point and append it to the list,
        line argument is a string of a line read """
        import sys
        print 'readPoint : ', line
        stVal = string.find(line,'<values>')
        enVal = string.find(line,'</values>')
        value = float(line[stVal+8:enVal])
        print 'value : ', value

        stSol = string.find(line,'<solution>')
        enSol = string.find(line,'</solution>')
        solution = [float(x) for x in string.split(line[stSol+10:enSol])]
        print 'solutions : ', solution

        print 'index : ', pindex
        sys.exit(-1)
    
if __name__ == '__main__':

    q = Qparser('../ometah/ometah')
    q.qparse()
