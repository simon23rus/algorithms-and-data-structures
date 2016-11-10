//
//  main.cpp
//  Dijkstra
//
//  Created by Simon Fedotov on 06.04.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <set>

int main(int argc, const char * argv[]) {
    std::vector<std::vector<std::pair<int, int>>> OurGraph;
    int NodeNumber, StartNode, FinishNode;
    int Weight;
    std::cin >> NodeNumber >> StartNode >> FinishNode;
    OurGraph.resize(NodeNumber);
    for(int i = 0 ; i < NodeNumber; ++i) {
        for(int j = 0; j < NodeNumber; ++j) {
            std::cin >> Weight;
            OurGraph[i].push_back(std::make_pair(j, Weight));
        }
    }
    std::vector<int> Costs(NodeNumber, INT32_MAX);
    std::vector<int> Parents(NodeNumber, -1);
    Costs[StartNode - 1] = 0;
    std::set<std::pair<int, int>> OurQueue;
    OurQueue.insert(std::make_pair(Costs[StartNode - 1], StartNode - 1));
//    for(int i = 0 ; i < NodeNumber; ++i) {
//        for(int j = 0; j < NodeNumber; ++j) {
//            std::cout << OurGraph[i][j] << " ";
//        }
//        std::cout << "\n";
//    }
    
    
    while(!OurQueue.empty()) {
        int MinNode = OurQueue.begin() -> second;
        OurQueue.erase(OurQueue.begin());
        
        for(int j = 0; j < NodeNumber; ++j) {
            if(OurGraph[MinNode][j].second != 0 && OurGraph[MinNode][j].second != -1) {
                int OurNeighbor = OurGraph[MinNode][j].first;
                int OurWeight = OurGraph[MinNode][j].second;
                if(Costs[MinNode] + OurWeight < Costs[OurNeighbor]) {
                    OurQueue.erase(std::make_pair(Costs[OurNeighbor], OurNeighbor));
                    Costs[OurNeighbor] = Costs[MinNode] + OurWeight;
                    Parents[OurNeighbor] = MinNode;
        
                    OurQueue.insert(std::make_pair(Costs[OurNeighbor], OurNeighbor));
                }
            }
        }

    }
    
    
    
//    for(int Counter = 0; Counter < NodeNumber; ++Counter) {
//        int MinNode = 0;
//        int MinCost = INT32_MAX;
//        for(int j = 0; j < NodeNumber; ++j) {
//            if(!Checked[j] && Costs[j] < MinCost) {
//                MinNode = j;
//                MinCost = Costs[j];
//            }
//        }
//        int RepairNode = MinNode;
//        Checked[RepairNode] = true;
//        for(int Neighbor = 0; Neighbor < NodeNumber; ++Neighbor) {
//            if(!Checked[Neighbor] && OurGraph[RepairNode][Neighbor] >= 0 && Costs[RepairNode] + OurGraph[RepairNode][Neighbor] < Costs[Neighbor] && MinCost != INT32_MAX) {
//                
//                Costs[Neighbor] = OurGraph[RepairNode][Neighbor] + Costs[RepairNode];
//                if(Parents[Neighbor] == -1)
//                    Parents[Neighbor] = RepairNode;
//            }
//        }
//        
//    }
//
    
    
    if(Parents[FinishNode - 1] == -1)
        std::cout << -1;
    else
        std::cout << Costs[FinishNode - 1];

//    std::vector<int> OurWay;
//    
//    if(Parents[FinishNode - 1] == -1)
//        std::cout << -1;
//    else {
//        for(int i = Parents[FinishNode - 1]; i != (StartNode - 1); i = Parents[i])
//            OurWay.push_back(i + 1);
//        std::reverse(OurWay.begin(), OurWay.end());
//    }
//    for(int i = 0; i < OurWay.size(); ++i) {
//        std::cout << OurWay[i] << " ";
//    }
    return 0;
}
