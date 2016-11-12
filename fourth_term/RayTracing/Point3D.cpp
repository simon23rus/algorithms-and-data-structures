//
//  Point3D.cpp
//  RayTracing
//
//  Created by Simon Fedotov on 23.04.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include "Point3D.hpp"


long double Point3D::length() const {
    return sqrt(x * x + y * y + z * z);
}

long double Point3D::squaredLength() const {
    return x * x + y * y + z * z;
}

Point3D Point3D::operator -(const Point3D &other) const {
    return {x - other.x, y - other.y, z - other.z};
}
Point3D Point3D::operator +(const Point3D &other)  const {
    return {x + other.x, y + other.y, z + other.z};
}

bool Point3D::operator ==(const Point3D &other) {
    return (x == other.x) && (y == other.y) && (z == other.z);
}

bool Point3D::operator <(const Point3D &other) const {
    return (x < other.x) || (x == other.x && y < other.y) || (x == other.x && y == other.y && z < other.z);
}

Point3D Point3D::operator *(const double &coeff) const {
    return {coeff * x, coeff * y, coeff * z};
}


Point3D operator * (const double &coeff, const Point3D& ourpoint) {
    return {coeff * ourpoint.x, coeff * ourpoint.y, coeff * ourpoint.z};
}


std::ostream & operator <<(std::ostream & out, const Point3D & elem) {
    out <<  "[" << elem.x << ";" << elem.y  << ";" << elem.z << "]\n";
    return out;
}

Point3D normalize(const Point3D &first, const Point3D &second) {
    Point3D res = {second.x - first.x, second.y - first.y, second.z - first.z};
    return res * (1 / res.length());
}

Point3D normalize(const Point3D &res) {
    return res * (1 / res.length());
}

Point3D cross(Point3D first, Point3D second) {
    return Point3D{first.y * second.z - first.z * second.y,first.z * second.x - first.x * second.z,first.x * second.y - first.y * second.x};
}


long double scalar(Point3D first, Point3D second) {
    return first.x * second.x + first.y * second.y + first.z * second.z;
}


//smeshannoe
double determinant(Point3D A, Point3D B, Point3D C) {
    return
    + A.x * (B.y * C.z - B.z * C.y)
    - B.x * (A.y * C.z - A.z * C.y)
    + C.x * (A.y * B.z - A.z * B.y);
}


