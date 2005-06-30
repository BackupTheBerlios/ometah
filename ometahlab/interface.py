#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  Author: Jean-Philippe Aumasson <jeanphilippe.aumasson@gmail.com>
#  File: interface.py
#  An Interface is used by a Test of ometah (cf. ometahtest), and should not
#  be used directly.
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

import os
from rpy import *


class Interface:
    """ Interface between ometah output and ometahlab data treatement.
    One interface instance is used for each ometah run, """
    
    def __init__(self):
        """ Constructor """
        pass

    def log(self, astring):
        """ Log the given string in the log file, appending to the end of the file. """    
        fd = open(self.__logfile, 'a')
        fd.write(astring)
        fd.close()
    
    def setLog(self, astring):
        """ Set the name of the log file as the given string, no extension is added, you should include it in astring."""
        self.__logfile = astring
    
    def copyToDisk(self, rfd, filename="xml"):
        """ Copy the file opened with rfd to a new file on disk, is used to copy XML output on disk. """
        filename += '.xml'
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

    def fatal(self, msg):
        import sys
        print 'FATAL ERROR: %s' % msg
        sys.exit(-1)

if __name__ == '__main__':
    print "This file contains no instructions in main(), please see README for program usage\n"
