#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
# Test module for Open Metaheuristic, interface for a test
# Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
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

import os
from rpy import *

class Interface:
    """ the main interface with ometah output """
    
    LOG_ON = 0

    def __init__(self, args):
        """ constructor, from command line arguments """
        self.__argv = [''] + args[1:]
        self.__defaultFileName = "metahtest"
        self.__temp = "."
        self.__path = "."
        self.__logfile = "%s.log" % (self.__defaultFileName)
        self.__points = []
        self.__name = ""
    
    def getXmlFromExecOmetah(self, path):
        """ execute ometah with given arguments,
        returns the file objects corresponding to the cmd output """
        import string # faster here !
        cmd = "%s %s" % (path, string.join(self.__argv))
        try:
            fd = os.popen(cmd)
        except:
            self.log('ERROR : wrong path to ometah [Interface.getXmlFromExecOmetah]\n')            
            fatal('(see log file)')
        self.log('ometah execution ... OK\n')
        return fd

    def getXmlFromFile(self, path):
        """ open an existing file of XML output and return the fd """
        try:
            fd = open(path, 'r')
        except:
            self.log('ERROR : wrong path to XML file [Interface.getXmlFromFile]\n')
        return fd
            
    def log(self, astring):
        """ write log of current job in a *.log file with date,
        pb name, output files... """    
        if self.LOG_ON:
            logf = os.path.join(self.__path, self.__logfile)
            fd = open(logf, 'a')
            fd.write(astring)
            fd.close()
    
    def setLog(self, boolean):
        """ turn the log on or off """
        self.LOG_ON = boolean

    def setLogFileName(self, astring):
        self.__logfile = astring
    
    def datedFileName(self, name, extension):
        """ return a string of toda
        y's date + given string """
        import datetime
        s = datetime.date.today().isoformat()
        return "%s%s%s" % (name, s, extension)

    def setPath(self, path):
        """ set the path where will be saved the output files (ps &| png) """
        try:
            os.listdir(path)
        except: # if can't ls the dir, create it :
            try:
                os.mkdir(path)
            except:
                self.log('ERROR : wrong path for output files, error creating directory [Interface.setPath]\n')
        self.__path = path

    def getPath(self):
        return self.__path

    def copyToDisk(self, rfd, filename="xml"):
        """ copy the file open with fd to a new file on disk
        used to cp XML output on disk """
        filename = filename + '.xml'
        xfile = os.path.join(self.__path, filename)
        try:
            wfd = open(xfile, 'w')
        except:
            self.log('ERROR : cannot create file, maybe directory does not exists... [Interface.copyToDisk]\n')
        try:
            wfd.write(rfd.read())
        except:
            self.log('ERROR : cannot write file [Interface.copyToDisk]\n')
        wfd.close()
        return xfile

    def archiveXml(self):
        """ Create a compressed tar archive of XML files. """
        try:
            import tarfile
            path = os.path.join(self.__path, 'xml.tar.gz')
            tf = tarfile.open(name=path, mode='w:gz')
            for s in os.listdir(self.__path):
                if s[-4:] == '.xml':
                    f = os.path.join(self.__path, s)
                    tf.add(f)
                    os.remove(f)
            tf.close()
        except:
            pass

    def rmDiskCopy(self, filename="xml"):
        """ remove XML copy on disk """
        filename = filename 
        path = os.path.join(self.__path, filename)
        try: # if ls fail, the wrong file name, exit
            os.listdir(path)
        except:
            self.log('ERROR : cannot remove disk copy of XML [Interface.rmDiskCopy]\n')
            return
        os.remove(path)

    def setTemp(self, path):
        """ set the path to the temporary directory """
        self.__temp = path

    def moveLog(self):
        """ move the log file to the new path value """
        src = os.path.join(self.__temp, self.__logfile)
        tar = os.path.join(self.__path, src)
        try:
            os.rename(src, tar)
        except:
            self.log('ERROR : cannot move log file [Interface.moveLog]\n')

    def setPoints(self, points):
        """ set the list of Point object to be plotted """
        self.__points = points
        c = 1
        for p in self.__points:            
            p.index = c
            c += 1

    def getPoints(self):
        return self.__points

    def getOptimum(self):
        """ returns the Point object which has the smallest value,
        from __points list """
        optim = self.__points[0]
        for point in self.__points:
            if point.value < optim.value:
                optim = point
        slog = 'optimum value : %f\n' % (optim.value)
        self.log(slog)
        return optim

    def getTitle(self):
        return self.__title;
    def setTitle(self, title):
        self.__title = title

    def fatal(self, mess):
        import sys
        print "FATAL ERROR : " + mess + "\n"
        sys.exit(1)


if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
