//
//  main.cpp
//  EdmondsKarp
//
//  Created by Simon Fedotov on 12.09.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>
#include <chrono>
#include <cstddef>

#define source 0

static std::vector<char> visited;

class Graph {
protected:
    size_t vertexNumber;
    std::vector<std::vector<int>> netFlows;
    std::vector<std::vector<int>> netCapacity;
    std::vector<std::vector<int>> remainingCapacity;
};

class Net: private Graph {
    int maxFlow;
    std::vector<size_t> ourPath;

    bool BFS(const int &nodeNumber);
public:
    Net(const size_t &nodesNumber, const size_t &edgesNumber, std::vector<std::vector<int>> &capacities);
    int FindTheMaxFlow();
};

Net::Net(const size_t &nodesNumber, const size_t &edgesNumber, std::vector<std::vector<int>> &capacities) {
    netCapacity = capacities;
    remainingCapacity = netCapacity;
    vertexNumber = nodesNumber;
    maxFlow = 0;
    netFlows.resize(nodesNumber);
    for(size_t i = 0; i < nodesNumber; ++i)
        netFlows[i].resize(nodesNumber, 0);
}

bool Net::BFS(const int &nodeNumber) {
    size_t runoffVertex = vertexNumber - 1;
    ourPath.clear();
    visited.clear();
    visited.resize(vertexNumber, false);
    std::vector<int> parents(vertexNumber, -1);
    std::queue<size_t> ourQueue;
    ourQueue.push(nodeNumber);
    
    while (!ourQueue.empty()) {
        size_t thisVertex = ourQueue.front();
        ourQueue.pop();
        for(size_t to = 0; to < vertexNumber; ++to){
            if(!visited[to] && remainingCapacity[thisVertex][to] != 0) {
                visited[to] = true;
                ourQueue.push(to);
                parents[to] = (int)thisVertex;
                
                if(to == runoffVertex) {
                    ourPath.push_back(to);
                    size_t parentNode = thisVertex;
                    while (parentNode != 0) {
                        ourPath.push_back(parentNode);
                        parentNode = parents[parentNode];
                    }
                    ourPath.push_back(source);
                    break;
                }

            }
            
        }
        
    }
    
    return visited[runoffVertex];
    
}

int Net::FindTheMaxFlow() {
    size_t minPathCapacity = INT32_MAX, lastNode;
    std::vector<std::pair<int, int>> pathEdges;
    while (BFS(source)) {
        lastNode = source;
        for(ptrdiff_t i = (int)ourPath.size() - 2; i >= 0; --i) {
            pathEdges.push_back({lastNode, ourPath[i]});
            if(remainingCapacity[lastNode][ourPath[i]] < minPathCapacity)
                minPathCapacity = remainingCapacity[lastNode][ourPath[i]];
            
            lastNode = ourPath[i];
        }
        for(size_t i = 0; i < pathEdges.size(); ++i) {
            netFlows[pathEdges[i].first][pathEdges[i].second] += minPathCapacity;
            remainingCapacity[pathEdges[i].first][pathEdges[i].second] -= minPathCapacity;
            remainingCapacity[pathEdges[i].second][pathEdges[i].first] += minPathCapacity;
        }
        pathEdges.clear();
        maxFlow += minPathCapacity;
    }
    return maxFlow;
}


int main(int argc, const char * argv[]) {
    std::ios_base::sync_with_stdio(false);
    
    size_t nodesNumber, edgesNumber;
    std::cin >> nodesNumber >> edgesNumber;
    std::vector<std::vector<int>> capacities(nodesNumber);
    for(int i = 0; i < nodesNumber; ++i)
        capacities[i].resize(nodesNumber, 0);
    for(int i = 0; i < edgesNumber; ++i) {
        int from, to, capacity;
        std::cin >> from >> to >> capacity;
        capacities[from - 1][to - 1] = capacity;
    }
    
    Net myNet = Net(nodesNumber, edgesNumber, capacities);
    auto start = std::chrono::steady_clock::now();
    int Result = myNet.FindTheMaxFlow();
    auto end = std::chrono::steady_clock::now();
    std::cout << "\n And the time is "<<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001 << "\n";
    std::cout << Result;
    return 0;
}
