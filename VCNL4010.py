# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# VCNL4010
# This code is designed to work with the VCNL4010_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/content/Light?sku=VCNL4010_I2CS#tabs-0-product_tabset-2

### This is the script that runs on the Rpi to take data with the sensors

import smbus
import time
import os
import glob

# Get the two I2C buses, 0 and 1
bus0 = smbus.SMBus(0)
bus1 = smbus.SMBus(1)

delay = 0.5

# Open output file 
#topdir = os.path.dirname(os.path.realpath(__file__))
topdir = '/home/pi/QuadsAlignment/'
rundir = os.path.join(topdir,'data')
if not os.path.exists(rundir):
    os.makedirs(rundir)

PREFIX = "VCNL4010"
files = glob.glob(rundir + "/" + PREFIX + "_*.dat")
lastrun = -1
for fn in files:
    num = int(fn.split("_")[-1].split(".dat")[0])
    if num > lastrun:
        lastrun = num
filename = '%s_%d.dat' % (PREFIX, lastrun+1)
fileout = open(os.path.join(rundir,filename),'w')
fileout.write('## Data format:\n## prox0\tlux0\tprox1\tlux1\n')
print 'Writing output to %s' %filename

while True:
    # VCNL4010 address, 0x13(19)
    # Select command register, 0x80(128)
    # 0xFF(255)	Enable ALS and proximity measurement, LP oscillator
    bus0.write_byte_data(0x13, 0x80, 0xFF)

    # VCNL4010 address, 0x13(19)
    # Select proximity rate register, 0x82(130)
    # 0x00(00)	1.95 proximity measeurements/sec
    bus0.write_byte_data(0x13, 0x82, 0x00)
    
    # VCNL4010 address, 0x13(19)
    # Select ambient light register, 0x84(132)
    # 0x9D(157)	Continuous conversion mode, ALS rate 2 samples/sec
    bus0.write_byte_data(0x13, 0x84, 0x9D)
    
    time.sleep(delay)
    
    # VCNL4010 address, 0x13(19)
    # Read data back from 0x85(133), 4 bytes
    # luminance MSB, luminance LSB, Proximity MSB, Proximity LSB
    data = bus0.read_i2c_block_data(0x13, 0x85, 4)
    
    # Convert the data
    proxi0 = data[2] * 256 + data[3]
    lumi0 = data[0] * 256 + data[1]
    

    ## Now repeat for the other sensor
    bus1.write_byte_data(0x13, 0x80, 0xFF)
    bus1.write_byte_data(0x13, 0x82, 0x00)
    bus1.write_byte_data(0x13, 0x84, 0x9D)
    time.sleep(delay)
    data = bus0.read_i2c_block_data(0x13, 0x85, 4)
    proxi1 = data[2] * 256 + data[3]
    lumi1 = data[0] * 256 + data[1]

    # Output data to screen
    print "Bus0/Bu1 Proximity and Ambient Light Luminance : %d\t%d\t%d\t%d" % (proxi0, lumi0, proxi1, lumi1)

    
    fileout.write('%d\t%d\t%d\t%d\n' % (proxi0, lumi0, proxi1, lumi1))

