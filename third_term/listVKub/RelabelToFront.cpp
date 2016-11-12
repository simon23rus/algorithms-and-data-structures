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
#include <limits>
#include <chrono>

#define source 0

struct Edge {
    size_t from;
    size_t to;
    long long capacity;
    long long flow;
};

std::vector<std::vector<long long>> createGraph(const size_t &vertexNumber, const size_t &edgesNumber) {
    std::vector<std::vector<long long>> graph(vertexNumber);
    for(size_t i = 0; i < vertexNumber; ++i)
        graph[i].resize(vertexNumber);
    //    for(long long i = 0; i < edgesNumber; ++i) {
    //        long long from, to, capacity;
    //        from = rand() % vertexNumber;
    //        to = rand() % vertexNumber;
    //        capacity = rand() % 123456;
    //    }
    for(long long i = 0; i < vertexNumber; ++i)
        for (long long j  = 0; j < vertexNumber; ++j) {
            graph[i][j] = rand() % 123456;
        }
    //    for(long long i = 0; i < vertexNumber - 1; ++i)
    //        graph[i][i + 1] = 300;
    return graph;
}

class Net {
    std::vector<Edge> edges;
    std::vector<std::vector<size_t>> listOfEdges;
    std::vector<long long> excess;
    std::vector<size_t> height;
    std::vector<size_t> positionInSequence;
    std::list<size_t> vertexQueue;
    size_t vertexNumber;
    
    void Push(Edge &edgeToPush, Edge &reverseEdge);
    void Lift(const size_t &from);
    void Discharge(const size_t &from);
    void toFront(std::list<size_t>::iterator &indexToMove);
public:
    void RelabelToFront();
    
    Net(std::vector<Edge> &ourEdges, const size_t &nodeNumber, std::vector<std::vector<size_t>> &matrix
) : edges(ourEdges), vertexNumber(nodeNumber) {
        listOfEdges = matrix;
        excess.resize(vertexNumber, 0);
        excess[source] = INT64_MAX;
        height.resize(vertexNumber, 0);
        height[source] = vertexNumber;
        for(size_t i = 0; i < vertexNumber; ++i)
            positionInSequence.push_back(listOfEdges[i].size() == 0 ? 0 : listOfEdges[i][0]);
    }
    
};

void Net::Push(Edge &edgeToPush, Edge &reverseEdge) {
    long long amountToPush = std::min(excess[edgeToPush.from], edgeToPush.capacity - edgeToPush.flow);
    edgeToPush.flow += amountToPush;
    reverseEdge.flow = -edgeToPush.flow;
    excess[edgeToPush.from] -= amountToPush;
    excess[edgeToPush.to] += amountToPush;
}

void Net::toFront(std::list<size_t>::iterator &indexToMove) {
    vertexQueue.push_front(*indexToMove);
    vertexQueue.erase(indexToMove);
    indexToMove = vertexQueue.begin();
}

void Net::Lift(const size_t &from) {
    size_t minimalHeight = std::numeric_limits<size_t>::max();
    for(auto &pos : listOfEdges[from]) {
        if(edges[pos].capacity - edges[pos].flow > 0)
            minimalHeight = std::min(minimalHeight, height[edges[pos].to]);
    }
    height[from] = minimalHeight + 1;
}

void Net::Discharge(const size_t &from) {
    while (excess[from] > 0) {
        size_t to = positionInSequence[from];
        if(to < listOfEdges[from].size()) {
            if(edges[listOfEdges[from][to]].capacity - edges[listOfEdges[from][to]].flow > 0 && height[from] == height[edges[listOfEdges[from][to]].to] + 1)
                Push(edges[listOfEdges[from][to]], (listOfEdges[from][to] % 2 == 0)? edges[listOfEdges[from][to] + 1]:edges[listOfEdges[from][to] - 1]);
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
    for(auto &to : listOfEdges[source]) {
        if(edges[to].capacity != 0)
            Push(edges[to], (to % 2 == 0)? edges[to + 1]:edges[to - 1]); //tk rebra dobavlyayutsa po 2
    }
    for(size_t to = source + 1; to < vertexNumber - 1; ++to)
        vertexQueue.push_back(to);
    
    auto currentPosition = vertexQueue.begin();
    while(currentPosition != vertexQueue.end()) {
        size_t thisNode = *currentPosition;
        size_t oldHeight = height[thisNode];
        Discharge(thisNode);
        if(oldHeight != height[thisNode])
            toFront(currentPosition);
        
            ++currentPosition;
    }
    
    std::vector<long long> edgeFlows;
    long long maxFlow = 0;
    for(auto &edge : edges) {
        if(edge.to == vertexNumber - 1) {
            maxFlow += edge.flow;
        }
    }
    for(size_t i = 0; i < edges.size(); i += 2)
        edgeFlows.push_back(edges[i].flow);
    std::cout << maxFlow << "\n";
    for(auto &flow : edgeFlows)
        std::cout << flow << "\n";

    vertexQueue.clear();
}



int main(int argc, const char * argv[]) {
    std::ios_base::sync_with_stdio(false);
    //    auto start = std::chrono::steady_clock::now();
    std::vector<Edge> thisEdges;
    std::vector<std::vector<size_t>> matrix;
    size_t vertexNumber, edgesNumber;
    std::cin >> vertexNumber >> edgesNumber;
    matrix.resize(vertexNumber);
    size_t from, to;
    long long capacity;
    for(size_t i = 0; i < edgesNumber; ++i) {
        std::cin >> from >> to >> capacity;
        thisEdges.push_back({from - 1, to - 1, capacity, 0});
        thisEdges.push_back({to - 1, from - 1, 0, 0});
        matrix[from - 1].push_back(2 * i);
        matrix[to - 1].push_back(2 * i  + 1);
    }
    //    thisCapacities = createGraph(500, 30000);
    Net myFirstNet = Net(thisEdges, vertexNumber, matrix);
//    auto start = std::chrono::steady_clock::now();
    myFirstNet.RelabelToFront();
//    auto end = std::chrono::steady_clock::now();
    //    std::cout << "\n And the time is "<<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001;
    return 0;
}
