#ifndef Position_cpp
#define Position_cpp

#include "Position.h"

//** Point Class **
Point::Point() {
    x = y = z = 0;
}
Point::Point(float ix, float iy, float iz) {
    x = ix;
    y = iy;
    z = iz;
}
void Point::setX(const float& ix) {
    x = ix;
}
void Point::setY(const float& iy) {
    y = iy;
}
void Point::setZ(const float& iz) {
    z = iz;
}
float Point::getX() const {
    return x;
}
float Point::getY() const {
    return y;
}
float Point::getZ() const {
    return z;
}
Point Point::operator/(float num) const {
    return Point(x / num, y / num, z / num);
}
Point Point::operator+(const Point& point) const {
    return Point(x + point.x, y + point.y, z + point.z);
}

//** Vector Class **
Vector::Vector() {
    x = y = z = 0;
}
Vector::Vector(float ix, float iy, float iz) {
    x = ix;
    y = iy;
    z = iz;
}
float Vector::getX() const {
    return x;
}
float Vector::getY() const {
    return y;
}
float Vector::getZ() const {
    return z;
}
float Vector::getMag() const {
    return (float)pow(x * x + y * y + z * z, 0.5);
}
float Vector::dot(Vector v) const {
    return x * v.x + y * v.y + z * v.z;
}
float Vector::getAngle(Vector v) const {
    float x = dot(v) / (getMag() * v.getMag());
    if (x > 1)
        return 0;
    return acos(x);
}
Vector Vector::getUnit() {
    Vector unit = *this / getMag();
    return unit;
}
Vector Vector::operator+(Vector vec) {
    float rx, ry, rz; // Coordinates of result
    rx = x + vec.x;
    ry = y + vec.y;
    rz = z + vec.z;
    return Vector(rx, ry, rz);
}
Vector Vector::operator*(float num) {
    return Vector(x * num, y * num, z * num);
}
Vector Vector::operator/(float num) {
    return Vector(x / num, y / num, z / num);
}

// ** Plane Class **
Plane::Plane(float ia, float ib, float ic, float id)
    :a(ia), b(ib), c(ic), d(id) {}
void Plane::createPlane(Vector normal, Point point) {
    a = normal.getX();
    b = normal.getY();
    c = normal.getZ();
    d = a * point.getX() + b * point.getY() + c * point.getZ();
}
float Plane::geta() const {
    return a;
}
float Plane::getb() const {
    return b;
}
float Plane::getc() const {
    return c;
}
float Plane::getd() const {
    return d;
}
float Plane::getZ(float x, float y) const {
    return (d - a * x - b * y) / c;
}

// ** Line Class **
Line::Line() {}
Line::Line(Point point1, Point point2) {
    direction = Vector(point1.getX() - point2.getX(), point1.getY() - point2.getY(), point1.getZ() - point2.getZ());
    point = Vector(point1.getX(), point1.getY(), point1.getZ());
}
Vector& Line::getPointVector() {
    return point;
}
Vector& Line::getDirectionVector() {
    return direction;
}
Vector Line::getPointVector() const {
    return point;
}
Vector Line::getDirectionVector() const {
    return direction;
}
Point Line::getPointOfLine(float t) {
    Vector r = point + direction * t;
    return Point(r.getX(), r.getY(), r.getZ());
}

// ** Other functions **
Point getIntersection(Line line, Plane plane) {
    // Line:
    // point = <px, py, pz>
    // direction = <dx, dy, dz>
    // r = <px + t(dx), py + t(dy), pz + t(dz)>
    // Plane: ax + by + cz = d
    // Combining the two would get the equation:
    // t = (d - a(px) - b(py) - c(pz)) / (a(dx) + b(dy) + c(pz))
    float t = (plane.getd() - plane.geta() * line.getPointVector().getX() - plane.getb() * line.getPointVector().getY() - plane.getc() * line.getPointVector().getZ())
        / (plane.geta() * line.getDirectionVector().getX() + plane.getb() * line.getDirectionVector().getY() + plane.getc() * line.getDirectionVector().getZ());
    // The point of intersection is at t in the equations: x = px + t(dx), y = py + t(dy), z = pz + t(dz)
    return Point(line.getPointVector().getX() + t * line.getDirectionVector().getX(),
        line.getPointVector().getY() + t * line.getDirectionVector().getY(),
        line.getPointVector().getZ() + t * line.getDirectionVector().getZ());
}

Point midPoint(const Point& p1, const Point& p2) {
    return (p1 + p2) / 2;
}

float distance(const Point& p1, const Point& p2) {
    return (float)pow(pow(p1.getX() - p2.getX(), 2.0) + pow(p1.getY() - p2.getY(), 2.0) + pow(p1.getZ() - p2.getZ(), 2.0), 0.5);
}

Vector getProjection(Vector a, Vector b) { // Projection of a onto b
    return b * (a.dot(b) / b.dot(b));
}

ostream& operator<<(ostream& out, const Point& p) {
    out << "(" << p.getX() << ", " << p.getY() << ", " << p.getZ() << ")";
    return out;
}
ostream& operator<<(ostream& out, const Vector& vec) {
    out << "<" << vec.getX() << ", " << vec.getY() << ", " << vec.getZ() << ">";
    return out;
}
ostream& operator<<(ostream& out, const Plane& plane) {
    out << plane.geta() << "x + " << plane.getb() << "y + " << plane.getc() << "z = " << plane.getd();
    return out;
}
ostream& operator<<(ostream& out, const Line& line) {
    out << "<" << line.getPointVector().getX() << " + " << line.getDirectionVector().getX() << "t" << ", " <<
        line.getPointVector().getY() << " + " << line.getDirectionVector().getY() << "t" << ", " <<
        line.getPointVector().getZ() << " + " << line.getDirectionVector().getZ() << "t" << ">";
    return out;
}

#endif