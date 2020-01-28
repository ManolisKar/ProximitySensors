# This code is designed to work with the VCNL4010_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/content/Light?sku=VCNL4010_I2CS#tabs-0-product_tabset-2

### 20200120 out dir: time read out once at beginning of each file
### _alt output dir: time read out on each measurement
### _alt_alt out dir: no print out statement on screen

import smbus
import time
import os
import glob
from datetime import datetime
start_time=datetime.now()




def main():

    
    import argparse
    parser = argparse.ArgumentParser(description=__doc__,epilog=' ',formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('-N',type=int,default=1000,help='number of readouts')
    args = parser.parse_args()

    
    ## Get the two I2C buses, 0 and 1
    bus0 = smbus.SMBus(0)
    bus1 = smbus.SMBus(1)

    # VCNL4010 address, 0x13(19)
    # Select command register, 0x80(128)
    # 0xFF(255)	Enable ALS and proximity measurement, LP oscillator
    ##bus0.write_byte_data(0x13, 0x80, 0xFF)
    ##bus1.write_byte_data(0x13, 0x80, 0xFF)
    # 0x18 Enable single on-demand ALS and proximity measurement
    ##    bus0.write_byte_data(0x13, 0x80, 0x18)
    ##    bus1.write_byte_data(0x13, 0x80, 0x18)
    
    # VCNL4010 address, 0x13(19)
    # Select IR LED current register, 0x83
    # 0x03	100 mA
    ##bus0.write_byte_data(0x13, 0x83, 0x03)
    ##bus1.write_byte_data(0x13, 0x83, 0x03)

    # VCNL4010 address, 0x13(19)
    # Select proximity rate register, 0x82(130)
    # 0x00(00)	1.95 proximity measeurements/sec
    # 0x07    	250  proximity measeurements/sec
    ##bus0.write_byte_data(0x13, 0x82, 0x07)
    ##bus1.write_byte_data(0x13, 0x82, 0x07)


    delay = 0.1 
    nmeasure = args.N # total number of measurements per script execution
    
    

    ## Open output file 

    #topdir = os.path.dirname(os.path.realpath(__file__))
    topdir = '/home/pi/QuadsAlignment/'
    rundir = os.path.join(topdir,'data/20200120_alt')
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
    #fileout.write('%s\n' % str(datetime.now()-start_time))
    fileout.write('## Data format:\n##str(datetime)\tprox0\tlux0\tprox1\tlux1\n')
    print 'Writing output to %s' %filename


    ### Measurement and readout
    previous0,previous1 = 0,0
    filled_registers=1
    
    for i in range(nmeasure):

        # VCNL4010 address, 0x13(19)
        # Select command register, 0x80(128)
        # 0xFF(255)	Enable ALS and proximity measurement, LP oscillator
        ##bus0.write_byte_data(0x13, 0x80, 0xFF)
        #    bus1.write_byte_data(0x13, 0x80, 0xFF)
        # 0x18 Enable single on-demand ALS and proximity measurement
        ###bus0.write_byte_data(0x13, 0x80, 0x18)

        # VCNL4010 address, 0x13(19)
        # Select ambient light register, 0x84(132)
        # 0x9D(157)	Continuous conversion mode, ALS rate 2 samples/sec
        ###bus0.write_byte_data(0x13, 0x84, 0x9D)

        #time.sleep(delay)
    
        # VCNL4010 address, 0x13(19)
        # Read data back from 0x85(133), 4 bytes
        # luminance MSB, luminance LSB, Proximity MSB, Proximity LSB
        ###data = bus0.read_i2c_block_data(0x13, 0x85, 4)
    
        # Convert the data
        ###proxi0 = data[2] * 256 + data[3]
        ###lumi0 = data[0] * 256 + data[1]
        proxi0 = 0
        lumi0 = 0
    

        ## Now repeat for the other sensor
        ##bus1.write_byte_data(0x13, 0x80, 0xFF)
        bus1.write_byte_data(0x13, 0x80, 0x18)
        bus1.write_byte_data(0x13, 0x84, 0x9D)
        #time.sleep(delay)
        data = bus1.read_i2c_block_data(0x13, 0x85, 4)
        proxi1 = data[2] * 256 + data[3]
        lumi1 = data[0] * 256 + data[1]

        # Output data to screen
        print "#%d\t%s  :  Bus0/Bus1 Proximity and Ambient Light Luminance : %d\t%d\t%d\t%d" % (i,str(datetime.now()-start_time),proxi0, lumi0, proxi1, lumi1)

    
        fileout.write('%s\t%d\t%d\t%d\t%d\n' % (str(datetime.now()-start_time),
                                                proxi0, lumi0, proxi1, lumi1))

        if i>0 and proxi1-previous1!=0: ## or proxi0-previous0!=0:
            ## registers are fine
            filled_registers=0             

        previous0=proxi0
        previous1=proxi1

    return filled_registers




main()
