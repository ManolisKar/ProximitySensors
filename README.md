# Quadrupole plates alignment survey

## Motivation

The quadrupole plates generate the E-field experienced by the muons in the Muon g-2 experiment. 
They consist of 4 sets (Q1-Q4), each with a short and long segment, for a total of 8 quadrupole sections (Q1S, Q1L, Q2S, ...). There are 4 plates in each section: top, bottom, inner, outer.

The plates had only been surveyed before installation into the vacuum chambers, where access is difficult. 
That survey was performed with a specialized and very accurate laser scan by the Fermilab Alignment Department. 
Only the average plate locations were quoted from that scan. 
I was the first to look at individual plates in much higher detail, and found significant deviations from their design location.


<p align = "center">
<img src="https://github.com/ManolisKar/ProximitySensors/blob/master/images/Q1S_laser_scan.png?raw=true" alt="Trulli" style="width:80%">
</p>
<p align = "center">
<sup>
Fig. 1: Laser survey of Q1S bottom plate location, in cylindrical coordinates. The deviation from the ideal location (blue surface) is significant.
</sup>
</p>

<p align = "center">
<img src="https://github.com/ManolisKar/ProximitySensors/blob/master/images/Q1_survey.png?raw=true" alt="Trulli" style="width:100%">
</p>
<p align = "center">
<sup>
Fig. 2: Top 2 panes: Radial-azimuthal depiction of Q1S top and bottom plates surveyed points. Color-coding is the deviation from their design vertical location. The specification is up to 2 mm deviation. The red bands on the right highlight the color-code that doesn't meet the specification. Bottom 2 panes: Vertical-azimuthal depiction of Q1S inner and outer plates surveyed points. Color-coding is the deviation from their design radial location. The outer plate is made from aluminized mylar, as opposed to solid aluminum for all other plates. The thin mylar sheet forms "waves" when stretched, which are captured in the survey. 
</sup>
</p>


Exact knowledge of all plate locations is very important for input to simulations of their produced field. 
Quantification of uncerainty on that knowledge is also very important. 
However we can't claim to have either, as plates were only surveyed before they were actually installed. 
For that reason I designed two in-situ surveys, one of which is presented here. 


## In-situ survey with proximity sensors

The requirements of an in-situ measurement are very challenging:
* We need to access inside the very tight and hard-to-reach area inside the vacuum chambers, just 10 cm in diameter.
* We need to measure the location of the plates with high precision.
* We cannot touch or disturb the sensitive plates. 

Given these challenging requirements, the Alignment Department was considering the purchase of highly specialized equipment in the tens of thousands $$. 
Instead I was able to perform the measurement with inexpensive infrared sensors, commonly used in robotic vision, mounted on the vertical surface of a "trolley" that can access the vacuum region to sample the magnetic field.


<p align = "center">
<img src="https://github.com/ManolisKar/ProximitySensors/blob/master/images/VCNL4010.png?raw=true" alt="Trulli" style="width:60%">
</p>
<p align = "center">
<sup>
Fig. 3: The VCNL4010 proximity sensors (https://www.vishay.com/docs/83462/vcnl4010.pdf). They can perform a distance measurement via an IR emitter and matched photodiode.
</sup>
</p>


The VCNL sensors were controlled by a Raspberry pi board, which was powered by a battery, so that the measurement can be made completely autonomously without the need for cables inside the vacuum region that could affect sensitive equipment. 
The entire system was fit on a specially designed and machined fixture, which was mounted on the 9 cm diameter vertical surface of the trolley.

<p align = "center">
<img src="https://github.com/ManolisKar/ProximitySensors/blob/master/images/fixture.png?raw=true" alt="Trulli" style="width:70%">
</p>
<p align = "center">
<sup>
Fig. 4: The sensors-Rpi-battery fixture mounted on the vacuum trolley. 
</sup>
</p>


The fixture with a pair of sensors mounted to sample the inner and radial plates. The trolley traversed a long distance around the ring, passing through each quad segment twice. The raw data is shown in Fig. 5 for the two sensors. 


<p align = "center">
<img src="https://github.com/ManolisKar/ProximitySensors/blob/master/images/raw_sensor_data.png?raw=true" alt="Trulli" style="width:90%">
</p>
<p align = "center">
<sup>
Fig. 5: The raw distance measurement from the two sensors as they sampled quadrupole plates. 
</sup>
</p>


## Calibration and results

The raw sensor data still have to be calibrated before we can extract the plates location in global ring coordinates. 
Each data point in Fig. 5 correponds to the average from 3s of data collection from the sensors. 
Under the reasonable assumption of constant trolley velocity, we can map the data collected along the azimuthal extent of each plate. 

By characterizing the geometric properties of the fixture, we can translate the sensor distance measurements to the distance from the center of the fixture/trolley. 
Finally we use the previously surveyed locations of the trolley rails, to take into account their variation, and also to translate the sensor measurement into global coordinates.

Finally we are able to plot the in-situ sensor measurement against the expectation from the pre-installation laser survey, in Figures 6,7. 
Compare the representation of the Q1S inner and outer plates laser scan data between Figures 2 and 6. Notice that more "spread" in the data of Figures 6,7 suggests a tilt of the plates at a given azimuthal location. 




<p align = "center">
<img src="https://github.com/ManolisKar/ProximitySensors/blob/master/images/Q1S_final.png?raw=true" alt="Trulli" style="width:100%">
</p>
<p align = "center">
<sup>
Fig. 6: Q1S inner (left) and outer (right) plates locations, as measured from independent surveys. Red triangles: In-situ proximity sensors measurement. Inverted and upright triangles correspond to the two trolley passes. Black dots: Pre-installation laser survey. Green dots: Pre-installation laser survey, within 5 mm from the vertical center. Blue crosses: Independent survey through vacuum flange. 
</sup>
</p>


<p align = "center">
<img src="https://github.com/ManolisKar/ProximitySensors/blob/master/images/Q2S_final.png?raw=true" alt="Trulli" style="width:100%">
</p>
<p align = "center">
<sup>
Fig. 7: As in Fig. 6, but for Q2S plates. 
</sup>
</p>


The proximity sensors measurement is plotted separately for the two passes of the trolley, with inverted and upright red triangles. The two trolley passes were performed in opposite directions no less. The apparent excellent agreement between the two passes is a strong indication of the stability of the sensors' measurement, and the accuracy of our calibraiton. 

The laser scan data are plotted in green near the plates' vertical center. Because the sensors mostly sample that area near the vertical center, we should mostly be comparing those. 
For these two sets of plates of Figures 6,7, we achieve exceptional agreement between two completely independent and very challenging measurements. 
We even have the sensitivity to identify small discrepancies at the edges of Q1S inner plate, that may indicate small displacements that could have occurred during installation. 

Through these studies we gain very accurate knowledge of the average location of all plates, which is a very important input to simulations of the E-field generated by the plates and its effect on particle beam dynamics. 
We are also able to constrain significantly the uncertainty of the plates' location, and thus strongly bound systematic effects that could bias the muon spin precession measurement. 
