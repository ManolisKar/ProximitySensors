import sys
import glob
import os
import json
import struct
import time
import readline
import numpy as np
import matplotlib
import matplotlib.pyplot as plt


'''
Analyze data collected with the VCNL4010.

Usage:
python analyze_data.py <datadir> <outfile>
- <datadir> is the directory which contains the data to be analyzed.
- <outfile>: If given, output all results in this datafile.

Author: Manolis Kargiantoulakis
'''

#mode = "sample" ## In this mode plot and output data at the sample level
mode = "run" ## In this mode plot and output data at the run level


def main():


  global mode
  global gainsettingscal
  global threshsettingscal
  global gainsettingshv
  global threshsettingshv
  global fout
  global dcvoltages
  global rundir

  prox0 = []
  prox1 = []
  lux0 = []
  lux1 = []
  timestamp = []
  runnum = []
  
  PREFIX = "VCNL4010"

  if len(sys.argv) > 1:
    datadir = sys.argv[1]
  else: 
    topdir = '/Users/ekargian/Documents/g-2/Magnets/ElectrostaticQuads'
    datadir = os.path.join(topdir,'Alignment/VCNL4010_measurement/data/test')

  if not os.path.exists(datadir):
    print 'Data dir doesn\'t exist!\n'
    exit()

  files = glob.glob(datadir + "/" + PREFIX + "_*.dat")
  nfiles=0
  for file_name in files:
      num = int(file_name.split("_")[-1].split(".dat")[0])
      print 'Processing data file #%d' % num
      nfiles += 1
      
      prox0_file = []
      prox1_file = []
      lux0_file = []
      lux1_file = []

      current_file = open(file_name,'r')
      nline = 0

      if mode=="run":
        
        for line in current_file:
          if line[0] == '#':
            continue
          if nline == 0:
            timestamp.append(float(line))
            nline += 1
            continue

          prox0_file.append(int(line.split()[0]))
          lux0_file.append(int(line.split()[1]))
          prox1_file.append(int(line.split()[2]))
          lux1_file.append(int(line.split()[3]))

        prox0.append(np.mean(prox0_file))
        lux0.append(np.mean(lux0_file))
        prox1.append(np.mean(prox1_file))
        lux1.append(np.mean(lux1_file))
        runnum.append(num)

      elif mode=="sample":

        for line in current_file:
          if line[0] == '#':
            continue
          if nline == 0:
            stamp = float(line)
            nline += 1
            continue

          prox0.append(int(line.split()[0]))
          lux0.append(int(line.split()[1]))
          prox1.append(int(line.split()[2]))
          lux1.append(int(line.split()[3]))
          timestamp.append(stamp)
          runnum.append(num)

        

      if num == 0:
          time0 = timestamp[-1]
          

  print 'Processed %d files' % nfiles

  ## Conversion to mm
  ## This is based on the calibration files ~/Documents/g-2/Magnets/ElectrostaticQuads/Alignment/VCNL4010_measurement/calibration/calibration_20190201.*
  distance0=[]
  distance1=[]
  for i in range(len(prox0)):
    # For distance to inner:
    x = prox0[i]
    dist0 = (378.207+-0.317061*x+0.000117992*x**2+-2.23865e-08*x**3+2.35328e-12*x**4+-1.26998e-16*x**5+2.90089e-21*x**6)*np.exp(-0.000432495*x)
    if dist0<0:
      dist0=0
    distance0.append(dist0)
    # For distance to outer
    x = prox1[i]
    dist1 = (309.989+-0.210837*x+6.05399e-05*x**2+-7.32434e-09*x**3+3.68052e-13*x**4)*np.exp(-0.000479273*x)
    if dist1<0:
      dist1=0
    distance1.append(dist1)


        
  
  
  prox0 = np.array(prox0)
  lux0 = np.array(lux0)
  prox1 = np.array(prox1)
  lux1 = np.array(lux1)
  distance0 = np.array(distance0)
  distance1 = np.array(distance1)
  timestamp = np.array(timestamp)
  runnum = np.array(runnum)

    
  for i in range(len(timestamp)):
      timestamp[i] -= time0
      #timestamp[i] /= 60. ## time in minutes
      #runnum[i] = runnum[i]*2.08 ## multiplying by this factor gives minutes


  ## If a second argument is given, output data into a file. The filename is the argument.
  if len(sys.argv) > 2:
    outfile_name = sys.argv[2]
    outfile = open(outfile_name,'w')
    outfile.write('Timestamp  Distance0  Lux0  Distance1  Lux1\n')
    for i in range(len(timestamp)):
      outfile.write('%f\t%f\t%f\t%f\t%f\t%f\n' % (runnum[i], timestamp[i], distance0[i],lux0[i],distance1[i],lux1[i]))
    print 'Results stored in file ', outfile_name


      

  ## Plot data

  plt.figure()
  plt.plot(runnum,distance0,'ro',label='Distance to Inner')
  plt.plot(runnum,distance1,'bo',label='Distance to Outer')
  plt.ylabel('Distance (mm)')
  plt.xlabel('Run number (~3s each)')
  plt.legend()

  plt.figure()
  plt.plot(runnum,prox0,'ro',label='Prox0 -- inner')
  plt.plot(runnum,prox1,'bo',label='Prox1 -- outer')
  plt.ylabel('Output')
  plt.xlabel('Run number (~3s each)')
  plt.legend()

  plt.figure()
  plt.plot(runnum,lux0,'ro',label='Lux0 -- inner')
  plt.plot(runnum,lux1,'bo',label='Lux1 -- outer')
  plt.ylabel('Luminosity')
  plt.xlabel('Run number (~3s each)')
  plt.legend()

  plt.show()

      

  
if __name__ == "__main__":
    main()
