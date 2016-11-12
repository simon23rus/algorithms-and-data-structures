//
//  BoundingBox.hpp
//  RayTracing
//
//  Created by Simon Fedotov on 10.05.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#ifndef BoundingBox_hpp
#define BoundingBox_hpp

#include <stdio.h>

#include "Point3D.hpp"
#include <cmath>
#include <algorithm>

class BoundingBox {
public:
    double xUp, xDown;
    double yUp, yDown;
    double zUp, zDown;
    
    BoundingBox() {};
    BoundingBox(Point3D first, Point3D second);
    
    BoundingBox(BoundingBox first, BoundingBox second);
    
    Point3D getUpPoint();
    Point3D getDownPoint();
    
    BoundingBox(double xUp, double xDown,
                double yUp, double yDown,
                double zUp, double zDown) :
    xUp(xUp), xDown(xDown), yUp(yUp), yDown(yDown), zUp(zUp), zDown(zDown) {};
};



#endif /* BoundingBox_hpp */
