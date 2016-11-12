//
//  main.cpp
//  RayTracing
//
//  Created by Simon Fedotov on 21.04.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include "Point3D.hpp"
#include "Primitives.hpp"
#include "LightSource.h"

#include "Color.h"
#include "Ray.h"
#include "threadPool.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600
std::vector<double> ambient = {0.1,0.25,0.0,1.0};
std::vector<double> diffuse = {0.1,0.25,0.0,1.0};
std::vector<double> specular = {1.0,0.0,1.0,1.0};


thread_pool<void> pool(8);


class Object {
public:
    std::vector<Primitive*> parts;
    Object(std::vector<Primitive*> parts) : parts(parts){};
};





class Scene {
public:
    std::vector<Object*> objects;
    std::vector<LightSource> lighters;
    Scene(std::vector<Object*> &objects, std::vector<LightSource> &lighters) : objects(objects), lighters(lighters) {};
};




class Drawer {
public:
    Drawer(Point3D camera,
           double monitorWidth,
           double monitorHeight,
           Point3D leftTop,
           const std::vector<Object*> &objects,
           const std::vector<LightSource> &lighters, bool needTexture, bool needPool);
    SDL_Renderer *renderer;
    std::vector<std::vector<Color>> image;
    Point3D camera;
    double monitorWidth;
    double monitorHeight;
    bool needTexture;
    bool needPool;
    Point3D leftTop;
    std::vector<Object*> objects;
    std::vector<LightSource> lighters;
    void drawImage(int constantAntialiasing = 1);
    void calculateColorsInRow(int i, std::vector<std::vector<Color>> &pixels, std::vector<std::vector<Color>> &extendedImage);
    double lightingCalculation(Ray currentRay, Intersection intersection);
    std::vector<std::vector<Color>> antiAliasing(std::vector<std::vector<Color>> &image, int constant = 1);
};

Color meanColor(std::vector<Color> &colors) {
    double r = 0, g = 0, b = 0;
    for(auto color : colors) {
        r += color.r;
        g += color.g;
        b += color.b;
    }
    r /= colors.size();
    g /= colors.size();
    b /= colors.size();
    return {r, g,  b};
}


std::vector<std::vector<Color>> Drawer::antiAliasing(std::vector<std::vector<Color>> &extendedImage, int constant) {
    for (size_t i = 0; i < image.size(); i++) {
        for (size_t j = 0; j < image[i].size(); j++) {
//            if (i > 0u && i + 1u < image.size() && j > 0u && j + 1u < image[i].size() && image[i + 1].size() > 0u) {
                std::vector<Color> neighbours;
                for(int k = 0; k < constant; ++k) {
                    for (int l = 0; l < constant; ++l) {
                        neighbours.push_back(extendedImage[constant * i + k][constant * j + l]);
                    }
                }
                image[i][j] = meanColor(neighbours);
//            }
        }
    }
    return image;
}
    
double BACKGROUND_INTENSITY = 0.423;
double Drawer::lightingCalculation(Ray currentRay, Intersection intersection) {
    double intensity = BACKGROUND_INTENSITY;
    for(auto light : lighters) {
        bool lightHasIntersection = false;
        Ray rayToSource = {intersection.intersectionPoint, normalize(intersection.intersectionPoint - light.location)};
        double len = (intersection.intersectionPoint - light.location).length();
        double squaredLen = (intersection.intersectionPoint - light.location).squaredLength();
        for(auto object : objects) {
            if(lightHasIntersection)
                break;
            for(auto part : object -> parts) {
                Intersection lightIntersection = part -> rayIntersection(rayToSource);
                Point3D vecToCamera = currentRay.direction * (-1);
                Color calculatedColor = BLACK;
                if(lightIntersection.hasIntersection && fabs(lightIntersection.shift - len) <= 1e-10) {
//                    Point3D reflection = intersection.normal * scalar(intersection.normal, rayToSource.direction) * 2 - rayToSource.direction;
//                    intensity +=
                    lightHasIntersection = true;
                    break;
                }
            }
            if(!lightHasIntersection && intensity == BACKGROUND_INTENSITY)
                intensity = BACKGROUND_INTENSITY;
            if(!lightHasIntersection) {
//                double increase = light.getIncrease(intersection.intersectionPoint, intersection.normal);
                double cosNorm = (scalar(rayToSource.direction, intersection.normal));                //    std::cout << (intensity * cosNorm)/dist2 << std::endl;
                intensity += (light.intensity * cosNorm) / squaredLen;
//                std::cout << "int " << intensity << "\n";
            }
        }
        
    }
    return intensity;
}


SDL_Texture *t;


std::vector<std::vector<Color>> getpixel()
{
    std::vector<std::vector<Color>> answer;
    ///Users/semenfedotov/Desktop/stripes.png    /Users/semenfedotov/Desktop/texture.jpg
    SDL_Surface * surface = IMG_Load("/Users/semenfedotov/Desktop/stripes.png");
    answer.resize(surface->h);
    for(int i = 0; i < answer.size(); ++i) {
        answer[i].resize(surface->w);
    }
    int bpp = surface->format->BytesPerPixel;
    for(int i = 0; i < answer.size(); ++i) {
        for(int j = 0; j < answer[i].size(); ++j) {
            Uint8 *p = (Uint8 *)surface->pixels + i * surface->pitch + j * bpp;
            size_t index = i * surface->w + j;
            double b = ((unsigned char*)(surface->pixels))[index * bpp + 0] / 255.0f;
            double g = ((unsigned char*)(surface->pixels))[index * bpp + 1] / 255.0f;
            double r = ((unsigned char*)(surface->pixels))[index * bpp + 2] / 255.0f;
            answer[i][j] = {r,g,b,1};
            
        }
    }
    return answer;
}



Uint32* texturer() {
    // Init SDL
    SDL_Window *window;
    SDL_Renderer *renderator;
    SDL_CreateWindowAndRenderer(1200, 600, 0, &window, &renderator);
    SDL_Surface * img = IMG_Load("/Users/semenfedotov/Desktop/textura_girafa.png");
    std::cout << "SHIERna" << img->w <<"  " << img->h;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderator, img);
    void * pixels;
    SDL_LockTexture(t, &img->clip_rect, &pixels, &img->pitch);
    
    memcpy(pixels, img->pixels, img->w * img->h);
    
    Uint32 * upixels = (Uint32 *) pixels;
    
    // get or modify pixels
    
    SDL_UnlockTexture(t);
    return upixels;
}

Uint32 get_pixel_at(Uint32 * pixels, int x, int y, int w)
{
    return pixels[y * w + x];
}







Drawer::Drawer(Point3D camera,
               double monitorWidth,
               double monitorHeight,
               Point3D leftTop,
               const std::vector<Object*> &objects,
               const std::vector<LightSource> &lighters, bool needTexture, bool needPool) : camera(camera), monitorWidth(monitorWidth), monitorHeight(monitorHeight), leftTop(leftTop), objects(objects), lighters(lighters), needTexture(needTexture), needPool(needPool){
    image.resize(monitorWidth);
    for(int i = 0; i < image.size(); ++i) {
        image[i].resize(monitorHeight);
    }
}

unsigned createMask(unsigned a, unsigned b)
{
    unsigned r = 0;
    for (unsigned i=a; i<=b; i++)
        r |= 1 << i;
    
    return r;
}


void Drawer::calculateColorsInRow(int i, std::vector<std::vector<Color>> &pixels, std::vector<std::vector<Color>> &extendedImage) {
    for(int j = 0; j < monitorWidth; ++j) {
        Color pixelColor = BLACK;
        Ray currentRay = {camera, Point3D{leftTop.x + (j * (2 * fabs(leftTop.x))) / monitorWidth,leftTop.y - (i * (2 * fabs(leftTop.y))) / monitorHeight ,0} - camera};
        //            std::cout << "pixel coord = " << Point3D{leftTop.x + (j * (2 * fabs(leftTop.x))) / monitorWidth,leftTop.y - (i * (2 * fabs(leftTop.y))) / monitorHeight ,0};
        double minimalDistance = INT64_MAX;
        double intensity = 1;
        for(auto object : objects) {
            for(auto part : object -> parts) {
                Intersection intersection = part -> rayIntersection(currentRay);
                if(intersection.hasIntersection) {
                    if(intersection.shift < minimalDistance) {
                        minimalDistance = intersection.shift;
                        pixelColor = part -> getColor();
                        //                                                    std::cout << i << " " << j << "\n";
                        //                        std::cout << currentRay.start + currentRay.direction * (intersection.shift);
                        intensity = lightingCalculation(currentRay, intersection);
                        //                            Uint8* colors = (Uint8*)malloc(3 * sizeof(Uint8));
                        //                            memcpy(colors, 0, 3 * sizeof(Uint8));
                        
                        
                        
                        
                        //                            SDL_GetRGB(pixel, nullptr, &colors[0], &colors[1], &colors[2]);
                        //                            std::cout << i << "_" << j << "\n";
                        if(needTexture)
                            pixelColor = pixels[(int)intersection.textureX][(int)intersection.textureY];
                        //                            std::cout << red << " " << green << " " << blue << "\n";
                        
                    }
                }
            }
        }
        pixelColor = pixelColor * (intensity);
        extendedImage[j][i] = pixelColor;
        //            SDL_SetRenderDrawColor(renderer, pixelColor.r, pixelColor.g, pixelColor.b, pixelColor.a);
        //            SDL_RenderDrawPoint(renderer, j, i);
    }
}


void Drawer::drawImage(int constantAntialiasing) {
    SDL_Event event;
    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(monitorWidth, monitorHeight, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //        for (i = 0; i < WINDOW_WIDTH; ++i)
    //            SDL_RenderDrawPoint(renderer, i, i);

    monitorWidth *= constantAntialiasing;
    monitorHeight *= constantAntialiasing;
    std::vector<std::vector<Color>> extendedImage;
    extendedImage.resize(monitorWidth);
    for(int i = 0; i < extendedImage.size(); ++i) {
        extendedImage[i].resize(monitorHeight);
    }
    int imageWidth = 987;
    
    auto pixels = getpixel();
    if(needPool) {
        for(int i = 0; i < monitorHeight; ++i) {
            pool.submit(std::bind([this, i, &pixels, &extendedImage] () {
                for(int j = 0; j < monitorWidth; ++j) {
                    Color pixelColor = BLACK;
                    Ray currentRay = {camera, Point3D{leftTop.x + (j * (2 * fabs(leftTop.x))) / monitorWidth,leftTop.y - (i * (2 * fabs(leftTop.y))) / monitorHeight ,0} - camera};
                    //            std::cout << "pixel coord = " << Point3D{leftTop.x + (j * (2 * fabs(leftTop.x))) / monitorWidth,leftTop.y - (i * (2 * fabs(leftTop.y))) / monitorHeight ,0};
                    double minimalDistance = INT64_MAX;
                    double intensity = 1;
                    for(auto object : objects) {
                        for(auto part : object -> parts) {
                            Intersection intersection = part -> rayIntersection(currentRay);
                            if(intersection.hasIntersection) {
                                if(intersection.shift < minimalDistance) {
                                    minimalDistance = intersection.shift;
                                    pixelColor = part -> getColor();
                                    //                                                    std::cout << i << " " << j << "\n";
                                    //                        std::cout << currentRay.start + currentRay.direction * (intersection.shift);
                                    intensity = lightingCalculation(currentRay, intersection);
                                    //                            Uint8* colors = (Uint8*)malloc(3 * sizeof(Uint8));
                                    //                            memcpy(colors, 0, 3 * sizeof(Uint8));
                                    
                                    
                                    
                                    
                                    //                            SDL_GetRGB(pixel, nullptr, &colors[0], &colors[1], &colors[2]);
                                    //                            std::cout << i << "_" << j << "\n";
                                    if(needTexture)
                                        pixelColor = pixels[(int)intersection.textureX][(int)intersection.textureY];
                                    //                            std::cout << red << " " << green << " " << blue << "\n";
                                    
                                }
                            }
                        }
                    }
                    pixelColor = pixelColor * (intensity);
                    extendedImage[j][i] = pixelColor;
                    //            SDL_SetRenderDrawColor(renderer, pixelColor.r, pixelColor.g, pixelColor.b, pixelColor.a);
                    //            SDL_RenderDrawPoint(renderer, j, i);
                }
            
            }));
        }
    }
    else {
        for(int i = 0; i < monitorHeight; ++i) {
            for(int j = 0; j < monitorWidth; ++j) {
                Color pixelColor = BLACK;
                Ray currentRay = {camera, Point3D{leftTop.x + (j * (2 * fabs(leftTop.x))) / monitorWidth,leftTop.y - (i * (2 * fabs(leftTop.y))) / monitorHeight ,0} - camera};
                //            std::cout << "pixel coord = " << Point3D{leftTop.x + (j * (2 * fabs(leftTop.x))) / monitorWidth,leftTop.y - (i * (2 * fabs(leftTop.y))) / monitorHeight ,0};
                double minimalDistance = INT64_MAX;
                double intensity = 1;
                for(auto object : objects) {
                    for(auto part : object -> parts) {
                        Intersection intersection = part -> rayIntersection(currentRay);
                        if(intersection.hasIntersection) {
                            if(intersection.shift < minimalDistance) {
                                minimalDistance = intersection.shift;
                                pixelColor = part -> getColor();
    //                                                    std::cout << i << " " << j << "\n";
                                //                        std::cout << currentRay.start + currentRay.direction * (intersection.shift);
                                intensity = lightingCalculation(currentRay, intersection);
    //                            Uint8* colors = (Uint8*)malloc(3 * sizeof(Uint8));
    //                            memcpy(colors, 0, 3 * sizeof(Uint8));
                                
                                
                                
                                
    //                            SDL_GetRGB(pixel, nullptr, &colors[0], &colors[1], &colors[2]);
    //                            std::cout << i << "_" << j << "\n";
                                if(needTexture) {
                                    
                                    pixelColor = pixels[(int)intersection.textureX][(int)intersection.textureY];
                                }
    //                            std::cout << red << " " << green << " " << blue << "\n";
                                
                            }
                        }
                    }
                }
                pixelColor = pixelColor * (intensity);
                extendedImage[j][i] = pixelColor;
    //            SDL_SetRenderDrawColor(renderer, pixelColor.r, pixelColor.g, pixelColor.b, pixelColor.a);
    //            SDL_RenderDrawPoint(renderer, j, i);
            }
        }
    }
    
        monitorHeight /= constantAntialiasing;
        monitorWidth /= constantAntialiasing;
    
        antiAliasing(extendedImage, constantAntialiasing);
    
    for(int i = 0; i < monitorHeight; ++i) {
        for(int j = 0; j < monitorWidth; ++j) {
            SDL_SetRenderDrawColor(renderer, image[j][i].r, image[j][i].g, image[j][i].b, image[j][i].a);
            SDL_RenderDrawPoint(renderer, j, i);
        }
    }
    
    SDL_RenderPresent(renderer);
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
}







int main( int argc, char* args[] )
{
    srand(time(0));
    
    Sphere *mySphere = new Sphere({(double)(rand() % 256), (double)(rand() % 256) ,(double)(rand() % 256) ,1}, {0, 0 ,10}, 7);
//    Triangle *myTriangle = new Triangle({{(double)(rand() % 256), (double)(rand() % 256) ,(double)(rand() % 256) ,1}, {-30 ,-20, 0}, {30, 20, 0}, {70, -10, 0}});
//    Triangle *myTriangle2 = new Triangle({{(double)(rand() % 256), (double)(rand() % 256) ,(double)(rand() % 256) ,1}, {-40 ,-20, 2}, {40, 20, 2}, {70, -10, 2}});
//    Quadrangle *myQuadrangle = new Quadrangle({{(double)(rand() % 256), (double)(rand() % 256) ,(double)(rand() % 256) ,1}, {-0 ,0, 6}, {0, 50, 6}, {10, 30, 6}, {14,0,6}});
    Object *myObject = new Object({mySphere});
//    Object *otherObject = new Object({myTriangle});
//    Object *otherObject1 = new Object({myTriangle2});
//    Object *otherObject2 = new Object({myQuadrangle});
    LightSource light = {{15, 15, -4 }, 600};
//    LightSource light2 = {{-15, -20, -6 }, 600};
//    Drawer myDrawer = Drawer({0,0, -10}, 1200,600, {-8, 4, 0}, {myObject}, {light});

//        myDrawer.drawImage({myObject, otherObject, otherObject2});
//        myDrawer.drawImage({otherObject, otherObject1});
//    
    
//    myDrawer.drawImage();
    
//
////    myDrawer.drawImage({otherObject2});
//    
//        std::cout << "\n" <<  dynamic_cast<Sphere*>(myObject ->  parts[0]) -> center;
//    
    bool needPool = false;
    std::cin >> needPool;
    bool needTexture = false;
    std::cin >> needTexture;
    int constantAntialiasing;
    std::cin >> constantAntialiasing;
    
    std::vector<Object*> objects;
    std::vector<LightSource> lights;
    int n;
    std::cin >> n;
    for (int i = 0;i < n;++i) {
        int type;
        int color[3];
        std::cin >> type;
        for(int i = 0;i < 3;++i) {
            std::cin >> color[i];
        }
        if(!type) {
            Point3D v[3];
            for(int i = 0;i < 3;++i) {
                std::cin >> v[i].x >> v[i].y >> v[i].z;
            }
            
            Point3D normals[3] = {{1,1,1}, {1,1,1}, {1,1,1}};
//            for(int i = 0;i < 3;++i) {
//                std::cin >> normals[i].x >> normals[i].y >> normals[i].z;
//            }
            Point3D normalka = cross(v[1] - v[0], v[2] - v[0]);
            Triangle *myTriangle = new Triangle({{static_cast<double>(color[0]), static_cast<double>(color[1]), static_cast<double>(color[2]), 1},v[0],v[1],v[2], normalize(normalka), 0, 50, 80, 600, 200, 340, 600});
            objects.push_back(new Object({myTriangle}));
        } else {
            Point3D c;
            long double r;
            std::cin >> c.x >> c.y >> c.z >> r;
            objects.push_back(new Object({new Sphere({{static_cast<double>(color[0]), static_cast<double>(color[1]), static_cast<double>(color[2]), 1}, c, static_cast<double>(r)})}));
        }
    }

    int k;
    std::cin >> k;
    for(int i = 0;i < k;++i) {
        Point3D c;
        double intense;
        std::cin >> c.x >> c.y >> c.z >> intense;
        lights.push_back(LightSource(c,intense));
    }
    
    
    Drawer myDrawer = Drawer({0, 0, -10}, 1200,600, {-10, 5, 0}, objects, lights, needTexture, needPool);
    myDrawer.drawImage(constantAntialiasing);

    
    Point3D A = {-10, 10, 2};

    Point3D B = {12, 9, 8};

    Point3D C = {11, 16, 20};
  //    [-12;-23;241]
//    [-12;-23;241]
//    [-12;-23;241]
    /*
1
1
0
128 128 0
-10 10 2
12 9 8
11 16 20
54 270 -153
54 270 -153
54 270 -153
1
20 30 -2 120
     
     
     1
     255 0 0
     1
     0 0 10 3
     1
     20 30 -2 120
     
     
1
1
0
255 0 0
-5 5 2
0 5 1
0 10 2
-1 -1 -1
-1 -1 -1
-1 -1 -1
1
0 0 -2 120

     
     */
    return 0;
}