# isOmetahLabPlugin

import os
from rpy import *
from plugin import Plugin

class end_optimum_distribution(Plugin):

    def __init__(self,data):
        Plugin.__init__(self,data)

        self.setName("end_optimum_distribution",
        "Plot frequency distribution of optima for each test, that is each Test \
        instance, each one having its own sublist of optimas in self.__optimas. \
        We have one optimum for each run of the test.")

    def process(self):
        self.outputInit()
    
        breaks = 10 # nb breaks in histo, may be reduced if not enough points
        
        i = 0
        for points in self.data.optimas:
            vlist = [p.value for p in points]
            txt = '%s\nOptima distribution' % self.data.tests[i].args
            r.hist(vlist, breaks, col=self.data.color, main=txt, xlab='Value', ylab='Frequency') 
            r.grid(nx=10)
            i += 1
        
        self.outputEnd()
