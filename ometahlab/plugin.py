#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#
###
#
#  Ometahlab is a set of Python scripts to make experiments on Ometah.
#
#  $id$
#  Copyright : Free Software Fundation
#  Author : Johann Dréo <nojhan@gmail.com>
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


import glob
import os

from rpy import *

class Plugin:
    """Base class for plugins"""
    def __init__(self,data):
        
        # link to the data object
        self.data = data
        
        # options for the output
        self.ps_paper = 'letter'
        
        # list of supported outputs
        self.output_available = ['.ps','.png']
        
        # type of the default output file
        self.fileType = ".ps"
        
        # name and description of the plugin
        self.setName("ometalab_unnamed_plugin","none")
        
    def setName(self,name,description="none"):
        self.name = name
        self.description = description
        self.setFileName()
        
        
    def setFileName(self):
        self.fileName = os.path.join(self.data.dir, self.name+self.fileType)
        
        
    def setFileType(self,type):
        if type in self.output_available:
            self.fileType = type
            self.setFileName()
        else:
            raise 'OptionError : unsupported output type : '+type+', not in '+self.output_available
        
        
    def outputInit(self):
        testName = self.fileName
        i=1
        while os.path.isfile(testName):
            testName = "%s_%i%s" % ( os.path.join(self.data.dir, self.name), i, self.fileType )
            
        self.fileName = testName
            
        
        if self.fileType == ".ps":
            r.postscript(self.fileName, paper=self.ps_paper)
        elif self.fileType == ".png":
            r.png(self.fileName)
        else:
            return
        
    
    def outputEnd(self):
        if self.fileType in self.output_available:
            r.dev_off()
        else:
            return
        
        
    def process(self):
        pass
