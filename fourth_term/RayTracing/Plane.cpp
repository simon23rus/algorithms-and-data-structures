//
//  Plane.cpp
//  RayTracing
//
//  Created by Simon Fedotov on 10.05.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include "Plane.hpp"

Plane::Plane(Point3D first, Point3D second, Point3D third) {
    normal = normalize(cross( (first - second),(third - second) ));
    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = (-1.) * (second.x * a + second.y * b + second.z * c);
}

Plane::Plane(Point3D p, Point3D n) {
    normal = n;
    a = n.x;
    b = n.y;
    c = n.z;
    d = (-1.) * (p.x * a + p.y * b + p.z * c);
}

double Plane::putPoint(Point3D point) {
    return a * point.x + b * point.y + c * point.z + d;
}