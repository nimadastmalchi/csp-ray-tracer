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

RayTracer::RayTracer(const float& time, const Point& colLoc, const Point& colDim, const int& N, const float& rMin, const float& rMax, const float& panelSize, const float& panelDist, const float& zInc) {
    sun = Sun(time);
    collector = Collector(colLoc, colDim.getX(), colDim.getY(), colDim.getZ());
    this->N = N;
    this->rMin = rMin;
    this->rMax = rMax;
    this->panelSize = panelSize;
    this->panelDist = panelDist;
    this->zInc = zInc;
    init_k = 0;
    totalArea = 0;
    flux = 0;
    tempRateAtCollector = 0;
    powerAtCollector = 0;
}

Sun& RayTracer::getSun() {
    return sun;
}

void RayTracer::setup(int mode) {
    //panels.clear();

    ofstream panelFile("~panel_lines.txt");
    ofstream collectorFile("~collector_lines.txt");
    ofstream sunFile("~sun_path.txt");

    collector.printGnuplot(collectorFile);
    sun.printPath(0.5, collector.getCenter().getZ(), sunFile);

    float k = init_k;

    if (mode == 0) {
        float theta_inc;
        for (float r = rMin; r < rMax; r += panelDist / 1.3) { // Dividing by 1.2 for testing
            theta_inc = (panelDist / r) * PI / 6;
            for (float theta = 0; theta < 2 * PI; theta += theta_inc) {
                panels.push_back(Panel(Point(r * cos(theta), r * sin(theta), k), sun.getDirection(), collector.getCenter(), panelSize));
                panels.back().printGnuplot(panelFile);
                totalArea += pow(panels.back().getLength(), 2);
            }
            k += zInc;
        }
    }

    else if (mode == 1) {
        float r = 0.2;
        for (float theta = 0; theta < 2 * PI - 0.5; theta += PI / 4) {
            panels.push_back(Panel(Point(r * cos(theta), r * sin(theta), k), sun.getDirection(), collector.getCenter(), panelSize));
            panels.back().printGnuplot(panelFile);
            totalArea += pow(panels.back().getLength(), 2);
        }

        r = 0.4;
        for (float theta = 0; theta < 2 * PI - 0.1; theta += PI / 8) {
            panels.push_back(Panel(Point(r * cos(theta), r * sin(theta), k), sun.getDirection(), collector.getCenter(), panelSize));
            panels.back().printGnuplot(panelFile);
            totalArea += pow(panels.back().getLength(), 2);
        }

        r = 0.6;
        for (float theta = 0; theta < 2 * PI - 0.1; theta += PI / 12) {
            panels.push_back(Panel(Point(r * cos(theta), r * sin(theta), k), sun.getDirection(), collector.getCenter(), panelSize));
            panels.back().printGnuplot(panelFile);
            totalArea += pow(panels.back().getLength(), 2.0);
        }
        totalArea *= MIRROR_AREA_FRACTION;
    }

}

void RayTracer::generate() {
    float max_k = init_k + zInc * ((rMax - rMin) / panelDist);
    generateRays(N, Point(1.5 * rMax * cos(5 * PI / 4), 1.5 * rMax * sin(5 * PI / 4) * 1.5, init_k), Point(1.5 * rMax * cos(PI / 4), 1.5 * rMax * sin(PI / 4), max_k), sun, collector, panels, hitPanel, missPanel, hitCollector, missCollector, collector.getMaxZ().getd());
    setPowerData();
}

void RayTracer::setPowerData() {
    cout << "RayTracer::setPowerData() -- Sun's direction vector: " << sun.getDirection() << endl;
    cout << "RayTracer::setPowerData() -- Sun's Normal Angle: " << sun.getNormalAngle() << endl;
    powerAtCollector = 0;
    flux = K * pow(SUN_TEMP, 4) * pow(SUN_RADIUS / SUN_DISTANCE, 2) * abs(cos(sun.getNormalAngle() * PI / 180)) * RADIATION_FRACTION;
    for (vector<Panel>::iterator panelIdx = panels.begin(); panelIdx != panels.end(); panelIdx++) {
        powerAtCollector += flux * (totalArea / panels.size()) * abs(cos(panelIdx->getNormal().getAngle(sun.getDirection()))) * MIRROR_RADIATION_FRACTION;
    }
    if (hitPanel.size() != 0) {
        cout << "RayTracer::setPowerData() -- hitCollector.size()/hitPanel.size() = " << (float)hitCollector.size() / hitPanel.size() << endl;
        powerAtCollector *= ((float)hitCollector.size() / hitPanel.size());
    }
    else {
        powerAtCollector = 0;
    }

    // The sun also hits the collector directly
    float area1 = collector.getLength() * collector.getHeight();
    float area2 = area1;
    float area3 = collector.getLength() * collector.getWidth();
    float directPower1 = area1 * flux * abs(cos(Vector(1, 0, 0).getAngle(sun.getDirection())));
    float directPower2 = area2 * flux * abs(cos(Vector(0, 1, 0).getAngle(sun.getDirection())));
    float directPower3 = area3 * flux * abs(cos(Vector(0, 0, 1).getAngle(sun.getDirection())));
    float directPower = directPower1 + directPower2 + directPower3;
    powerAtCollector += directPower;
    cout << "RayTracer::setPowerData() -- Direct Power -- " << directPower << endl;

    cout << "RayTracer::setPowerData() -- powerAtCollector() -- " << powerAtCollector << endl;
    tempRateAtCollector = collector.calcTemperature(powerAtCollector, DENSITY, MASS, SH);
    cout << "RayTracer::setPowerData() -- tempRateAtCollector() -- " << tempRateAtCollector << endl;
}

// Only works when the panels are setup for the time at which this function is called
void RayTracer::setPanelContributions() {
    float sum = 0;
    for (vector<Panel>::iterator panelIdx = panels.begin(); panelIdx != panels.end(); panelIdx++) {
        float panelArea = pow(panelIdx->getLength(), 2.0);
        panelIdx->power() = flux * panelArea * abs(cos(panelIdx->getNormal().getAngle(sun.getDirection())));
        sum += flux * panelArea * abs(cos(panelIdx->getNormal().getAngle(sun.getDirection())));
    }
}

void RayTracer::visualize() {
    printRays(missPanel, hitPanel, missCollector, hitCollector);
}

void RayTracer::printPanelData() {
    ofstream panelPower("~ind_panel_power.txt");
    int count = 0;
    for (vector<Panel>::iterator panelIdx = panels.begin(); panelIdx != panels.end(); panelIdx++) {
        panelPower << "Panel " << ++count << ": " << panelIdx->power() << "W of power contributed." << endl;
    }
}

void RayTracer::eraseRayPowerData() {
    missPanel.clear();
    hitPanel.clear();
    missCollector.clear();
    hitCollector.clear();

    flux = 0;
    powerAtCollector = 0;
    tempRateAtCollector = 0;
}

void RayTracer::erasePanelData() {
    panels.clear();
    totalArea = 0;
}

int RayTracer::getNumOfPanels() const {
    return panels.size();
}

void RayTracer::info() const {
    int total = missPanel.size() + hitPanel.size();
    cout << panels.size() << " panels generated" << endl;
    cout << total << " rays generated" << endl;
    cout << hitPanel.size() << " rays hit a panel" << endl;
    cout << hitCollector.size() << " rays hit the collector" << endl;
    cout << missCollector.size() << " rays miss the collector" << endl;
    cout << endl;
    cout << "Flux from the sun: " << flux << " W/m^2" << endl;
    cout << "Total power at the collector: " << powerAtCollector << " W" << endl;
    cout << "Temperature rate at the collector: " << tempRateAtCollector << " K/s" << endl;
}

float RayTracer::getpowerAtCollector() const {
    return powerAtCollector;
}

float RayTracer::getTempRateAtCollector() const {
    return tempRateAtCollector;
}

float getSurroundingTemp(const float& time) {
    //return -0.25 * pow(time, 2) + 7.5 * time - 24.25;
    return 45;
}

// Prints temperature and power data from tMin to tMax. tMin must be the time that the concentrated plant is set up. 
void printVarSunData(const float& tMin, const float& tMax, const float& tInc, const Point& colLoc, const Point& colDim, const int& N, const float& rMin, const float& rMax, const float& panelSize, const float& panelDist, const float& zInc) {
    ofstream varSunTemp("~var_sun_temp.txt");
    ofstream varSunPower("~var_sun_power.txt");

    float actualTemp = getSurroundingTemp(tMin);
    float previousTemp = getSurroundingTemp(tMin);
    float k = 0.05 / 60;
    for (float t = tMin; t <= tMax; t += tInc) {
        RayTracer r(t, colLoc, colDim, N, rMin, rMax, panelSize, panelDist, zInc);
        r.setup(SETUP_MODE);
        r.generate();
        actualTemp += (r.getTempRateAtCollector() - k * (previousTemp + r.getTempRateAtCollector() * (tInc * 3600) - getSurroundingTemp(t))) * (tInc * 3600);
        cout << "printVarSunData(...) -- Temperature at time " << t << ": " << actualTemp << " deg celsius." << endl;
        previousTemp = actualTemp;
        varSunTemp << t << " " << actualTemp << endl;
        varSunPower << t << " " << r.getpowerAtCollector() << endl;
    }
}

// Prints temperature and power data from tMin to tMax; the mirrors are adjusted every tInc. tMin must be the time that the concentrated plant is set up. 
void printVarSunDataIncPolar(float tMin, float tMax, float tInc, float changeInc, const Point& colLoc, const Point& colDim, const int& N, const float& rMin, const float& rMax, const float& panelSize, const float& panelDist, const float& zInc) {
    ofstream varSunIncPower("~var_sun_inc_power.txt");

    int count = 0;
    int int_changeInc = changeInc / tInc;

    RayTracer r(0, colLoc, colDim, N, rMin, rMax, panelSize, panelDist, zInc);
    for (float t = tMin; t <= tMax; t += tInc) {
        cout << "Time: " << t << endl;
        r.getSun() = Sun(t);
        if (count++ % int_changeInc == 0) {
            r.erasePanelData();
            r.setup(SETUP_MODE);
        }
        r.generate();
        varSunIncPower << t << " " << r.getpowerAtCollector() << endl;
        r.eraseRayPowerData();
    }
}

// The mirrors are set up for tMin and no longer adjusted. tMin must be the time the concentrated plant is set up under the sun. This function is used to compare to experimental results. The mirrors are adjusted for a certain time and no longer changed.
void printVarSunDataFixed(const float& tMin, const float& tMax, const float& tInc, const Point& colLoc, const Point& colDim, const int& N, const float& rMin, const float& rMax, const float& panelSize, const float& panelDist, const float& zInc) {
    ofstream varSunTempFixed("~var_sun_temp_fixed.txt");
    ofstream varSunPowerFixed("~var_sun_power_fixed.txt");

    float actualTemp = getSurroundingTemp(tMin);

    // INITIAL TEMP using the symbol INITIAL TEMP
    actualTemp = INITIAL_TEMP;

    float k = 0.005;

    RayTracer r(tMin, colLoc, colDim, N, rMin, rMax, panelSize, panelDist, zInc);
    r.setup(SETUP_MODE);
    cout << "Number of Panels generated for printVarSunDataFixed(...): " << r.getNumOfPanels() << endl;

    int count = 0;

    float tempInc = 0;
    for (float t = tMin; t <= tMax; t += tInc) {
        cout << "TIME: " << t << endl;
        cout << "Number of minutes passed: " << (t - tMin) * 60 << endl;
        r.getSun() = Sun(t);
        r.generate();
        tempInc = (r.getTempRateAtCollector() - k * (actualTemp + r.getTempRateAtCollector() * (tInc * 3600) - getSurroundingTemp(t))) * (tInc * 3600);
        cout << "printVarSunDataFixed(...) -- Delta Temperature (deg C): " << tempInc << endl;
        cout << "printVarSunDataFixed(...) -- Temp Rate (deg C/s): " << tempInc / (tInc * 3600) << endl;
        actualTemp += tempInc;
        cout << "printVarSunDataFixed(...) -- Temperature: " << actualTemp << " deg celsius." << endl;

        // printing into the file:
        varSunTempFixed << t << " " << actualTemp << endl;
        varSunPowerFixed << t << " " << r.getpowerAtCollector() << endl;

        count++;

        r.eraseRayPowerData();

        if (tempInc < 0) {
            cout << endl << endl << endl << "************************ MAX REACHED ************************" << endl << endl << endl;
        }

        cout << "------------------------------------" << endl;
    }
}

/*
Ts - outside temperature
Tc - temperature cooled
Tr - temperature at the but not cooled in the last dt
Ta - temperature added to receiver from mirrors
r(t) - temperature rate at a certain time t
k - cooling time constant ([k] = 1/s)


T - Actual temperature at collector
Tp - Actual temperature at collector dt time ago

dTa = r(t) * dt  -- > integrate: Ta -> 0 to Ta, t -> 0 to t

Tr = Tp + dTa

Newton's Law of Cooling
dTc/dt = -k(Tr - Ts)
dTc/dt = -k(Tp + dTa - Ts)
dTc/dt = -k(Tp + r(t) * dt - Tsd)
dTc = -k(Tp + r(t) * dt - Ts) * dt

dT = dTa + dTc
dT = r(t) * dt - k(Tp + r(t) * dt - Ts) * dt
dT = [r(t) - k(Tp + r(t) * dt - Ts)] * dt

------------------------------------------

-code:

float actualTemp = getSurroundingTemp(tMin);
float previousTemp = getSurroundingTemp(tMin);
float k = 0.05/60;
for (float t = tMin; t <= tMax; t += tInc) {
     RayTracer r(t, colLoc, colDim, N, rMin, rMax, panelSize, panelDist, zInc);
     r.setup();
     r.generate();
     actualTemp += (r.getTempRateAtCollector() - k * (previousTemp + r.getTempRateAtCollector() * (tInc * 3600) - getSurroundingTemp(t))) * (tInc * 3600);
     cout << actualTemp << endl;
     previousTemp = actualTemp;
     varSunTemp << t << " " << actualTemp << endl;
     varSunPower << t << " " << r.getpowerAtCollector() << endl;
}

*/

#endif