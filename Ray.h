#ifndef Ray_h
#define Ray_h

#include <iostream>
#include <limits.h>
#include "Components.h" // Also gets Position.h

using namespace std;

float height;

class Ray {
private:
    Line line; // Equation of the line representing the specific ray
    Vector vector; // Each ray is also represented by a vector

    bool reflected;
    bool collectored;
    bool panelled;

    // Point start;
    Point sunPoint;
    Point panelPoint;
    Point collectorPoint;
public:
    Ray();
    Ray(Point center, Point point);
    Ray(Vector ivector, Line iline);
    Line& getLine();
    Vector& getVector();

    bool getReflected() const;
    bool getCollectored() const;
    bool getPanelled() const;

    Point getPanelPoint() const;
    Point getCollectorPoint() const;

    bool hitsPanel(Panel panel);
    bool reflect(Panel panel, Ray& reflectedRay);
    bool hitsCollector(Collector collector);
    void printGnuplot(ofstream& file);
};
Ray::Ray() { reflected = false; panelled = false; collectored = false; }
Ray::Ray(Point center, Point point) {
    reflected = false;
    collectored = false;
    panelled = false;
    sunPoint = center;
    line = Line(center, point);
    vector = Vector(point.getX() - center.getX(), point.getY() - center.getY(), point.getZ() - center.getZ());
}
Ray::Ray(Vector ivector, Line iline) {
    vector = ivector;
    line = iline;

    reflected = false;
    collectored = false;
    panelled = false;
}
Line& Ray::getLine() {
    return line;
}
Vector& Ray::getVector() {
    return vector;
}

bool Ray::getReflected() const {
    return reflected;
}
bool Ray::getCollectored() const {
    return collectored;
}
bool Ray::getPanelled() const {
    return panelled;
}

Point Ray::getPanelPoint() const {
    return panelPoint;
}
Point Ray::getCollectorPoint() const {
    return collectorPoint;
}

bool Ray::hitsPanel(Panel panel) {
    Point intersection = getIntersection(line, panel.getPlane());
    if (intersection.getX() <= panel.getMaxX() && intersection.getX() >= panel.getMinX() && intersection.getY() <= panel.getMaxY() && intersection.getY() >= panel.getMinY()) {
        if (panel.getNormal().dot(vector) < 0) { // If n.v < 0, then the two vectors are pointing to each other
            panelPoint = intersection;
            panelled = true;
            return true;
        }
    }
    return false;
}
// Creates new reflected ray after the ray hits the panel 
bool Ray::reflect(Panel panel, Ray& reflectedRay) {
    if (!hitsPanel(panel)) {
        return false;
    }
    reflectedRay = *this;
    reflectedRay.reflected = true;
    Point intersection = getIntersection(line, panel.getPlane());

    // Reflected vector V = V - 2 * ( Proj of V onto n ) = V - 2 * ( V.n ) / ( n.n ) * n .... V is the vector of the ray coming from the sun, n is normal to the panel
    reflectedRay.vector = vector + getProjection(vector, panel.getNormal()) * -2;
    // Two points of the line: the point of intersection with panel, and the vector added to the point of intersection
    reflectedRay.line = Line(Point(reflectedRay.vector.getX() + intersection.getX(), reflectedRay.vector.getY() + intersection.getY(), reflectedRay.vector.getZ() + intersection.getZ()), intersection);
    return true;
}
bool Ray::hitsCollector(Collector collector) {
    float minX = collector.getMinX().getd();
    float maxX = collector.getMaxX().getd();
    float minY = collector.getMinY().getd();
    float maxY = collector.getMaxY().getd();
    float minZ = collector.getMinZ().getd();
    float maxZ = collector.getMaxZ().getd();

    bool hitArr[6] = { false, false, false, false, false, false };
    Point intersections[6];

    Point planeMinX = getIntersection(line, collector.getMinX());
    intersections[0] = planeMinX;
    if (planeMinX.getY() <= maxY && planeMinX.getY() >= minY && planeMinX.getZ() <= maxZ && planeMinX.getZ() >= minZ) {
        hitArr[0] = true;
    }
    Point planeMaxX = getIntersection(line, collector.getMaxX());
    intersections[1] = planeMaxX;
    if (planeMaxX.getY() <= maxY && planeMaxX.getY() >= minY && planeMaxX.getZ() <= maxZ && planeMaxX.getZ() >= minZ) {
        hitArr[1] = true;
    }
    Point planeMinY = getIntersection(line, collector.getMinY());
    intersections[2] = planeMinY;
    if (planeMinY.getX() <= maxX && planeMinY.getX() >= minX && planeMinY.getZ() <= maxZ && planeMinY.getZ() >= minZ) {
        hitArr[2] = true;
    }
    Point planeMaxY = getIntersection(line, collector.getMaxY());
    intersections[3] = planeMaxY;
    if (planeMaxY.getX() <= maxX && planeMaxY.getX() >= minX && planeMaxY.getZ() <= maxZ && planeMaxY.getZ() >= minZ) {
        hitArr[3] = true;
    }
    Point planeMinZ = getIntersection(line, collector.getMinZ());
    intersections[4] = planeMinZ;
    if (planeMinZ.getX() <= maxX && planeMinZ.getX() >= minX && planeMinZ.getY() <= maxY && planeMinZ.getY() >= minY) {
        hitArr[4] = true;
    }
    Point planeMaxZ = getIntersection(line, collector.getMaxZ());
    intersections[5] = planeMaxZ;
    if (planeMaxZ.getX() <= maxX && planeMaxZ.getX() >= minX && planeMaxZ.getY() <= maxY && planeMaxZ.getY() >= minY) {
        hitArr[5] = true;
    }

    float minDistance = (float)INT_MAX;
    int minIndex = 0;

    for (int i = 0; i < 6; i++) {
        if (hitArr[i]) {
            float currentDistance = distance(Point(panelPoint.getX(), panelPoint.getY(), panelPoint.getZ()), intersections[i]);
            if (currentDistance < minDistance) {
                minDistance = currentDistance;
                minIndex = i;
            }
            collectored = true;
        }
    }

    collectorPoint = intersections[minIndex];

    return collectored;
}
void Ray::printGnuplot(ofstream& file) {
    float distance = 1.5 * height;
    if (!panelled) {
        file << sunPoint.getX() << " " << sunPoint.getY() << " " << sunPoint.getZ() << " " << distance * vector.getX() << " " << distance * vector.getY() << " " << distance * vector.getZ() << endl;
    }
    else if (reflected) {
        if (collectored) {
            file << panelPoint.getX() << " " << panelPoint.getY() << " " << panelPoint.getZ() << " " << collectorPoint.getX() - panelPoint.getX() << " " << collectorPoint.getY() - panelPoint.getY() << " " << collectorPoint.getZ() - panelPoint.getZ() << endl;
        }
        else {
            file << panelPoint.getX() << " " << panelPoint.getY() << " " << panelPoint.getZ() << " " << distance * vector.getX() << " " << distance * vector.getY() << " " << distance * vector.getZ() << endl;
        }
    }
    else { // Hits Panel
        file << sunPoint.getX() << " " << sunPoint.getY() << " " << sunPoint.getZ() << " " << panelPoint.getX() - sunPoint.getX() << " " << panelPoint.getY() - sunPoint.getY() << " " << panelPoint.getZ() - sunPoint.getZ() << endl;
    }
}

// New method
void printRays(vector<Ray>& missPanel, vector<Ray>& hitPanel, vector<Ray>& missCollector, vector<Ray>& hitCollector) {
    vector<Ray>::iterator rayIdx;

    ofstream missPanelFile("~miss_panel.txt");
    int increment = 0;
    for (rayIdx = missPanel.begin(); rayIdx != missPanel.end(); rayIdx++) {
        if (increment++ % 1 == 0) {
            rayIdx->printGnuplot(missPanelFile);
        }
    }

    ofstream hitPanelFile("~hit_panel.txt");
    for (rayIdx = hitPanel.begin(); rayIdx != hitPanel.end(); rayIdx++) {
        rayIdx->printGnuplot(hitPanelFile);
    }

    ofstream missCollectorFile("~miss_collector.txt");
    for (rayIdx = missCollector.begin(); rayIdx != missCollector.end(); rayIdx++) {
        rayIdx->printGnuplot(missCollectorFile);
    }

    ofstream hitCollectorFile("~hit_collector.txt");
    for (rayIdx = hitCollector.begin(); rayIdx != hitCollector.end(); rayIdx++) {
        rayIdx->printGnuplot(hitCollectorFile);
    }
}

// Array of Panels
vector<Ray> generateRays(int n, Point min, Point max, Sun sun, Collector collector, vector<Panel> panels, vector<Ray>& hitPanel, vector<Ray>& missPanel, vector<Ray>& hitCollector, vector<Ray>& missCollector, float collectorHeight) {
    hitPanel.clear();
    missPanel.clear();
    hitCollector.clear();
    missCollector.clear();

    Vector sunVec = sun.getDirection() * -1;
    height = (sun.getTime() - NOON) > 0 ? (21 - sun.getTime()) / 3 * collectorHeight : (sun.getTime() - 3) / 3 * collectorHeight;

    vector<Ray> allRays;
    vector<Ray> reflectedRays;
    vector<Ray>::iterator rayIdx;
    vector<Panel>::iterator panelIdx;

    float xh = abs(max.getX() - min.getX()) / n;
    float yh = abs(max.getY() - min.getY()) / n;

    //
    // Determining the bounds:
    Vector sunUnit = (sunVec * -1) / (sunVec * -1).getMag();

    Plane heightPlane(0, 0, 1, height);
    Point minIntersection = getIntersection(Line(min, Point(sunUnit.getX() + min.getX(), sunUnit.getY() + min.getY(), sunUnit.getZ() + min.getZ())), heightPlane);
    Point maxIntersection = getIntersection(Line(max, Point(sunUnit.getX() + max.getX(), sunUnit.getY() + max.getY(), sunUnit.getZ() + max.getZ())), heightPlane);
    //
    //

    // Generating all Rays
    for (float i = minIntersection.getX() - 4 * panels[0].getLength(); i <= maxIntersection.getX() + 4 * panels[0].getLength(); i += xh) {
        for (float j = minIntersection.getY() - 4 * panels[0].getLength(); j <= maxIntersection.getY() + 4 * panels[0].getLength(); j += yh) {
            Point center(i, j, height);
            Point point(center.getX() + sunVec.getX(), center.getY() + sunVec.getY(), center.getZ() + sunVec.getZ());
            Ray oneRay(center, point);
            allRays.push_back(oneRay);
        }
    }

    // Storing hitPanel and missPanel
    for (rayIdx = allRays.begin(); rayIdx != allRays.end(); rayIdx++) {
        bool hitsAnyPanel = false;
        for (panelIdx = panels.begin(); panelIdx != panels.end(); panelIdx++) {
            if (!rayIdx->getPanelled() && rayIdx->hitsPanel(*panelIdx)) {
                hitPanel.push_back(*rayIdx);
                hitsAnyPanel = true;
                Ray reflectedRay;
                rayIdx->reflect(*panelIdx, reflectedRay);
                reflectedRays.push_back(reflectedRay);
            }
        }
        if (!hitsAnyPanel) {
            missPanel.push_back(*rayIdx);
        }
    }

    // Storing hitCollector and missCollector
    for (rayIdx = reflectedRays.begin(); rayIdx != reflectedRays.end(); rayIdx++) {
        if (rayIdx->hitsCollector(collector)) {
            hitCollector.push_back(*rayIdx);
        }
        else {
            missCollector.push_back(*rayIdx);
        }
    }

    return allRays;
}

#endif