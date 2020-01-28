import glob
import os
import json
import struct
import time
import readline
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import sys
from dateutil import parser
import scipy.fftpack
from datetime import timedelta, datetime

####
## Analyze data collected with the VCNL4010.
## Pass the data directory as argument.
##
## Author: Manolis Kargiantoulakis
####



global mode
global gainsettingscal
global threshsettingscal
global gainsettingshv
global threshsettingshv
global fout
global dcvoltages
global rundir


def main():



  prox0 = []
  prox1 = []
  lux0 = []
  lux1 = []
  prox0_250Hz = []
  prox1_250Hz = []
  lux0_250Hz = []
  lux1_250Hz = []
  time_250Hz = []
  timestamp = []
  timestamp_delta = []
  runnum = []
  
  PREFIX = "VCNL4010"

  if len(sys.argv) > 1:
    datadir = sys.argv[1]
  else: 
    topdir = '/home/pi'
    datadir = os.path.join(topdir,'QuadsAlignment/data/test')

  if not os.path.exists(datadir):
    print 'Data dir doesn\'t exist!\n'
    exit()

  files = glob.glob(datadir + "/" + PREFIX + "_*.dat")
  nfiles=0
  timestamp_delta.append(0)

  for file_name in files:
    num = int(file_name.split("_")[-1].split(".dat")[0])
    print ('Processing data file #%d, run=%d' % (nfiles,num))
    nfiles += 1
    runnum.append(num)
    
    prox0_file = []
    prox1_file = []
    lux0_file = []
    lux1_file = []

    current_file = open(file_name,'r')
    nline = 0
    for line in current_file:
      if line[0] == '#':
          continue
      if nline == 0:
          line_parsed = parser.parse(line)
          line_date2num = matplotlib.dates.date2num(line_parsed)
          timestamp.append(line_date2num)
          #timestamp.append(line_parsed)
          #if nfiles>1:
            #delta =  timedelta(timestamp[-1]-timestamp[-2]).total_seconds()
            #timestamp_delta.append( delta )
            #print line, line_parsed, line_date2num, delta
          nline += 1
          continue

      prox0_250Hz.append(int(line.split()[0]))
      lux0_250Hz.append(int(line.split()[1]))
      prox1_250Hz.append(int(line.split()[2]))
      lux1_250Hz.append(int(line.split()[3]))
      #time_250Hz.append(timestamp[-1]+nline*4e-9)
      
      prox0_file.append(int(line.split()[0]))
      lux0_file.append(int(line.split()[1]))
      prox1_file.append(int(line.split()[2]))
      lux1_file.append(int(line.split()[3]))
      nline += 1

    prox0.append(np.mean(prox0_file))
    lux0.append(np.mean(lux0_file))
    prox1.append(np.mean(prox1_file))
    lux1.append(np.mean(lux1_file))

    #if num == 0:
    #  time0 = timestamp[-1]
          
  print 'Processed %d files' % nfiles

  #times = matplotlib.dates.date2num(timestamp)
  

  # Rough conversion to mm:
  distance0=[]
  distance1=[]
  for i in range(len(prox0)):
    x = prox0[i]
    dist0 = (364.315-0.253896*x+7.42632e-05*x**2-9.12668e-09*x**3+4.44983e-13*x**4)*np.exp(-0.000482712*x)
    if dist0<0:
      dist0=0
    distance0.append(dist0)
    x = prox1[i]
    dist1 = (73.1555-0.0267666*x+6.79842e-06*x**2-5.78545e-10*x**3+3.00264e-14*x**4)*np.exp(-0.000357827*x)
    if dist1<0:
      dist1=0
    distance1.append(dist1)

  distance0_250Hz=[]
  distance1_250Hz=[]
  for i in range(len(prox0_250Hz)):
    x = prox0_250Hz[i]
    dist0 = (364.315-0.253896*x+7.42632e-05*x**2-9.12668e-09*x**3+4.44983e-13*x**4)*np.exp(-0.000482712*x)
    if dist0<0:
      dist0=0
    distance0_250Hz.append(dist0)
    x = prox1_250Hz[i]
    dist1 = (73.1555-0.0267666*x+6.79842e-06*x**2-5.78545e-10*x**3+3.00264e-14*x**4)*np.exp(-0.000357827*x)
    if dist1<0:
      dist1=0
    distance1_250Hz.append(dist1)


        

  ## If a second argument is given, output data into a file. The filename is the argument.
  if len(sys.argv) > 2:
    outfile_name = sys.argv[2]
    outfile = open(outfile_name,'w')
    outfile.write('Timestamp  Distance0  Lux0  Distance1  Lux1\n')
    for i in range(len(timestamp)):
      outfile.write('%d\t%d\t%f\t%f\t%d\t%f\t%d\n' % (i,runnum[i],timestamp[i], distance0[i],lux0[i],distance1[i],lux1[i]))
    print 'Results stored in file ', outfile_name


  
  
  prox0_250Hz = np.array(prox0_250Hz)
  lux0_250Hz = np.array(lux0_250Hz)
  prox1_250Hz = np.array(prox1_250Hz)
  lux1_250Hz = np.array(lux1_250Hz)
  time_250Hz = np.array(time_250Hz)
  prox0 = np.array(prox0)
  lux0 = np.array(lux0)
  prox1 = np.array(prox1)
  lux1 = np.array(lux1)
  distance0 = np.array(distance0)
  distance1 = np.array(distance1)
  distance0_250Hz = np.array(distance0_250Hz)
  distance1_250Hz = np.array(distance1_250Hz)
  timestamp = np.array(timestamp)
  runnum = np.array(runnum)

  ### To do FFT: need equidistant data 
  ## Maybe extract FFT from each file, setting (time[-1]-time[0])/N as period between points.
  ## Can the FFTs from each file then be combined?
  #distance0_fft = scipy.fftpack.fft(distance0_250Hz)
  #distance1_fft = scipy.fftpack.fft(distance1_250Hz)


  #time_passed = matplotlib.dates.date2num(timestamp)
  #for i in range(len(time_passed)):
  #    time_passed[i] -= time_passed[0]
  #    time_passed[i] /= 60. ## time in minutes
      #runnum[i] = runnum[i]*2.08 ## multiplying by this factor gives minutes


  ## Order files correctly in time according to runnum
  inds = runnum.argsort()
  runnum_sorted = runnum[inds]
  prox0_sorted = prox0_250Hz[inds]
  lux0_sorted = lux0_250Hz[inds]
  prox1_sorted = prox1_250Hz[inds]
  lux1_sorted = lux1_250Hz[inds]
  distance1_sorted = distance1[inds]
  timestamp_sorted = timestamp[inds]

  time_delta = []
  time_delta.append(0)
  for i in range(len(timestamp_sorted)):
    if i>0:
      time_delta.append( timedelta(timestamp_sorted[i]-timestamp_sorted[i-1]).total_seconds() )
      print i, runnum_sorted[i], timestamp_sorted[i], time_delta[i]
      

  ## Plot data

  plt.figure()
  #plt.plot(times,distance0,'ro',label='Distance to Inner')
  #plt.plot(time_250Hz,distance1_250Hz,'ro',markersize=1,label='Distance, 250Hz')
  plt.plot(time_delta,distance1,'bo',markersize=5,label='Distance, file-averaged')
  plt.ylabel('Distance (mm)')
  plt.xlabel('Time (s?)')
  plt.legend()

  plt.figure()
  #plt.plot(times,prox0,'ro',label='Prox0 -- inner')
  plt.plot(timestamp,prox1,'bo',label='Prox1 -- outer')
  plt.ylabel('Output')
  plt.xlabel('Time (s?)')
  plt.legend()

  plt.figure()
  #plt.plot(runnum,lux0,'ro',label='Lux0 -- inner')
  plt.plot(runnum,lux1,'bo',label='Lux1 -- outer')
  plt.ylabel('Luminosity')
  plt.xlabel('Run number (~3s each)')
  plt.legend()

  plt.show()

      

  
if __name__ == "__main__":
    main()
