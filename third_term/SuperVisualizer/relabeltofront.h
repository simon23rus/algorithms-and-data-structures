#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <limits>

//action = p - push, l - lift



struct Edge {
    size_t from;
    size_t to;
    long long capacity;
    long long flow;
};

struct AlgoStep {
    size_t affectedNode;
    size_t pushingEdgeNumber;
    char action;
};

class Net {
public:
    std::vector<Edge> edges;
    std::vector<std::vector<size_t>> listOfEdges;
    std::vector<long long> excess;
    std::vector<size_t> height;
    std::vector<size_t> oldHeight;
    std::vector<size_t> positionInSequence;
    std::list<size_t> vertexQueue;
    size_t vertexNumber;
    bool IsVisualizeNeeded;

    void Push(const size_t &edgeToPush, const size_t &reverseEdge);
    void Lift(const size_t &from);
    size_t Discharge(const size_t &from);
    void toFront(std::list<size_t>::iterator &indexToMove);
public:
    void DoInitialize(const size_t &callNumber);
    AlgoStep DoQueueStep(std::list<size_t>::iterator &currentPoint);
    bool DoAlgoStep();
    bool RelabelToFront();
    size_t getVertexNumber();
    size_t getVertexHeight(const size_t &vertex);
    std::list<size_t>::iterator getQueueBeginning();
    size_t getNumberOfEdgesFromSource();
    Net() = default;
    Net(std::vector<Edge> &ourEdges, const size_t &nodeNumber, std::vector<std::vector<size_t>> &matrix, const bool &mode = false
);
};


