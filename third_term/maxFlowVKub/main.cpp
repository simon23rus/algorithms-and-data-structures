//
//  main.cpp
//  maxFlowVKub
//
//  Created by Simon Fedotov on 28.09.15.
//  Copyright © 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <chrono>

#define source 0

std::vector<std::vector<long long>> createGraph(const int &vertexNumber, const int &edgesNumber) {
    std::vector<std::vector<long long>> graph(vertexNumber);
    for(int i = 0; i < vertexNumber; ++i)
        graph[i].resize(vertexNumber);
//    for(int i = 0; i < edgesNumber; ++i) {
//        int from, to, capacity;
//        from = rand() % vertexNumber;
//        to = rand() % vertexNumber;
//        capacity = rand() % 123456;
//    }
    for(int i = 0; i < vertexNumber; ++i)
        for (int j  = 0; j < vertexNumber; ++j) {
            graph[i][j] = rand() % 123456;
        }
//    for(int i = 0; i < vertexNumber - 1; ++i)
//        graph[i][i + 1] = 300;
    return graph;
}

class Net {
    std::vector<std::vector<long long>> capacities;
    std::vector<std::vector<long long>> flows;
    std::vector<long long>excess;
    std::vector<int>height;
    std::vector<int>positionInSequence;
    std::list<int>vertexQueue;
    int vertexNumber;
    
    void Push(const int &from, const int &to);
    void Lift(const int &from);
    void Discharge(const int &from);
    void toFront(std::list<int>::iterator &indexToMove);
public:
    void RelabelToFront();
    
    Net(std::vector<std::vector<long long>> &ourCapacities) : capacities(ourCapacities) {
        vertexNumber = capacities.size();
        flows.resize(vertexNumber);
        for(int i = 0; i < vertexNumber; ++i)
            flows[i].resize(vertexNumber, 0);
        excess.resize(vertexNumber, 0);
        excess[source] = INT32_MAX;
        height.resize(vertexNumber, 0);
        height[source] = vertexNumber;
        positionInSequence.resize(vertexNumber, 0);
    }

};

void Net::Push(const int &from, const int &to) {
    long long amountToPush = std::min(excess[from], capacities[from][to] - flows[from][to]);
    flows[from][to] += amountToPush;
    flows[to][from] = -flows[from][to];
    excess[from] -= amountToPush;
    excess[to] += amountToPush;
}

void Net::toFront(std::list<int>::iterator &indexToMove) {
    vertexQueue.push_front(*indexToMove);
    vertexQueue.erase(indexToMove);
    indexToMove = vertexQueue.begin();
}

void Net::Lift(const int &from) {
    int minimalHeight = INT32_MAX;
    for(int to = 0; to < vertexNumber; ++to) {
        if(capacities[from][to] - flows[from][to] > 0)
            minimalHeight = std::min(minimalHeight, height[to]);
    }
    height[from] = minimalHeight + 1;
}

void Net::Discharge(const int &from) {
    while (excess[from] > 0) {
        int to = positionInSequence[from];
        if(capacities[from][to] - flows[from][to] <= 0) {
            ++positionInSequence[from];
            continue;
        }
        if(to < vertexNumber) {
            if(capacities[from][to] - flows[from][to] > 0 && height[from] == height[to] + 1)
                Push(from, to);
            else
                ++positionInSequence[from];
        }
        else {
            Lift(from);
            positionInSequence[from] = 0;
        }
    }
}

void Net::RelabelToFront() {
    for(int to = 1; to < vertexNumber; ++to) {
        if(capacities[source][to] != 0)
            Push(source, to);
    }
    for(int to = source + 1; to < vertexNumber - 1; ++to)
        vertexQueue.push_back(to);
    
    auto currentPosition = vertexQueue.begin();
    while(currentPosition != vertexQueue.end()) {
        int thisNode = *currentPosition;
        int oldHeight = height[thisNode];
        Discharge(thisNode);
        if(oldHeight != height[thisNode])
            toFront(currentPosition);
        else
            ++currentPosition;
    }
    
    int maxFlow = 0;
    for(int from = 0; from < vertexNumber; ++from) {
        maxFlow += flows[from][vertexNumber - 1];
    }
    std::cout << maxFlow;
    vertexQueue.clear();
}



int main(int argc, const char * argv[]) {
    std::ios_base::sync_with_stdio(false);
//    auto start = std::chrono::steady_clock::now();
    std::vector<std::vector<long long>> thisCapacities;
    int vertexNumber, edgesNumber;
    std::cin >> vertexNumber >> edgesNumber;
    thisCapacities.resize(vertexNumber);
    for(int i = 0; i < thisCapacities.size(); ++i)
        thisCapacities[i].resize(vertexNumber, 0);
    int from, to, capacity;
    for(int i = 0; i < edgesNumber; ++i) {
        std::cin >> from >> to >> capacity;
        thisCapacities[from - 1][to - 1] += capacity;
    }
//    thisCapacities = createGraph(500, 30000);
    Net myFirstNet = Net(thisCapacities);
    auto start = std::chrono::steady_clock::now();
    myFirstNet.RelabelToFront();
    auto end = std::chrono::steady_clock::now();
//    std::cout << "\n And the time is "<<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001;
    return 0;
}
