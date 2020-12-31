#ifndef Components_h
#define Components_h

#include <iostream>
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
Panel::Panel() {
    maxX = maxY = minX = minY = 0;
    length = 0;
    powerContribution = 0;
}
Panel::Panel(Point iR, Vector sun, Point collector, float iLength) {
    R = iR;
    setNormal(sun, collector);
    equation.createPlane(normal, R);
    length = iLength;
    maxX = R.getX() + length / (float)2.0;
    maxY = R.getY() + length / (float)2.0;
    minX = R.getX() - length / (float)2.0;
    minY = R.getY() - length / (float)2.0;
    powerContribution = 0;
}
Point Panel::getCenter() const { return R; }
float Panel::getX() const { return R.getX(); }
float Panel::getY() const { return R.getY(); }
float Panel::getZ() const { return R.getZ(); }
float Panel::getMaxX() const { return maxX; }
float Panel::getMaxY() const { return maxY; }
float Panel::getMinX() const { return minX; }
float Panel::getMinY() const { return minY; }
float Panel::getLength() const { return length; }
float& Panel::power() { return powerContribution; }

Plane Panel::getPlane() const { return equation; }
Vector Panel::getNormal() const { return normal; }
Plane& Panel::getPlane() { return equation; }
Vector& Panel::getNormal() { return normal; }
float Panel::getNormalAngle() const {
    return normal.getAngle(Vector(0, 0, 1));
}

void Panel::printGnuplot(ofstream& panelFile) const {
    float x1 = R.getX() + length / 2; float x2 = R.getX() - length / 2;
    float y1 = R.getY() + length / 2; float y2 = R.getY() - length / 2;

    Point p1(x1, y1, equation.getZ(x1, y1)); // 10 10
    Point p2(x2, y1, equation.getZ(x2, y1)); // -10 10
    Point p3(x2, y2, equation.getZ(x2, y2)); // -10 -10
    Point p4(x1, y2, equation.getZ(x1, y2)); // 10 -10

    panelFile << p1.getX() << " " << p1.getY() << " " << p1.getZ() << " " << p2.getX() - p1.getX() << " " << p2.getY() - p1.getY() << " " << p2.getZ() - p1.getZ() << endl;
    panelFile << p2.getX() << " " << p2.getY() << " " << p2.getZ() << " " << p3.getX() - p2.getX() << " " << p3.getY() - p2.getY() << " " << p3.getZ() - p2.getZ() << endl;
    panelFile << p3.getX() << " " << p3.getY() << " " << p3.getZ() << " " << p4.getX() - p3.getX() << " " << p4.getY() - p3.getY() << " " << p4.getZ() - p3.getZ() << endl;
    panelFile << p4.getX() << " " << p4.getY() << " " << p4.getZ() << " " << p1.getX() - p4.getX() << " " << p1.getY() - p4.getY() << " " << p1.getZ() - p4.getZ() << endl;
    panelFile << endl;
}

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
Sun::Sun(float time) {
    this->time = time;
    calcDirection();
}
Vector Sun::getDirection() const {
    return direction;
}
float Sun::getTime() const {
    return time;
}
float Sun::getNormalAngle() const {
    // Realistic Path:
    return (180 / PI) * direction.getAngle(Vector(0, 0, 1));

    /*
    // Symmetric Path:
    return 15 * (time - NOON);
    */
}
float Sun::getHorizonAngle() const {
    return getNormalAngle() + 90;
}
void Sun::printPath(float inc, float height, ofstream& sunFile) const {
    for (float t = SUNRISE; t < SUNSET - inc; t += inc) {
        float temp = 3 * height;
        Sun currentS(t);
        Sun nextS(t + inc);
        Vector currentV = currentS.direction * temp;
        Vector nextV = nextS.direction * temp;

        sunFile << currentV.getX() << " " << currentV.getY() << " " << currentV.getZ() << " " <<
            nextV.getX() - currentV.getX() << " " << nextV.getY() - currentV.getY() << " " << nextV.getZ() - currentV.getZ() << endl;
    }
}

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
Collector::Collector(Point icenter, float length, float width, float height) {
    center = icenter;
    xMin = Plane(1, 0, 0, icenter.getX() - length / 2);
    xMax = Plane(1, 0, 0, icenter.getX() + length / 2);
    yMin = Plane(0, 1, 0, icenter.getY() - width / 2);
    yMax = Plane(0, 1, 0, icenter.getY() + width / 2);
    zMin = Plane(0, 0, 1, icenter.getZ() - height / 2);
    zMax = Plane(0, 0, 1, icenter.getZ() + height / 2);
    this->length = length;
    this->width = width;
    this->height = height;
}
Collector::Collector(float ixMin, float ixMax, float iyMin, float iyMax, float izMin, float izMax) {
    center = Point((float)0.5 * (ixMin + ixMax), (float)0.5 * (iyMin + iyMax), (float)0.5 * (izMin + izMax));
    xMin = Plane(1, 0, 0, ixMin);
    xMax = Plane(1, 0, 0, ixMax);
    yMin = Plane(0, 1, 0, iyMin);
    yMax = Plane(0, 1, 0, iyMax);
    zMin = Plane(0, 0, 1, izMin);
    zMax = Plane(0, 0, 1, izMax);
    length = ixMax - ixMin;
    width = iyMax - iyMin;
    height = izMax - izMin;
};
Plane& Collector::getMinX() {
    return xMin;
}
Plane& Collector::getMaxX() {
    return xMax;
}
Plane& Collector::getMinY() {
    return yMin;
}
Plane& Collector::getMaxY() {
    return yMax;
}
Plane& Collector::getMinZ() {
    return zMin;
}
Plane& Collector::getMaxZ() {
    return zMax;
}
Point& Collector::getCenter() {
    return center;
}
float Collector::calcTemperature(float power, float density, float mass, float specificHeat) {
    float volume = mass * density;
    //float volume = abs(xMax.getd() - xMin.getd()) * abs(yMax.getd() - yMin.getd()) * abs(zMax.getd() - zMin.getd());
    //float mass = volume * density; // kg
    // Q = mcT
    return power / mass / specificHeat; // (J/s) (kg K / J) (1 / kg) = K / s
}
float Collector::getLength() const {
    return length;
}
float Collector::getWidth() const {
    return width;
}
float Collector::getHeight() const {
    return height;
}
void Collector::printGnuplot(ofstream& collectorFile) const {
    collectorFile << xMax.getd() << " " << yMax.getd() << " " << zMax.getd() << endl; // 1
    collectorFile << xMax.getd() << " " << yMax.getd() << " " << zMin.getd() << endl; // 2
    collectorFile << xMin.getd() << " " << yMax.getd() << " " << zMin.getd() << endl; // 3
    collectorFile << xMin.getd() << " " << yMax.getd() << " " << zMax.getd() << endl; // 4
    collectorFile << xMax.getd() << " " << yMax.getd() << " " << zMax.getd() << endl; // 5
    collectorFile << xMax.getd() << " " << yMin.getd() << " " << zMax.getd() << endl; // 6
    collectorFile << xMax.getd() << " " << yMin.getd() << " " << zMin.getd() << endl; // 7
    collectorFile << xMax.getd() << " " << yMax.getd() << " " << zMin.getd() << endl; // 8
    collectorFile << xMax.getd() << " " << yMin.getd() << " " << zMin.getd() << endl; // 9
    collectorFile << xMin.getd() << " " << yMin.getd() << " " << zMin.getd() << endl; // 10
    collectorFile << xMin.getd() << " " << yMax.getd() << " " << zMin.getd() << endl; // 11
    collectorFile << xMin.getd() << " " << yMin.getd() << " " << zMin.getd() << endl; // 12
    collectorFile << xMin.getd() << " " << yMin.getd() << " " << zMax.getd() << endl; // 13
    collectorFile << xMax.getd() << " " << yMin.getd() << " " << zMax.getd() << endl; // 14
    collectorFile << xMin.getd() << " " << yMin.getd() << " " << zMax.getd() << endl; // 15
    collectorFile << xMin.getd() << " " << yMax.getd() << " " << zMax.getd() << endl; // 16
    collectorFile << endl;
}

ostream& operator<<(ostream& out, const Panel& panel) {
    out << "Position: " << "(" << panel.getX() << ", " << panel.getY() << ", " << panel.getZ() << ")" << endl;
    out << "Normal Vector: " << panel.getNormal() << endl;
    out << "Plane: " << panel.getPlane() << endl;
    return out;
}

#endif