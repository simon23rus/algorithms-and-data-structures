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
#include <stack>

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

class PerfectMatching {
    int vertexNumber;
    std::vector<std::vector<double>> edges_;
public:
    PerfectMatching(std::vector<std::vector<std::pair<int,int>>> &spanningTree, std::vector<Edge> &edges);
    std::vector<int> oldIndices;
    std::vector<int> matching;
    void run();
};

PerfectMatching::PerfectMatching(std::vector<std::vector<std::pair<int,int>>> &spanningTree, std::vector<Edge> &edges) {
    // we have to use in perfect matching only odd-degree vertices!.

    for(int i = 0; i < spanningTree.size(); ++i) {
        if(spanningTree[i].size() % 2 != 0) // ok
            oldIndices.push_back(i);
    }
    vertexNumber = oldIndices.size();
    
    edges_.push_back(std::vector<double>(oldIndices.size() + 1, INT32_MAX));
    for(int i = 0; i < oldIndices.size(); ++i) {
        std::vector<double> toPush;
        toPush.push_back(INT32_MAX);
        for(int j = 0; j < oldIndices.size(); ++j) {
            if(i == j)
                toPush.push_back(INT32_MAX);
            toPush.push_back(edges[oldIndices[i] * spanningTree.size() + oldIndices[j]].DatDistance);
        }
        edges_.push_back(toPush);
    }
    
}

void PerfectMatching::run() {
    std::vector<int> u(vertexNumber + 1), v(vertexNumber + 1), p(vertexNumber + 1), way(vertexNumber + 1);
    for (int i = 1; i <= vertexNumber; ++i) {
        p[0] = i;
        int j0 = 0;
        std::vector<int> minv (vertexNumber + 1, INT32_MAX);
        std::vector<char> used (vertexNumber + 1, false);
        do {
            used[j0] = true;
            int i0 = p[j0];
            int delta = INT32_MAX;
            int j1;
            for (int j = 1; j <= vertexNumber; ++j) {
                if (!used[j]) {
                    int cur = edges_[i0][j]-u[i0]-v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }
            for (int j = 0; j <= vertexNumber; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                }
                else
                    minv[j] -= delta;
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    
    std::vector<int> ans (vertexNumber + 1);
    for (int j = 1; j <= vertexNumber; ++j)
        ans[p[j]] = oldIndices[j - 1];
    matching = ans;
    
}


std::vector<int> eulerCycle(std::vector<std::vector<int>> &graph) {
    std::vector<int> answer;
    int vertexNumber = graph.size();
    std::vector<int> deg(vertexNumber);
    for (int i=0; i < vertexNumber; ++i)
        for (int j=0; j < vertexNumber; ++j)
            deg[i] += graph[i][j];
    
    int first = 0;
    while (!deg[first])
        ++first;
    
    int v1 = -1,  v2 = -1;
    bool bad = false;
    for (int i = 0; i < vertexNumber; ++i) {
        if (deg[i] & 1) {
            if (v1 == -1)
				v1 = i;
            else if (v2 == -1)
				v2 = i;
            else
				bad = true;
        }
    }
    
    if (v1 != -1) {
        ++graph[v1][v2];
        ++graph[v2][v1];
    }
    
    std::stack<int> st;
    st.push (first);
    std::vector<int> res;
    while (!st.empty()) {
        int v = st.top();
        int i;
        for (i = 0; i < vertexNumber; ++i)
            if (graph[v][i])
                break;
        if (i == vertexNumber) {
            res.push_back (v);
            st.pop();
        }
        else {
            --graph[v][i];
            --graph[i][v];
            st.push (i);
        }
    }
    
    if (v1 != -1)
    for (size_t i=0; i+1<res.size(); ++i) {
        if (res[i] == v1 && (res[i+1] == v2 || res[i] == v2) && res[i+1] == v1) {
            std:: vector<int> res2;
            for (size_t j=i+1; j<res.size(); ++j)
                res2.push_back (res[j]);
            for (size_t j=1; j<=i; ++j)
                res2.push_back (res[j]);
            res = res2;
            break;
        }
    }
    
    for (int i = 0; i < vertexNumber; ++i)
        for (int j = 0; j < vertexNumber; ++j)
            if (graph[i][j])
				bad = true;
    
    if (bad)
        return {};
    else {
        for (size_t i=0; i<res.size(); ++i) {
            answer.push_back(res[i]);
        }
    }
    return answer;
}

std::vector<int> eulerToHamilton(std::vector<int> &eulerCycle, int vertexNumber) {
    std::vector<int> answer;
    std::vector<char> visited(vertexNumber);
    for(int i = 0; i < eulerCycle.size(); ++i) {
        if(visited[eulerCycle[i]])
            continue;
        else {
            visited[eulerCycle[i]] = true;
            answer.push_back(eulerCycle[i]);
        }
    }
    return answer;
}



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
    std::vector<Point> OurDots;
    
    void DFS(std::vector<std::vector<std::pair<int, int>>> &OurGraph, int NodeNumber);
    
public:
    std::vector<Edge> Edges;
    std::vector<Edge> EdgesFixed;
    std::vector<std::vector<std::pair<int, int>>> MySpanningTree;
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
    Edges.resize(NumberOfVertex * NumberOfVertex);
    
    
    //Creating Edges
    for(int From = 0; From < OurDots.size(); ++From){
        for(int To = 0; To < OurDots.size(); ++To){
//            if(From == To)
//                continue;
//            
            double Distance = sqrt((OurDots[From].X - OurDots[To].X) * (OurDots[From].X - OurDots[To].X)
                                   +
                                   (OurDots[From].Y - OurDots[To].Y) *  (OurDots[From].Y - OurDots[To].Y));
            
            Edges[From * NumberOfVertex + To] = {Distance, From, To};
//            Edges.push_back({Distance, From, To});
        }
    }
    EdgesFixed = Edges;
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

std::vector<int> hungary(std::vector<std::vector<double>> &edges_, int vertexNumber) {
    std::vector<int> u(vertexNumber + 1), v(vertexNumber + 1), p(vertexNumber + 1), way(vertexNumber + 1);
    for (int i = 1; i <= vertexNumber; ++i) {
        p[0] = i;
        int j0 = 0;
        std::vector<int> minv (vertexNumber + 1, INT32_MAX);
        std::vector<char> used (vertexNumber + 1, false);
        do {
            used[j0] = true;
            int i0 = p[j0];
            int delta = INT32_MAX;
            int j1;
            for (int j = 1; j <= vertexNumber; ++j) {
                if (!used[j]) {
                    int cur = edges_[i0][j]-u[i0]-v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }
            for (int j = 0; j <= vertexNumber; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                }
                else
                    minv[j] -= delta;
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }
    
    std::vector<int> ans (vertexNumber + 1);
    for (int j = 1; j <= vertexNumber; ++j)
        ans[p[j]] = j - 1;
    return ans;
}


int main(int argc, const char * argv[]) {
    
//    std::vector<std::vector<double>> edges(5);
//    edges[0].resize(5, INT32_MAX);
//    for(int i = 1; i <= 4; ++i) {
//        edges[i].push_back(INT32_MAX);
//        for(int j = 1; j <= 4; ++j) {
//            double x;
//            std::cin >> x;
//            edges[i].push_back(x);
//        }
//    }
//    
//    auto ansa = hungary(edges, 4);
    
    
    
    int FirstSigma, SecondSigma, CentersNumber, DotsNumber;
    std::cin >> FirstSigma >> SecondSigma >> CentersNumber >> DotsNumber;
    int NumberOfVertex = CentersNumber * DotsNumber;
    Visited.resize(NumberOfVertex, false);

    std::default_random_engine JustRandom (std::chrono::system_clock::now().time_since_epoch().count());
    std::normal_distribution<double> CenterDistribution (0.0, FirstSigma);
    //Making Dots with Normal distribution
    std::vector<Point> OurDots(NumberOfVertex);
//    for(int i = 0; i < CentersNumber; ++i) {
//        std::normal_distribution<double> FirstCoordDistribution (CenterDistribution(JustRandom), SecondSigma);
//        std::normal_distribution<double> SecondCoordDistribution (CenterDistribution(JustRandom), SecondSigma);
//        
//        for(int j = 0; j < DotsNumber; ++j)
//            OurDots[i * DotsNumber + j] = {FirstCoordDistribution(JustRandom), SecondCoordDistribution(JustRandom)};
//    }
    for(int i = 0; i < CentersNumber; ++i) {
        for(int j = 0; j < DotsNumber; ++j) {
            double x, y;
            std::cin >> x >> y;
            OurDots[i * DotsNumber + j] = {x, y};
        }
    }
    
    
    SpanningTree MySpanningTree(NumberOfVertex, OurDots);
    std::vector<std::pair<int, int>> OurAnswer = MySpanningTree.Run();
    
    auto MST = MySpanningTree.MySpanningTree;
    auto allEdges = MySpanningTree.EdgesFixed;
    PerfectMatching Matcher = PerfectMatching(MST, allEdges);
    Matcher.run();
    auto matching = Matcher.matching;
    
    
    std::vector<std::vector<int>> graph(MST.size());
    for(int i = 0; i < graph.size(); ++i) {
        graph[i].resize(graph.size());
    }
    for(int i = 0; i < MST.size(); ++i) {
        for(int j = 0; j < MST[i].size(); ++j) {
            graph[i][MST[i][j].second] = 1;
        }
    }
    for(int i = 1; i < matching.size(); ++i) {
        graph[Matcher.oldIndices[i - 1]][matching[i]] = 1;
    }
    
    auto eCycle = eulerCycle(graph);
    auto ham = eulerToHamilton(eCycle, CentersNumber * DotsNumber);
    
    for(auto elem : matching) {
        std::cout << elem << " ";
    }
    std::cout << " \n iu\n";
    for(auto elem : MST) {
        for(auto edg : elem)
            std::cout << edg.second << " ";
        std::cout << "\n";
    }
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
