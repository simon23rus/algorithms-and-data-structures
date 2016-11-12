//
//  Primitives.hpp
//  RayTracing
//
//  Created by Simon Fedotov on 23.04.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#ifndef Primitives_hpp
#define Primitives_hpp

#include <stdio.h>
#include "Color.h"
#include "Intersection.h"
#include "Point3D.hpp"
#include "Ray.h"

class Primitive {
public:
    virtual double getReflection() const = 0; //reflection variates in [0 , 100]
    virtual Color getColor() const = 0;
    virtual Intersection rayIntersection(const Ray &ray) = 0;
};


class Triangle : public Primitive {
public:
    Color color;
    Point3D A;
    Point3D B;
    Point3D C;
    Point3D normal;
    double reflection = 0;
    double aTextureX = -1;
    double aTextureY = -1;
    double bTextureX = -1;
    double bTextureY = -1;
    double cTextureX = -1;
    double cTextureY = -1;
    double getReflection() const override {return reflection;};
    Color getColor() const override {return color;};
    Intersection rayIntersection(const Ray &ray) override;
    Triangle(Color color, Point3D A, Point3D B, Point3D C, Point3D normal = {1,1,1}, double reflection = 0,
             double aTextureX = -1,
             double aTextureY = -1,
             double bTextureX = -1,
             double bTextureY = -1,
             double cTextureX = -1,
             double cTextureY = -1
    )  : color(color), A(A), B(B), C(C), normal(normal), reflection(reflection), aTextureX(aTextureX), aTextureY(aTextureY), bTextureX(bTextureX), bTextureY(bTextureY), cTextureX(cTextureX), cTextureY(cTextureY) {};
};



//еще когда невыпуклый
class Quadrangle : public Primitive {
public:
    Color color;
    Point3D A;
    Point3D B;
    Point3D C;
    Point3D D;
    Color getColor() const override { return color ;};
    Intersection rayIntersection(const Ray &ray) override;
    Quadrangle(Color color, Point3D A, Point3D B , Point3D C , Point3D D) : color(color), A(A), B(B), C(C), D(D) {};
};



class Sphere : public Primitive {
public:
    Color color;
    Point3D center;
    double radius;
    double reflection = 0;
    Color getColor() const override { return color ;};
    double getReflection() const override {return reflection;};
    Intersection rayIntersection(const Ray &ray) override;
    Sphere(Color color, Point3D center, double radius, double reflection = 0) : color(color), center(center), radius(radius), reflection(reflection) {};
};

template <typename T>
bool isClose(const T &first, const T &second = 0) {
    return fabs(first - second) <= 1e-9;
}

class Box {
    Color color;
    Point3D A;
    Point3D B;
    Point3D C;
    Point3D D;
};

#endif /* Primitives_hpp */
