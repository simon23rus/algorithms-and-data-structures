//
//  Color.h
//  RayTracing
//
//  Created by Simon Fedotov on 23.04.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#ifndef Color_h
#define Color_h
#include <cmath>

struct Color {
    double r;
    double g;
    double b;
    double a;
    
    Color() :r(0), g(0), b(0), a(1){};

    
    Color(double red, double green, double blue, double ax = 1) : r(red), g(green), b(blue), a(ax) {
        if (red > 255)
            r = 255;
        else if(red < 0)
            r = 0;
        if(green > 255)
            g = 255;
        else if(green < 0)
            g = 0;
        if(blue > 255)
            b = 255;
        else if(blue < 0)
            b = 0;
    }
    
    Color operator +(const Color &other) const {
        return {static_cast<double>(r + other.r), static_cast<double>(g + other.g), static_cast<double>(b + other.b), fmax(a, other.a)};
    }
    Color operator *(double coeff) {
        return {r * coeff, g * coeff, b * coeff, a};
    }
};
const Color BLACK = {0, 0, 0, 1};
const Color RED = {255,0,0,1};


#endif /* Color_h */
