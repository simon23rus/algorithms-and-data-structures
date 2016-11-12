//
//  Point3D.hpp
//  RayTracing
//
//  Created by Simon Fedotov on 23.04.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#ifndef Point3D_hpp
#define Point3D_hpp
#include <iostream>
#include <cmath>
const int BAD_COORD = INT32_MIN;

struct Point3D {
    double x;
    double y;
    double z;
    
    long double length() const;
    
    long double squaredLength() const;
    
    friend std::ostream & operator <<(std::ostream & out, const Point3D &elem);
    
    Point3D operator -(const Point3D &other) const;
    Point3D operator +(const Point3D &other)  const;
    
    bool operator ==(const Point3D &other);
    
    bool operator <(const Point3D &other) const;
    Point3D operator *(const double &coeff) const;
    
};

const Point3D BAD_POINT = {BAD_COORD,BAD_COORD,BAD_COORD};
Point3D operator * (const double &coeff, const Point3D& ourpoint);
std::ostream & operator <<(std::ostream & out, const Point3D & elem);
Point3D normalize(const Point3D &first, const Point3D &second);
Point3D normalize(const Point3D &res);
Point3D cross(Point3D first, Point3D second);
long double scalar(Point3D first, Point3D second);

//smeshannoe
double determinant(Point3D A, Point3D B, Point3D C);

#endif /* Point3D_hpp */
