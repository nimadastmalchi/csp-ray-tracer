# CSPRayTracer
Author: Nima Dastmalchi
Data: 03/29/2021

A Concentrated Solar Power (CSP) system simulator that utilizes ray-tracing. 

A CSP system uses an array of mirrors placed around a receiver to concentrate
sunlight onto the receiver and generate heat, which is then converted to
electricity. This program is a simulation that generates rays toward an array
of virtual mirrors which then reflect each ray toward a receiver. The temperature
and power outputs of the receiver are calculated.

Running the program with different implemented functions outputs various files.
When those data files are generated, the commands on top of main.cpp can be put
in Gnuplot to visualize the ray tracer or plot the data calculated.

Input: Certain configuration of a CSP system, time interval in the day, time of
year, location on earth, etc.
Output: Temperature and power at the receiver of the CSP system. A file read by
Gnuplot to generate the following diagram:

![alt text](https://github.com/nimadastmalchi/csp-ray-tracer/blob/main/gnuplot-example-output.png?raw=true)

Link to public repo: https://github.com/nimadastmalchi/csp-ray-tracer

Please see research paper for detailed methodology:
https://thinkyou.bayhonors.org/images/2020Symposium/NimaDastmalchi_SolarPowerCapture.pdf
