#ifndef Ray_h
#define Ray_h

#include <iostream>
#include <limits.h>
#include <vector>
#include "Components.h" // Also gets Position.h

using namespace std;

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

void printRays(vector<Ray>& missPanel, vector<Ray>& hitPanel, vector<Ray>& missCollector, vector<Ray>& hitCollector);

// Array of Panels
vector<Ray> generateRays(int n, Point min, Point max, Sun sun, Collector collector, vector<Panel> panels, vector<Ray>& hitPanel, vector<Ray>& missPanel, vector<Ray>& hitCollector, vector<Ray>& missCollector, float collectorHeight);

#endif