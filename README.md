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

Given these challenging requirementst, the Alignment Department was considering the purchase of highly specialized equipment in the tens of thousands $. Instead I was able to perform the measurement with inexpensive infrared sensors, commonly used in robotic vision, mounted on the vertical surface of a "trolley" that can access the vacuum region to sample the magnetic field.



<p align = "center">
<img src="https://github.com/ManolisKar/ProximitySensors/blob/master/images/VCNL4010.png?raw=true" alt="Trulli" style="width:90%">
</p>
<p align = "center">
<sup>
Fig.3: The [VCNL4010 proximity sensors](https://www.vishay.com/docs/83462/vcnl4010.pdf). 
</sup>
</p>
