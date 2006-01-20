# This is a string marking the file as a ometahlab plugin
# isOmetahLabPlugin

# necessary base class
from plugin import Plugin

# import rpy if you use R
from rpy import *

# name your plugin according to the following notation :
# (end|iteration)_(optimum|all)_(type)
# for example : end_optimum_distribution
class end_optimum_kruskal(Plugin):

    def __init__(self,data):
        # call the plugin basic initializations
        Plugin.__init__(self,data)

        # set the name of your plugin
        self.setName("end_optimum_kruskal",
        "Create graph of distribution errors, and make non-parametric over ALL tests.")
        

    # necessary method, called when lauching the plugin
    def process(self):
        # uncomment this line if you use a R output
        self.outputInit()
        
        # put your plugin code here
        # the data are in self.data :
        #   self.data.color
        #   self.data.optimas
        #   self.data.pointsIter
        #   self.data.optimaIter
        #   ...
        import Numeric
        limit = .05
        # initialize our matrix with as sublists as tests
        emlist = [ [] for i in self.data.tests ]
        breaks = 10
        
        for i in xrange(len(self.data.optimas)):
            # for current test's optima list, add their error as a list at emlist[i]
            emlist[i] = [p.value - self.data.tests[i].opt_val for p in self.data.optimas[i]]
            txt = '%s\nOptima error distribution' % self.data.tests[i].args
            r.hist(emlist[i], breaks, col=self.data.color, main=txt, xlab='Error', ylab='Frequency')
            r.grid(nx=10)

        # uncomment this line if you use a R output
        self.outputEnd()
        
        if len(self.data.tests) == 2:
            # use Mann-Whitney test
            dic = r.wilcox_test(Numeric.array(emlist))
        elif len(self.data.tests) > 2:
            # use Kruskal-Wallis test
            dic = r.kruskal_test(emlist)
        else: # only one test
            return
        
        # TODO : find a way to automate the LaTeX output in a plugin
        #self.__same_distrib = dic['p.value']
        print "Non-parametric test on optima error over all tests: ",dic['p.value']
