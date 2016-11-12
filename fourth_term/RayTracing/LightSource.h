//
//  LightSource.h
//  RayTracing
//
//  Created by Simon Fedotov on 23.04.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#ifndef LightSource_h
#define LightSource_h

class LightSource {
public:
    Point3D location;
    double intensity;
    LightSource(Point3D location, double intensity) : location(location), intensity(intensity) {};
    double getIncrease(Point3D &point, Point3D &normal) const {
        return (intensity / pow((location - point).length(), 3.)
                * (scalar(normal, (location - point))));
    }
};


#endif /* LightSource_h */
