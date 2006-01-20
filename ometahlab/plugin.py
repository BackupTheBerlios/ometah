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
        self.output_available = ['.ps']
        
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
        if self.fileType == ".ps":
            r.postscript(self.fileName, paper=self.ps_paper)
        else:
            return
        
    
    def outputEnd(self):
        if self.fileType == ".ps":
            r.dev_off()
        else:
            return
        
        
    def process(self):
        pass
