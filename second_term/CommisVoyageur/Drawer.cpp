//
//  Drawer.cpp
//  CommisVoyageur
//
//  Created by Simon Fedotov on 19.05.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "Drawer.h"
#include <math.h>
#include <random>
#include <iostream>

struct Edge{
    double DatDistance;
    int From;
    int To;
    
    bool operator<(const Edge& Second) const {
        return this -> DatDistance < Second.DatDistance;
    }
};

struct Point{
    double X;
    double Y;
};


std::default_random_engine JustRandom(std::random_device{}());
std::uniform_real_distribution<double> OurDoubles(0.01, 0.9999999999);

using namespace std;
vector<Point> points;
vector<std::pair<int, int>> tree;
vector<std::vector<std::pair<int, int>>> MinTree;
int DotsNumber;
/*! glut display callback function.  Every time the window needs to be drawn,
 glut will call this function.  This includes when the window size
 changes, or when another window covering part of this window is
 moved so this window is uncovered.
 */
void DrawPoints(double x1,double y1)
{
    glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA);
    glPointSize(7);
    glBegin(GL_POINTS);
    glVertex2f(x1, y1);
    glEnd();
}

void DrawEdges(int a, int b)
{
    glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA);
    glBegin(GL_LINES);
    glVertex2f(points[a].X+512, points[a].Y+512);
    glVertex2f(points[b].X+512, points[b].Y+512);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    /* clear the color buffer (resets everything to black) */
    /* set painting color to black */
    glColor3f(1, 0, 0);
    double Delta = 0.0;
    /* рисуем точки */
    //    for (int i = 0; i < 250; ++i)
    //        DrawPoints(2*i, 2*i)
    for (int i = 0; i < points.size(); ++i){
        if(i % DotsNumber == 0 && i != 0) {
            Delta += (double)(1.0 / (double)(points.size() / DotsNumber + 1));
            glColor3f(OurDoubles(JustRandom), OurDoubles(JustRandom), OurDoubles(JustRandom));

        }

        DrawPoints(points[i].X + 512, points[i].Y + 512);
    }
    
    glColor3f(0, 0, 1/2);

    for (int i = 0; i < tree.size(); ++i){
        DrawEdges(tree[i].first, tree[i].second);
    }
    
    /* swap the back and front buffers so we can see what we just drew */
    glutSwapBuffers();
}

/*! glut reshape callback function.  GLUT calls this function whenever
 the window is resized, including the first time it is created.
 You can use variables to keep track the current window size.
 */
void reshape(int width, int height)
{
    /* tell OpenGL we want to display in a recangle that is the
     same size as the window
     */
    glViewport(0,0,width,height);
    
    /* switch to the projection matrix */
    glMatrixMode(GL_PROJECTION);
    
    /* clear the projection matrix */
    glLoadIdentity();
    
    /* set the camera view, orthographic projection in 2D */
    gluOrtho2D(0,width,0,height);
    
    /* switch back to the model view matrix */
    glMatrixMode(GL_MODELVIEW);
}

void AnswerVisualizer(vector<Point> &data, vector<std::pair<int, int>> &edges, int &DotsNum)
{
    points = data;
    tree = edges;
    DotsNumber = DotsNum;
    int argc = 0;
    char** argv = new(char*);
    glutInit(&argc, argv);
    
    /* set the window size to 512 x 512 */
    glutInitWindowSize(2560, 1600);
    glutInitWindowPosition(300, 300);
    
    /* set the display mode to Red, Green, Blue and Alpha
     allocate a depth buffer
     enable double buffering
     */
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    /* create the window (and call it Lab 1) */
    glutCreateWindow("Result");
    
    /* set the glut display callback function
     this is the function GLUT will call every time
     the window needs to be drawn
     */
    glutDisplayFunc(display);
    
    /* set the glut reshape callback function
     this is the function GLUT will call whenever
     the window is resized, including when it is
     first created
     */
    glutReshapeFunc(reshape);
    
    /* set the default background color to black */
    glClearColor(1,1,1,1);
    
    /* enter the main event loop so that GLUT can process
     all of the window event messages
     */
    glutMainLoop();
}


void display2()
{
    glClear(GL_COLOR_BUFFER_BIT);
    /* clear the color buffer (resets everything to black) */
    /* set painting color to black */
    glColor3f(1, 0, 0);
    double Delta = 0.0;
    /* рисуем точки */
    //    for (int i = 0; i < 250; ++i)
    //        DrawPoints(2*i, 2*i)
    for (int i = 0; i < points.size(); ++i){
        if(i % DotsNumber == 0 && i != 0) {
            Delta += (double)(1.0 / (double)(points.size() / DotsNumber + 1));
            glColor3f(OurDoubles(JustRandom), OurDoubles(JustRandom), OurDoubles(JustRandom));
            
        }
        
        DrawPoints(points[i].X + 512, points[i].Y + 512);
    }
    
    glColor3f(0, 0, 1/2);
    
    for (int i = 0; i < MinTree.size(); ++i){
        for(int j = 0; j < MinTree[i].size(); ++j)
            DrawEdges(MinTree[i][j].first, MinTree[i][j].second);
    }
    
    /* swap the back and front buffers so we can see what we just drew */
    glutSwapBuffers();
}

void SpanningTreeVisualizer(vector<Point> &data, vector<std::vector<std::pair<int, int>>> &edges, int &DotsNum)
{
    points = data;
    MinTree = edges;
    DotsNumber = DotsNum;
    int argc = 0;
    char** argv = new(char*);
    glutInit(&argc, argv);
    
    /* set the window size to 512 x 512 */
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(300, 300);
    
    /* set the display mode to Red, Green, Blue and Alpha
     allocate a depth buffer
     enable double buffering
     */
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    /* create the window (and call it Lab 1) */
    glutCreateWindow("Result");
    
    /* set the glut display callback function
     this is the function GLUT will call every time
     the window needs to be drawn
     */
    glutDisplayFunc(display2);
    
    /* set the glut reshape callback function
     this is the function GLUT will call whenever
     the window is resized, including when it is
     first created
     */
    glutReshapeFunc(reshape);
    
    /* set the default background color to black */
    glClearColor(1,1,1,1);
    
    /* enter the main event loop so that GLUT can process
     all of the window event messages
     */
    glutMainLoop();
}





