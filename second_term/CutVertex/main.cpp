//
//  main.cpp
//  CutVertex
//
//  Created by Simon Fedotov on 27.04.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

std::vector<std::vector<int>> OurGraph;
std::vector<bool> UsedNodes;
int Time;
std::vector<int> TimeIn;
std::vector<int> UpEdges;
std::set<int> CutNodes;

void DFSForCut(int NodeNumber, int Parent = -1) {
    UsedNodes[NodeNumber] = true;
    TimeIn[NodeNumber] = UpEdges[NodeNumber] = ++Time;
    int ChildNumber = 0;
    for (int i = 0; i < OurGraph[NodeNumber].size(); ++i) {
        int Destination = OurGraph[NodeNumber][i];
        if (Destination == Parent)
            continue;
        if (UsedNodes[Destination])
            UpEdges[NodeNumber] = std::min(UpEdges[NodeNumber], TimeIn[Destination]);
        else {
            DFSForCut(Destination, NodeNumber);
            UpEdges[NodeNumber] = std::min(UpEdges[NodeNumber], UpEdges[Destination]);
            if (UpEdges[Destination] >= TimeIn[NodeNumber] && Parent != -1)
                CutNodes.insert(NodeNumber);
            ++ChildNumber;
        }
    }
    if(Parent == -1 && ChildNumber > 1)
        CutNodes.insert(NodeNumber);
}

void DFSForBridges(int NodeNumber, int Parent = -1) {
    UsedNodes[NodeNumber] = true;
    TimeIn[NodeNumber] = UpEdges[NodeNumber] = ++Time;
    int ChildNumber = 0;
    for (int i = 0; i < OurGraph[NodeNumber].size(); ++i) {
        int Destination = OurGraph[NodeNumber][i];
        if (Destination == Parent)
            continue;
        if (UsedNodes[Destination])
            UpEdges[NodeNumber] = std::min(UpEdges[NodeNumber], TimeIn[Destination]);
        else {
            DFSForBridges(Destination, NodeNumber);
            UpEdges[NodeNumber] = std::min(UpEdges[NodeNumber], UpEdges[Destination]);
            if (UpEdges[Destination] > TimeIn[NodeNumber] && Parent != -1)
                CutNodes.insert(NodeNumber);
            ++ChildNumber;
        }
    }
    if(Parent == -1 && ChildNumber > 1)
        CutNodes.insert(NodeNumber);
}


int main() {
    int NumberOfNodes;
    std::cin >> NumberOfNodes;
    OurGraph.resize(NumberOfNodes);
    UsedNodes.resize(NumberOfNodes, false);
    UpEdges.resize(NumberOfNodes);
    TimeIn.resize(NumberOfNodes);
    
    for(int i = 0; i < NumberOfNodes; ++i) {
        int Number;
        std::cin >> Number;
        while (Number >= 0) {
            OurGraph[i].push_back(Number);
            std::cin >> Number;
        }
    }
    
    
    Time = 0;
    DFSForCut(0);
    
    std::set<int>::iterator bubu;
    for(bubu = CutNodes.begin(); bubu != CutNodes.end(); ++bubu) {
        std::cout << *bubu << " ";
    }
        std::cout << "\n";
    
    
    
}