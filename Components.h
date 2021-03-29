#ifndef Components_h
#define Components_h

#include <iostream>
#include <fstream>
#include <math.h>
#include "Position.h"

#define PI (float)(2 * asin(1))

#define SUNRISE 6.617
#define SUNSET 18.667
#define NOON 12.45

class Panel {
private:
    Point R; // Center of the panel
    Plane equation;
    Vector normal;

    float maxX, maxY, minX, minY;
    float length;

    float powerContribution;

    // Returns the normal vector to the Panel
    void setNormal(Vector sun, Point Q) {
        Vector RQ(Q.getX() - R.getX(), Q.getY() - R.getY(), Q.getZ() - R.getZ());
        // Magnitudes:
        float SUNmag = sun.getMag();
        float RQmag = RQ.getMag();
        // Unit vectors:
        Vector SUNunit = sun / SUNmag;
        Vector RQunit = RQ / RQmag;

        Vector temp_normal = SUNunit + RQunit;
        normal = temp_normal / temp_normal.getMag();
    }
public:
    Panel();
    Panel(Point iR, Vector sun, Point collector, float length);

    Point getCenter() const;
    float getX() const;
    float getY() const;
    float getZ() const;
    float getMaxX() const;
    float getMaxY() const;
    float getMinX() const;
    float getMinY() const;
    float getLength() const;

    float& power();
    Plane getPlane() const;
    Vector getNormal() const;
    Plane& getPlane();
    Vector& getNormal();
    float getNormalAngle() const;

    void printGnuplot(ofstream& panelFile) const;
};


// **Angles are in degrees**
class Sun {
private:
    float time;
    Vector direction;

    void calcDirection() {
        // Realistic Path -- Time Consid:
        float zenith = 0.5585; // 32 degree zenith

        float newT;
        if (time < NOON) {
            newT = ((float)(time - SUNRISE) / (NOON - SUNRISE)) * (PI / 2);
        }
        else if (time > NOON) {
            newT = ((float)(time - NOON) / (SUNSET - NOON)) * (PI / 2) + PI / 2;
        }
        else {
            newT = PI / 2;
        }

        float x = cos(newT);
        float y = -sin(newT);
        float z = 1.6 * sin(newT);

        direction = Vector(x, y, z);
        

        /*
        // Realistic Path -- no Time consid:
        float newT = ((float)(time - 6) / 12) * PI;
        float x = cos(newT);
        float y = sin(newT) - sin(PI / 6);
        float z = 2.35 * sin(newT);
        direction = Vector(x, y, z);
        */

        /*
        // Symmetric Path:
        // x0 = 1. So y = tan(sunrise angle)
        float y = 0;
        float x = cos(getHorizonAngle() * (PI / 180));
        float z = x * tan(getHorizonAngle() * (PI / 180));
        direction = Vector(x, y, z);
        */
    }
public:
    Sun(float time);
    Sun() { time = 0; }
    Vector getDirection() const;
    float getTime() const;
    float getNormalAngle() const; // Must return in degrees
    float getHorizonAngle() const; // Must return in degrees
    void printPath(float inc, float height, ofstream& sunFile) const;
};

class Collector {
private:
    Point center;
    Plane xMin, xMax, yMin, yMax, zMin, zMax;
    float length, width, height;
public:
    Collector() {}
    Collector(Point icenter, float length, float width, float height);
    Collector(float ixMin, float ixMax, float iyMin, float iyMax, float izMin, float izMax);
    Plane& getMinX();
    Plane& getMaxX();
    Plane& getMinY();
    Plane& getMaxY();
    Plane& getMinZ();
    Plane& getMaxZ();
    Point& getCenter();
    float calcTemperature(float power, float density, float mass, float specificHeat);

    float getLength() const;
    float getWidth() const;
    float getHeight() const;
    void printGnuplot(ofstream& collectorFile) const;
};

ostream& operator<<(ostream& out, const Panel& panel);

#endif