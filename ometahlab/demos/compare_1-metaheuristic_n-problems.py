import ometahtest
import ometahstats

runs = 100

print 'Launching test runs:'
print 'Test on Sphere'
u = ometahtest.Test()
u.setArgs('-s 10 -i 100 -p Sphere -d 2 -m CEDA')
u.setNbRuns(runs)
u.setOmetahPath('../../ometah/ometah')
u.start()

print 'Test on Rosenbrock'
v = ometahtest.Test()
v.setArgs('-s 10 -i 100 -p Schwefel -d 2 -m CEDA')
v.setNbRuns(runs)
v.setOmetahPath('../../ometah/ometah')
v.start()

print 'Test on Weierstrass'
w = ometahtest.Test()
w.setArgs('-s 10 -i 100 -p Ackley -d 2 -m SGEN')
w.setNbRuns(runs)
w.setOmetahPath('../../ometah/ometah')
w.start()


print '\nLaunching data process:'
paths = [ u.getPath(), v.getPath() , w.getPath()
          ]

print '  Using ',paths

ometahstats.process(paths,
   [
'end_optimum_bestworst',
'end_optimum_solution_distribution_2D_kernelestimation',
'end_optimum_solution_histogram2D',
'end_optimum_solution_points',
'end_optimum_value_bestworst',
'end_optimum_value_histogram',
##'end_optimum_value_kruskal',
'end_optimum_value_quantilebox',
'end_optimum_value_successrate',
'iteration_all_value_median_logarithmic',
'iteration_all_value_quantilebox',
'iteration_optimum_value_quantilebox',
'end_optimum_value_histogram_multi'
    ],
    'html') # output type
