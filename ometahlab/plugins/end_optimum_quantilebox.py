# This is a string marking the file as a ometahlab plugin
# isOmetahLabPlugin

# necessary base class
from plugin import Plugin

# import rpy if you use R
from rpy import *

class end_optimum_quantilebox(Plugin):

    def __init__(self,data):
        # call the plugin basic initializations
        Plugin.__init__(self,data)

        # set the name of your plugin
        self.setName("end_optimum_quantilebox","Show one quantile box of optima for each Test. ")
        

    # necessary method, called when lauching the plugin
    def process(self):
        # uncomment this line if you use a R output
        self.outputInit()
        
        # put your plugin code here
        vlist = [[p.value for p in points ] for points in self.data.optimas ]

        zero = False
        for v in vlist:
            if 0 in v:
                zero = True
        if zero:
            r.boxplot(vlist, style='quantile', col=self.data.color, main='Optimas list', xlab='Test index', ylab='')
        else:
            try:
                r.boxplot(vlist, style='quantile', log="y", col=self.data.color, main='Optimas list', xlab='Test index', ylab='')
            except:
                print 'Cannot use logarithmic scale 0'
                self.__LOG = 0
                r.boxplot(vlist, style='quantile', col=self.data.color, main='Optimas list', xlab='Test index', ylab='')
        r.grid(nx=10, ny=40)
        
        # uncomment this line if you use a R output
        self.outputEnd()
