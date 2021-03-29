#ifndef RayTracer_h
#define RayTracer_h

#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include "Ray.h" // Also gets Position.h and Components.h

#define POLAR_INPUTS 13
#define REC_INPUTS 15

#define SUN_DISTANCE (1.49597870 * pow(10, 11))
#define SUN_TEMP 5778
#define SUN_RADIUS (6.9634 * pow(10, 8))
#define EARTH_SA (5.10 * pow(10, 14))
#define K (5.6696 * pow(10, -8))

#define MASS 0.3 // kg
#define SH 4186  // J / kg K
#define DENSITY 2710 // kg / m^3

#define RADIATION_FRACTION 0.73 // Fraction of radiation that arrives at earth's surface and not lost to the atmosphere
#define MIRROR_AREA_FRACTION 0.9 // Fraction of the surface of the mirrors not covered with alumimum sheets
#define MIRROR_RADIATION_FRACTION 1 // Fraction of the light radiated through the mirror.

#define SETUP_MODE 1

#define INITIAL_TEMP 39.7

class RayTracer {
private:
    // Input Data:
    Sun sun;
    Collector collector;
    int N;
    float rMin, rMax;
    float panelSize;
    float panelDist;
    float zInc;
    float init_k;

    // Panel/Ray Data:
    float totalArea;
    vector<Panel> panels;
    vector<Ray> missPanel, hitPanel, missCollector, hitCollector;

    // Power Data:
    float flux;
    float powerAtCollector;
    float tempRateAtCollector;

public:
    RayTracer(const float& time, const Point& colLoc, const Point& colDim, const int& N, const float& rMin, const float& rMax, const float& panelSize, const float& panelDist, const float& zInc);
    Sun& getSun();
    void setup(int mode); // Sets up panels using rmin, rmax, panelsSize, zIncrement
    void generate(); // Generates rays using on N, panels
    void setPowerData();
    void setPanelContributions();
    void visualize();
    void printPanelData();

    void eraseRayPowerData();
    void erasePanelData();

    int getNumOfPanels() const;

    void info() const;
    float getpowerAtCollector() const;
    float getTempRateAtCollector() const;
};

float getSurroundingTemp(const float& time);

// Prints temperature and power data from tMin to tMax. tMin must be the time that the concentrated plant is set up. 
void printVarSunData(const float& tMin, const float& tMax, const float& tInc, const Point& colLoc, const Point& colDim, const int& N, const float& rMin, const float& rMax, const float& panelSize, const float& panelDist, const float& zInc);

// Prints temperature and power data from tMin to tMax; the mirrors are adjusted every tInc. tMin must be the time that the concentrated plant is set up. 
void printVarSunDataIncPolar(float tMin, float tMax, float tInc, float changeInc, const Point& colLoc, const Point& colDim, const int& N, const float& rMin, const float& rMax, const float& panelSize, const float& panelDist, const float& zInc);

// The mirrors are set up for tMin and no longer adjusted. tMin must be the time the concentrated plant is set up under the sun. This function is used to compare to experimental results. The mirrors are adjusted for a certain time and no longer changed.
void printVarSunDataFixed(const float& tMin, const float& tMax, const float& tInc, const Point& colLoc, const Point& colDim, const int& N, const float& rMin, const float& rMax, const float& panelSize, const float& panelDist, const float& zInc);

#endif