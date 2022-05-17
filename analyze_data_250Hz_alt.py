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
from scipy import fftpack
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



  prox0_all = []
  prox1_all = []
  lux0_all = []
  lux1_all = []
  distance0_all = []
  distance1_all = []
  timestamp_all = []
  time_all = []
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
  
  #for file_name in files:
  #  num = int(file_name.split("_")[-1].split(".dat")[0])
  for num in range(0,10000):
    filename = datadir + "/" + PREFIX + ("_%d.dat" % num)
    if not os.path.exists(filename): continue
    print ('Processing data file #%d, run=%d' % (nfiles,num))
    runnum.append(num)
    
    prox0_file = []
    prox1_file = []
    lux0_file = []
    lux1_file = []
    distance0_file = []
    distance1_file = []
    time_file = []
    full_time_file = []


    ##cut times in sec
    tmin,tmax = 4,6

    ## average samples in groups of (if 1, no averaging)
    avg_by_n=1

    current_file = open(filename,'r')
    nline = 0
    for line in current_file:
      if line[0] == '#':
          continue
      ### We no longer have the time output in the first line
      #if nline == 0:
      #    line_parsed = parser.parse(line)
      #    line_date2num = matplotlib.dates.date2num(line_parsed)
      #    timestamp.append(line_date2num)
      #    #timestamp.append(line_parsed)
      #    #if nfiles>1:
      #      #delta =  timedelta(timestamp[-1]-timestamp[-2]).total_seconds()
      #      #timestamp_delta.append( delta )
      #      #print line, line_parsed, line_date2num, delta
      #    nline += 1
      #    continue


      ## skip line if sensor readout is <2300
      if int(line.split()[3])<2260: continue


      ## Read sensor values in the file
      timestamp_all.append( matplotlib.dates.date2num( parser.parse( line.split()[0] ) ) )    
      if nline==0 and nfiles==0: time_all.append(0)
      else: 
        delta_seconds = timedelta( timestamp_all[-1]-timestamp_all[-2] ).total_seconds()
        time_all.append(time_all[-1]+delta_seconds)
      if nline==0: 
        full_time_file.append(0)
      else: 
        full_time_file.append(full_time_file[-1]+delta_seconds)

      ## Here we could apply time cuts, if desired
      if full_time_file[-1]>tmin and full_time_file[-1]<tmax:
        time_file.append(full_time_file[-1])
        prox0_all.append(int(line.split()[1]))
        lux0_all.append(int(line.split()[2]))
        prox1_all.append(int(line.split()[3]))
        lux1_all.append(int(line.split()[4]))
        #time_250Hz.append(timestamp[-1]+nline*4e-9)
        
        prox0_file.append(int(line.split()[1]))
        lux0_file.append(int(line.split()[2]))
        prox1_file.append(int(line.split()[3]))
        lux1_file.append(int(line.split()[4]))

        x = prox0_file[-1]
        dist0 = (364.315-0.253896*x+7.42632e-05*x**2-9.12668e-09*x**3+4.44983e-13*x**4)*np.exp(-0.000482712*x)
        if dist0<0: 
          dist0=0
        distance0_all.append(dist0)
        distance0_file.append(dist0)
        x = prox1_file[-1]
        dist1 = (73.1555-0.0267666*x+6.79842e-06*x**2-5.78545e-10*x**3+3.00264e-14*x**4)*np.exp(-0.000357827*x)
        if '200uA' in datadir:
          # Use calibration for 200uA
          x_abv = x-2211 ## sensor output above sensor noise level
          if x_abv<1000: dist1=86.1731-0.209823*x_abv+0.000251952*x_abv**2-1.06007e-07*x_abv**3
          else: dist1=19.6794-0.00266401*x_abv+1.31008e-07*x_abv**2
          dist1*=2 ## for the 200uA calibration
        if dist1<0:
          dist1=0
        distance1_all.append(dist1)
        distance1_file.append(dist1)

      nline += 1


    ## Finished reading entries in file, now plot this file alone
    ## Only if we have read outs that saw something
    if len(distance1_file)==0: 
      print 'No good entries in file, skipping'
      continue
    ## Also skip plotting file if buffers were filled -> the std dev of values is too small 
    if np.std(distance1_file)<0.0001:
      print 'Skipping file, frozen registers'
      continue
    ## Also skip plotting file if variation was too large 
    #if np.max(distance1_file)-np.min(distance1_file)>5: continue

    distance1_file = np.array(distance1_file)
    time_file = np.array(time_file)

    ## Average to limit the sampling rate, which is too high, >600Hz apparently
    distance1_avgN=[]
    time_avgN=[]
    for i in range(0,len(distance1_file)-(avg_by_n-1),avg_by_n):
      distance_avg = distance1_file[i]
      time_avg = time_file[i]
      for i_avg in range(0,avg_by_n-1):
        distance_avg += distance1_file[i+i_avg]
        time_avg += time_file[i+i_avg]
      distance1_avgN.append( distance_avg/avg_by_n )
      time_avgN.append( time_avg/avg_by_n )


    ## Now do the FFT
    fft_input = distance1_avgN
    sampling_rate = len(time_avgN)/(time_avgN[-1]-time_avgN[0]) ## assumes continuous entries in file, after cuts above
    print 'sampling rate: %f Hz' % sampling_rate
    ## from fftpack
    dist_fft = fftpack.fft(fft_input)
    freqs = fftpack.fftfreq(len(fft_input)) * sampling_rate
    ## from np.fft
    fft_x = np.fft.rfftfreq(len(fft_input), d=1/sampling_rate) #* 2.*math.pi
    fft_y = np.fft.rfft(fft_input)

    ## Find peak amplitude
    peak_amplitude,freq_at_peak = None,None
    for x,y in zip(fft_x[1:],np.abs(fft_y[1:])):
      if y>peak_amplitude:
        peak_amplitude = y
        freq_at_peak = x
    print 'Max amplitude = %f   @   freq = %f' %(peak_amplitude,freq_at_peak)
    ## Skip if we only see noise
    if peak_amplitude<1: continue

    ## Plot
    title='%s , Run1 %d' % (datadir,num)
    fig = plt.figure(figsize=(14,9))
    fig.text(0.5,0.97,title,size='x-large', 
      horizontalalignment='center', verticalalignment='top', 
      multialignment='center')
    fig.subplots_adjust(
      left=0.08, bottom=0.08, right=0.93, top=0.88) #hspace=0.

    distance_plot = fig.add_subplot(2,1,1,
      #sharex=fft_plot, 
      xlabel='Time [s]', ylabel='Distance [mm]')
    distance_plot.plot(
      time_avgN,distance1_avgN, 
      #label='real',
      color='red'
    )
    #distance_plot.axhline(0.,linestyle=':',color='gray')
    #fft_plot_bottom.legend(loc=0)

    fft_plot = fig.add_subplot(2,1,2,
      xlabel='frequency [Hz]', ylabel='FT Magnitude'
    )
    #fft_plot.xaxis.tick_top() # draw ticklabels at top
    #fft_plot.xaxis.set_ticks_position('both') # tickmarks at top and bottom
    #fft_plot.xaxis.set_label_position('top') # redundant?
    fft_plot.plot(
      fft_x[1:], np.abs(fft_y[1:]), 
      color='black', 
      label='abs'
    )
    #fft_plot.figure.text(0.4,0.8,'peak: %.6f rad Ghz'%(freq_at_peak,))
    #fft_plot.axis(xmax=fft_x[-1])
    #fft_plot.set_ylim(-5,100)
    fft_plot.set_yscale('log')




    '''
    fig, ax = plt.subplots(2)
    ax[0].plot(time_avg2,distance1_avg2,'b',markersize=1,label='Distance, single-file')
    ax[0].set_ylabel('Distance (mm)')
    ax[0].set_xlabel('Time(s)')

    ax[1].stem(freqs, np.abs(dist_fft))
    ax[1].set_xlabel('Frequency in Hertz [Hz]')
    ax[1].set_ylabel('Frequency Domain (Spectrum) Magnitude')
    ax[1].set_xlim(0, sampling_rate / 2)
    ax[1].set_ylim(-5, 100)
    '''

    plt.show()
    fig.savefig('%s/proximity_run%d.png' % (datadir, num))





    nfiles += 1

    #if num == 0:
    #  time0 = timestamp[-1]
          
  print 'Processed %d files' % nfiles

  #times = matplotlib.dates.date2num(timestamp)
  

      
  
  
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
