# CSPRayTracer
Author: Nima Dastmalchi
Data: 03/29/2021

A Concentrated Solar Power (CSP) system simulator that utilizes ray-tracing. Rays are generated toward an array of mirrors
which then reflect each toward a collector. The temperature and power outputs of the collector are calculated.

Running the program with different implemented functions outputs various files. When those data files are generated, the 
commands on top of main.cpp can be put in Gnuplot to visualize the ray tracer or plot the data calculated.

Input: Certain configuration of a CSP system, time interval in the day, time of year, location on earth, etc.
Output: Temperature and power at the receiver of the CSP system. A file read by Gnuplot to generate the following diagram:

![alt text](https://github.com/nimadastmalchi/csp-ray-tracer/blob/main/gnuplot-example-output.png?raw=true)

Please see research paper for detailed methodology: https://thinkyou.bayhonors.org/images/2020Symposium/NimaDastmalchi_SolarPowerCapture.pdf
