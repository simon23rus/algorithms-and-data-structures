//
//  Primitives.cpp
//  RayTracing
//
//  Created by Simon Fedotov on 23.04.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include "Primitives.hpp"


Intersection Triangle::rayIntersection(const Ray &ray) {
    double b1 = ray.start.x - C.x;
    double b2 = ray.start.y - C.y;
    double b3 = ray.start.z - C.z;
    double determinant = -ray.direction.x * ((A.y - C.y) * (B.z - C.z)  - (B.y - C.y) * (A.z - C.z))
    + ray.direction.y * ((A.x - C.x) * (B.z - C.z)  - (B.x - C.x) * (A.z - C.z))
    - ray.direction.z * ((A.x - C.x) * (B.y - C.y)  - (B.x - C.x) * (A.y - C.y));
    double delta1 = b1 * ((A.y - C.y) * (B.z - C.z)  - (B.y - C.y) * (A.z - C.z))
    - b2 * ((A.x - C.x) * (B.z - C.z)  - (B.x - C.x) * (A.z - C.z))
    + b3 * ((A.x - C.x) * (B.y - C.y)  - (B.x - C.x) * (A.y - C.y));
    
    double delta2 = -ray.direction.x * (b2 * (B.z - C.z)  - (B.y - C.y) * b3)
    + ray.direction.y * (b1 * (B.z - C.z)  - (B.x - C.x) * b3)
    - ray.direction.z * (b1 * (B.y - C.y)  - (B.x - C.x) * b2);
    
    double delta3 = -ray.direction.x * ((A.y - C.y) * b3  - b2 * (A.z - C.z))
    + ray.direction.y * ((A.x - C.x) * b3  - b1 * (A.z - C.z))
    - ray.direction.z * ((A.x - C.x) * b2  - b1 * (A.y - C.y));
    double shift = delta1 / determinant;
    double lambda1 = delta2 / determinant;
    double lambda2  = delta3 / determinant;
    double lambda3 = 1 - lambda1 - lambda2;
    //Nashli resheniya methodom Kramera
    if(lambda1 >= 0 && lambda2 >= 0 && lambda3 >= 0) {
        //est' peresechenie
        //        std::cout << ray.start + ray.direction * shift;
        Point3D intersectionPoint = ray.start + ray.direction * shift;
        Point3D normalka = intersectionPoint + normal;
        double textureX = lambda1 * aTextureX + lambda2 * bTextureX + lambda3 * cTextureX;
        double textureY = lambda1 * aTextureY + lambda2 * bTextureY + lambda3 * cTextureY;
        return {true, shift, intersectionPoint, normalka, textureX, textureY};
    }
    else
        return {false};
    return {false};
}


//Intersection Quadrangle::rayIntersection(const Ray &ray) {
//    Triangle first = Triangle({color, A, B, C});
//    Triangle second = Triangle({color, D, A, C});
//    Intersection firstIntersection = first.rayIntersection(ray);
//    Intersection secondIntersection = second.rayIntersection(ray);
//    if(firstIntersection.hasIntersection)
//        return firstIntersection;
//    else
//        return secondIntersection;
//}


Intersection Sphere::rayIntersection(const Ray &ray) {
    //    double A = ray.direction.x * ray.direction.x
    //    + ray.direction.y * ray.direction.y
    //    + ray.direction.z * ray.direction.z;
    double A = scalar(ray.direction, ray.direction);
    //    double B = 2 * ((ray.start.x - center.x) * ray.direction.x
    //                    + (ray.start.y - center.y) * ray.direction.y
    //                    + (ray.start.z - center.z) * ray.direction.z);
    double B = 2 * scalar(ray.start - center, ray.direction);
    //    double C = (ray.start.x - center.x) * (ray.start.x - center.x)
    //    + (ray.start.y - center.y) * (ray.start.y - center.y)
    //    + (ray.start.z - center.z) * (ray.start.z - center.z)
    //    - radius * radius;
    
    double C = scalar(ray.start - center, ray.start - center) - radius * radius;
    double discriminant = B * B - 4 * A * C;
    double minRoot;
    if(discriminant < 0)
        return {false, 0, {}};
    else if(isClose(discriminant)) { //1 root
        minRoot = -B / (2 * A);
        //        std::cout << "Kasatelnaya " << minRoot << " " << ray.start + ray.direction * minRoot;
    }
    else {
        double root1 = (-B + sqrt(discriminant)) / (2 * A);
        double root2 = (-B - sqrt(discriminant)) / (2 * A);
        if(root2 >= 0 && root1 >= 0)
            minRoot = std::min(root1, root2);
        else if(root2 < 0)
            minRoot = root1;
        else
            minRoot = root2;
    }
    //calculate Normal vector
    Point3D intersectionPoint = ray.start + ray.direction * minRoot;
    Point3D normal = normalize(intersectionPoint, center);
    return {true, minRoot, intersectionPoint, normal};
}
