//
//  Intersection.h
//  RayTracing
//
//  Created by Simon Fedotov on 23.04.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#ifndef Intersection_h
#define Intersection_h
#include "Point3D.hpp"

struct Intersection {
    bool hasIntersection;
    double shift;
    Point3D intersectionPoint;
    Point3D normal;
    double textureX;
    double textureY;
};


#endif /* Intersection_h */
