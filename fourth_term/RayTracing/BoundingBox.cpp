//
//  BoundingBox.cpp
//  RayTracing
//
//  Created by Simon Fedotov on 10.05.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include "BoundingBox.hpp"

Point3D BoundingBox::getUpPoint() {
    return {xUp, yUp, zUp};
}


Point3D BoundingBox::getDownPoint() {
    return {xDown, yDown, zDown};
}

BoundingBox::BoundingBox(Point3D first, Point3D second) {
    xUp = std::max(first.x, second.x);
    xDown = std::min(first.x, second.x);
    
    yUp = std::max(first.y, second.y);
    yDown = std::min(first.y, second.y);
    
    zUp = std::max(first.z, second.z);
    zDown = std::min(first.z, second.z);
}

BoundingBox::BoundingBox(BoundingBox first, BoundingBox second) {
    xUp = std::max(first.getUpPoint().x, second.getUpPoint().x);
    xDown = std::min(first.getDownPoint().x, second.getDownPoint().x);
    
    yUp = std::max(first.getUpPoint().y, second.getUpPoint().y);
    yDown = std::min(first.getDownPoint().y, second.getDownPoint().y);
    
    zUp = std::max(first.getUpPoint().z, second.getUpPoint().z);
    zDown = std::min(first.getDownPoint().z, second.getDownPoint().z);
}
