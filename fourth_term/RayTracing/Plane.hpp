//
//  Plane.hpp
//  RayTracing
//
//  Created by Simon Fedotov on 10.05.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#ifndef Plane_hpp
#define Plane_hpp

#include <stdio.h>
#include "Point3D.hpp"


class Plane {
public:
    double a, b, c, d;
    Point3D normal;
    
    Plane() {};
    Plane(double a, double b, double c, double d) :
    a(a), b(b), c(c), d(d), normal({a,b,c}){};
    
    
    //create a Plane by 3 points in normal equation Ax + By + Cz + D = 0
    //D has found by put into equation one of the dots
    Plane(Point3D first, Point3D second, Point3D third);
    
    
    //create a Plane by normal vector and point on the plane
    Plane(Point3D p, Point3D n);
    
    double putPoint(Point3D point);
};


#endif /* Plane_hpp */
