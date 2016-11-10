//
//  main.cpp
//  CommisVoyageur
//
//  Created by Simon Fedotov on 09.05.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <random>
#include <chrono>
#include <queue>

#include "/Users/semenfedotov/Library/Containers/com.apple.mail/Data/Library/Mail Downloads/FEB7BE24-C035-4FFF-8967-142E995F8837/Drawer.h"

std::vector<char> Visited;
std::vector<int> VertexSequence;


//void BFS(std::vector<std::vector<std::pair<int, int>>> &OurGraph, int &NodeNumber, int &VertexNumber) {
//    VertexSequence.resize(VertexNumber);
//    std::queue<int> OurQueue;
//    OurQueue.push(NodeNumber);
//    
//    while (!OurQueue.empty()) {
//        int ThisVertex = OurQueue.front();
//        OurQueue.pop();
//        for(int i = 0; i < OurGraph[NodeNumber].size(); ++i){
//            if(!Visited[OurGraph[NodeNumber][i].second]) {
//                Visited[OurGraph[NodeNumber][i].second] = true;
//                OurQueue.push(OurGraph[NodeNumber][i].second);
//                VertexSequence[OurGraph[NodeNumber][i].second] = VertexSequence[ThisVertex] + 1;
//            }
//            
//        }
//
//    }
//    
//}


class DSU {
private:
    std::vector<int> Parents;
    std::vector<int> Ranks;
public:
    DSU() = default;
    DSU(const int &NumberOfNodes);
    
    int Find(int &NodeNumber);
    void Union(int FirstNode, int SecondNode);
    void RanksUnion(int FirstNode, int SecondNode);
};

DSU::DSU(const int &NumberOfNodes){
    for(int i = 0 ; i < NumberOfNodes; ++i)
        Parents.push_back(i);
    Ranks.resize(NumberOfNodes, 0);
}

int DSU::Find(int &NodeNumber) {
    return (NodeNumber == Parents[NodeNumber] ? NodeNumber : Parents[NodeNumber] = Find(Parents[NodeNumber]));
}

void DSU::Union(int FirstNode, int SecondNode) {
    FirstNode = Find(FirstNode);
    SecondNode = Find(SecondNode);
    if(rand() & 1)
        std::swap(FirstNode, SecondNode);
    if(FirstNode != SecondNode)
        Parents[FirstNode] = SecondNode;
}

void DSU::RanksUnion(int FirstNode, int SecondNode) {
    FirstNode = Find(FirstNode);
    SecondNode = Find(SecondNode);
    if(FirstNode != SecondNode) {
        if(Ranks[FirstNode] < Ranks[SecondNode])
            std::swap(FirstNode, SecondNode);
        Parents[SecondNode] = FirstNode;
        if(Ranks[FirstNode] == Ranks[SecondNode])
            ++Ranks[FirstNode];
    }
}

class SpanningTree {
private:
    int SummaryDistance;
    int VertexNumber;
    DSU MySet;
    std::vector<std::vector<std::pair<int, int>>> MySpanningTree;
    std::vector<Edge> Edges;
    std::vector<Point> OurDots;
    
    void DFS(std::vector<std::vector<std::pair<int, int>>> &OurGraph, int NodeNumber);
    
public:
    SpanningTree(const int& NumberOfVertex, std::vector<Point> &CreatedDots);
    int GetDistance();
    std::vector<std::vector<std::pair<int, int>>> GetSpanningTree();
    
    std::vector<std::pair<int, int>> Run();
};

SpanningTree::SpanningTree(const int& NumberOfVertex, std::vector<Point> &CreatedDots){
    SummaryDistance = 0;
    VertexNumber = NumberOfVertex;
    MySpanningTree.resize(NumberOfVertex);
    OurDots = CreatedDots;
    MySet = DSU(NumberOfVertex);
    Edges.resize(NumberOfVertex * (NumberOfVertex - 1));
    
    //Creating Edges
    for(int From = 0; From < OurDots.size(); ++From){
        for(int To = 0; To < OurDots.size(); ++To){
            if(From >= To)
                continue;
            
            double Distance = sqrt((OurDots[From].X - OurDots[To].X) * (OurDots[From].X - OurDots[To].X)
                                   +
                                   (OurDots[From].Y - OurDots[To].Y) *  (OurDots[From].Y - OurDots[To].Y));
            
            Edges[From * NumberOfVertex + To] = {Distance, From, To};
        }
    }
    
}

int SpanningTree::GetDistance(){
    return SummaryDistance;
}

std::vector<std::vector<std::pair<int, int>>> SpanningTree::GetSpanningTree(){
    return MySpanningTree;
}



void SpanningTree::DFS(std::vector<std::vector<std::pair<int, int>>> &OurGraph, int NodeNumber) {
    
    Visited[NodeNumber] = true;
    VertexSequence.push_back(NodeNumber);
    for(int i = 0; i < OurGraph[NodeNumber].size(); ++i){
        if(!Visited[OurGraph[NodeNumber][i].second]) {
            DFS(OurGraph, OurGraph[NodeNumber][i].second);
        }
        
    }
    
}


std::vector<std::pair<int, int>> SpanningTree::Run() {
    auto Bu1 = std::chrono::steady_clock::now();
    std::sort(Edges.begin(), Edges.end());
    auto Bu2 = std::chrono::steady_clock::now();
    std::cout << "\nThe Second time is = " << std::chrono::duration_cast<std::chrono::microseconds>(Bu2 - Bu1).count() * 0.000001 << "\n";

    auto Start = std::chrono::steady_clock::now();
    for(int i = 0; i < Edges.size(); ++i){
        int From = Edges[i].From, To = Edges[i].To;
        double DatDistance = Edges[i].DatDistance;
        if((MySet.Find(From) != MySet.Find(To))) {
            SummaryDistance += DatDistance;
            MySpanningTree[From].push_back(std::make_pair(From, To));
            MySpanningTree[To].push_back(std::make_pair(To, From));
            MySet.RanksUnion(From, To);
        }
    }
    auto End = std::chrono::steady_clock::now();
    
    DFS(MySpanningTree, 0);
    Visited.clear();
    Visited.resize(VertexNumber, false);
    VertexSequence.push_back(0);
    
//    std::cout << "\nThe Second time is = " << std::chrono::duration_cast<std::chrono::microseconds>(End - Start).count() * 0.000001 << "\n";
    
    std::vector<std::pair<int, int>> KruskalAnswer;
    for(int i = 0; i < VertexSequence.size() - 1; ++i) {
        KruskalAnswer.push_back(std::make_pair(VertexSequence[i], VertexSequence[i + 1]));
    }

    return KruskalAnswer;
}


int main(int argc, const char * argv[]) {
    int FirstSigma, SecondSigma, CentersNumber, DotsNumber;
    std::cin >> FirstSigma >> SecondSigma >> CentersNumber >> DotsNumber;
    int NumberOfVertex = CentersNumber * DotsNumber;
    Visited.resize(NumberOfVertex, false);

    std::default_random_engine JustRandom (std::chrono::system_clock::now().time_since_epoch().count());
    std::normal_distribution<double> CenterDistribution (0.0, FirstSigma);
    //Making Dots with Normal distribution
    std::vector<Point> OurDots(NumberOfVertex);
    for(int i = 0; i < CentersNumber; ++i) {
        std::normal_distribution<double> FirstCoordDistribution (CenterDistribution(JustRandom), SecondSigma);
        std::normal_distribution<double> SecondCoordDistribution (CenterDistribution(JustRandom), SecondSigma);
        
        for(int j = 0; j < DotsNumber; ++j)
            OurDots[i * DotsNumber + j] = {FirstCoordDistribution(JustRandom), SecondCoordDistribution(JustRandom)};
    }
    
    SpanningTree MySpanningTree(NumberOfVertex, OurDots);
    std::vector<std::pair<int, int>> OurAnswer = MySpanningTree.Run();

    std::cout << "<>=<>-<>=<><>=<>-<>=<>The Answer is<>=<>-<>=<><>=<>-<>=<> \n";
    for(int i = 0; i < OurAnswer.size(); ++i)
        std::cout << OurAnswer[i].first << " " << OurAnswer[i].second << "\n";
    std::cout << "<>=<>-<>=<><>=<>-<>=<>The Answer is<>=<>-<>=<><>=<>-<>=<> \n";

    std::cout << "~~~~~~~Summary Distance is " << MySpanningTree.GetDistance() << "~~~~~~~\n";
        AnswerVisualizer(OurDots, OurAnswer, DotsNumber);
    std::vector<std::vector<std::pair<int, int>>> Bu = MySpanningTree.GetSpanningTree();
//    SpanningTreeVisualizer(OurDots, Bu, DotsNumber);
    return 0;
}
