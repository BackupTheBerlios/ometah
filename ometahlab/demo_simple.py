
import ometahtest
import ometahstats


print 'Launching test runs:'
print '  First test on JGEN:'
u = ometahtest.Test()
u.setArgs('-e 10 -p Sphere -d 1 -m JGEN')
u.setNbRuns(5)
u.setOmetahPath('../ometah/ometah')
u.start()

print '  Second test on SGEN:'
v = ometahtest.Test()
v.setArgs('-e 10 -p Sphere -d 1 -m SGEN')
v.setNbRuns(5)
v.setOmetahPath('../ometah/ometah')
v.start()

print '\nLaunching data process:'
paths = [ u.getPath(), v.getPath() ]
ometahstats.process(paths,
    [
    'end_optimum_distribution',
    'end_optimum_quantilebox',
    'end_optimum_bestworst',
    'iteration_all_quantilebox',
    'iteration_optimum_quantilebox',
    'end_optimum_successrate',
    'end_optimum_position',
    'end_optimum_kruskal',
    'iteration_median_logarithmic'
    ])
