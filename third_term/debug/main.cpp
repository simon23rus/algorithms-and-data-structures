//
//  main.cpp
//  debug
//
//  Created by Simon Fedotov on 06.10.15.
//  Copyright © 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <random>
#include <vector>

struct Dot {
    double XCoordinate;
    double YCoordinate;
};

struct Edge {
    size_t from;
    size_t to;
    long long capacity;
    long long flow;
};


void generateRandomNet() {
    std::vector<Dot> vertices;
    std::vector<Edge> edges;
    std::vector<std::vector<size_t>> matrix;
    size_t vertexNumber;
    std::default_random_engine justRandom (std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> vertexNumberRandomizer(5, 15);
    std::uniform_int_distribution<int> amountRandomizer(1, 50);
    std::uniform_int_distribution<int> sigmaRandomizer(20, 50);
    std::normal_distribution<double> verticeDistribution (0.0, sigmaRandomizer(justRandom));
    //Making Dots with Normal distribution
    vertexNumber = vertexNumberRandomizer(justRandom);
    std::cout << vertexNumber << "verNum\n;";
    std::cout << "dasd";
    matrix.resize(vertexNumber);
    size_t edgesCounter = 0;
    for(size_t from = 0; from < vertexNumber; ++from) {
        std::normal_distribution<double> firstCoordDistribution (verticeDistribution(justRandom), sigmaRandomizer(justRandom));
        std::normal_distribution<double> secondCoordDistribution (verticeDistribution(justRandom), sigmaRandomizer(justRandom));
        vertices.push_back({firstCoordDistribution(justRandom), secondCoordDistribution(justRandom)});
        for(size_t to = 1; to < vertexNumber; ++to) {
            if(rand() % 2) {
                long long capacity = amountRandomizer(justRandom);
                edges.push_back({from, to, capacity, 0});
                edges.push_back({to, from, 0, 0});
                matrix[from].push_back(edgesCounter);
                matrix[to].push_back(edgesCounter + 1);
                edgesCounter += 2;
            }
        }
        
    }
    
    std::cout << edges.size() << "\n\n";
    for(auto &edge : edges) {
        
        std::cout << edge.from << " " << edge.to << " " << edge.capacity << "\n";
    }
    for(auto &vert : vertices)
        std::cout << vert.XCoordinate << "  " << vert.YCoordinate << "\n";
    
    
}

int main() {
    generateRandomNet();
    
    return 0;
}
