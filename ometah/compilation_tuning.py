# optimizations test

import time, os

compile_command = 'scons static=yes debug=no flags='
run_command = './ometah -m HCIAC -p Rosenbrock -d 2 -i 10 -e 1000 -s 100'
run_number=20

# -ftree-vectorize -fprefetch-loop-arrays -O1 -O2 -O3
#test_flags = ['-O0','-O1','-O2','-O3','-02  -mfpmath=sse']
test_flags = ['-O1','-O2','-O3','-O2 -mfpmath=sse','-O3 -mfpmath=sse']


def compilation(flags):
  #print compile_command + "'" + flags + "'"
  os.popen( compile_command + "'" + flags + "'" ).readlines()
   

def run():
  #print run_number,"x",run_command
  start = time.time()
  for i in range(run_number):
    os.popen( run_command )
  total = time.time() - start
  #print total
  return total


if __name__=="__main__":
  res = {}
  for i in test_flags:
    print i
    compilation(i)
    res[i] = run()
  
  print run_number,"x",run_command
  for i in res:
    print i,res[i]
