##
## AccumulatorAction.py
##

import myShutil # for better copytree()
import os
import shutil

def accumulatorFunction(target, source, env):
  """Function called when builder is called"""
  destDir = str(target[0])
  if not os.path.exists(destDir):
      os.makedirs(destDir)
  for s in source:
      s = str(s)
      if os.path.isdir(s):
          myShutil.copytree(s, destDir, symlinks = False)
      else:
          shutil.copy2(s, destDir)
