# CSPRayTracer
Author: Nima Dastmalchi

Data: 03/29/2021

A Concentrated Solar Power (CSP) system C++ simulator that utilizes ray tracing, 
laws of physics, linear algebra, and differential equations. 

## Background

A CSP system uses an array of mirrors placed around a receiver to concentrate
sunlight onto the receiver and generate heat, which is then converted to
electricity. This program is a simulation that generates rays toward an array
of virtual mirrors which then reflect each ray toward a receiver. The temperature
and power outputs of the receiver are calculated.

## Build and run

To build the simulator, simply run the make command after installing `g++` compiler:

```
> make
```

The input of the program is given in the `Input` directory. The configuration
of the CSP system is given in an input file in this directory. To specify
the locations of the mirrors in the rectangular coordinate system, specify
input in the `Input/~input_rec.txt` file. Otherwise, to use the polar coordinate
system, use the `Input/~input_polar.txt` file. Other data such as the time of the
day, the duration of the simulation, the size and location of the collector, mirror
dimensions, etc. are also specified in the input files.

Running the program with different implemented functions outputs various files.
When those data files are generated, the commands on top of main.cpp can be put
in Gnuplot to visualize the ray tracer or plot the data calculated. Alternatively,
you may install `gnuplot` and run the `run.sh` script to display some graphs and
output data associated with the simulation.

The format of the run function is `./run.sh <number>` where `<number>` specifies
a particular output file to plot. For example, when this argument is 1,
the generated rays, mirrors, collector, and trajectory of the sun are plotted.

```
> ./run.sh 1
```

## General input/output

Input: Certain configurations of a CSP system, time interval in the day, time of
year, location on earth, etc.

Output: Temperature and power at the receiver of the CSP system. Various plots to 
visualize the simulation

## Example output

![alt text](https://github.com/nimadastmalchi/csp-ray-tracer/blob/main/gnuplot-example-output.png?raw=true)

## Methodology

Please see the research paper for detailed methodology:
~https://thinkyou.bayhonors.org/images/2020Symposium/NimaDastmalchi_SolarPowerCapture.pdf~

https://doi.org/10.13140/RG.2.2.20408.93442

Access here if the above links do not work:

https://drive.google.com/file/d/1v3IZShhF47HLz45c6JaK69IsqogkkLTd/view?usp=sharing


