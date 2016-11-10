//
//  main.cpp
//  AVL_New
//
//  Created by Simon Fedotov on 10.12.14.
//  Copyright (c) 2014 Simon23Rus. All rights reserved.
//

#include <stdio.h>
#include "AVLka.h"
#include <queue>

class Point
{
    friend std::istream & operator>> (std::istream& in, Point& x);
    friend std::ostream & operator>> (std::ostream& out, Point& x);

private: double x; double y;
public: Point(){    }
    ~Point(){ }
    Point (double &a, double & b) : x(a), y(b) {}
};



std::istream & operator>> (std::istream& in, Point& x)
{
    in>>x.x>>x.y; return in;
}
std::ostream & operator>> (std::ostream& out, Point& x)
{
    out<<x.x<<";"<<x.y<<"\n";
    return out;
}

int main() {
    size_t n;
    std::cin>>n; Point y; std::queue<Node<Point>*> Kuku;
    Node<Point>* x=0;
    for(size_t i=0; i<n; ++i)
    {
        std::cin>>y; InsertNode(x, y, Kuku);
    }
    return 0;
}