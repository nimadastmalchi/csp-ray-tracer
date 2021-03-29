#ifndef Position_h
#define Position_h

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

class Point {
private:
    float x, y, z;
public:
    Point();
    Point(float ix, float iy, float iz);
    void setX(const float& ix);
    void setY(const float& iy);
    void setZ(const float& iz);
    float getX() const;
    float getY() const;
    float getZ() const;
    Point operator/(float num) const;
    Point operator+(const Point& point) const;
};

class Vector {
private:
    float x, y, z;
public:
    Vector();
    Vector(float ix, float iy, float iz);
    float getX() const;
    float getY() const;
    float getZ() const;
    float getMag() const;
    float dot(Vector v) const;
    float getAngle(Vector v) const;

    Vector getUnit();

    Vector operator+(Vector vec);
    Vector operator*(float num);
    Vector operator/(float num);
};

class Plane {
private:
    // Equation: ax + by + cz = d
    float a, b, c, d;
public:
    Plane(float ia = 0, float ib = 0, float ic = 0, float id = 0);
    void createPlane(Vector normal, Point point);
    float geta() const;
    float getb() const;
    float getc() const;
    float getd() const;

    float getZ(float x, float y) const;
};

class Line {
private:
    // Equation: line = point + t * direction
    Vector direction;
    Vector point;
public:
    Line();
    Line(Point point1, Point point2);
    Vector& getPointVector();
    Vector& getDirectionVector();
    Vector getPointVector() const;
    Vector getDirectionVector() const;
    Point getPointOfLine(float t);
};

Point getIntersection(Line line, Plane plane);
Point midPoint(const Point& p1, const Point& p2);
float distance(const Point& p1, const Point& p2);
Vector getProjection(Vector a, Vector b);

ostream& operator<<(ostream& out, const Point& p);
ostream& operator<<(ostream& out, const Vector& vec);
ostream& operator<<(ostream& out, const Plane& plane);
ostream& operator<<(ostream& out, const Line& line);

#endif
