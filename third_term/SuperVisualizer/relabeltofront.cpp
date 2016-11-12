//
//  main.cpp
//  maxFlowVKub
//
//  Created by Simon Fedotov on 28.09.15.
//  Copyright © 2015 Simon23Rus. All rights reserved.
//


#include "relabeltofront.h"

#define source 0

size_t Net::getVertexNumber() {
    return vertexNumber;
}

Net::Net(std::vector<Edge> &ourEdges, const size_t &nodeNumber, std::vector<std::vector<size_t>> &matrix, const bool &mode
) : edges(ourEdges), vertexNumber(nodeNumber), IsVisualizeNeeded(mode) {
    listOfEdges = matrix;
    excess.resize(vertexNumber, 0);
    excess[source] = INT64_MAX;
    height.resize(vertexNumber, 0);
    height[source] = vertexNumber;
    oldHeight = height;
    for(size_t i = 0; i < vertexNumber; ++i)
        positionInSequence.push_back(listOfEdges[i].size() == 0 ? 0 : listOfEdges[i][0]);
}

void Net::Push(const size_t &edgeToPushNumber, const size_t &reverseEdge) {
    long long amountToPush = std::min(excess[edges[edgeToPushNumber].from], edges[edgeToPushNumber].capacity - edges[edgeToPushNumber].flow);
//    std::cout << edgeToPushNumber << " " << reverseEdge << " " << edges[edgeToPushNumber].from << " " << excess[edges[edgeToPushNumber].from] << " " << edges[edgeToPushNumber].capacity - edges[edgeToPushNumber].flow << "\n";
    edges[edgeToPushNumber].flow += amountToPush;
    edges[reverseEdge].flow = -edges[edgeToPushNumber].flow;
    excess[edges[edgeToPushNumber].from] -= amountToPush;
    excess[edges[edgeToPushNumber].to] += amountToPush;
}

void Net::toFront(std::list<size_t>::iterator &indexToMove) {
    vertexQueue.push_front(*indexToMove);
    vertexQueue.erase(indexToMove);
    indexToMove = vertexQueue.begin();
}

void Net::Lift(const size_t &from) {
    size_t minimalHeight = std::numeric_limits<size_t>::max();
//    std::cout << "B4 " << height[from];
    for(auto &pos : listOfEdges[from]) {
        if(edges[pos].capacity - edges[pos].flow > 0)
            minimalHeight = std::min(minimalHeight, height[edges[pos].to]);
    }
    height[from] = minimalHeight + 1;
//    std::cout << "AFTA " << height[from];
}

size_t Net::Discharge(const size_t &from) {
    while (excess[from] > 0) {
        size_t to = positionInSequence[from];
        if(to < listOfEdges[from].size()) {
            if(edges[listOfEdges[from][to]].capacity - edges[listOfEdges[from][to]].flow > 0 && height[from] == height[edges[listOfEdges[from][to]].to] + 1) {
                Push(listOfEdges[from][to], (listOfEdges[from][to] % 2 == 0)? listOfEdges[from][to] + 1 : listOfEdges[from][to] - 1);
                if(edges[listOfEdges[from][to]].capacity - edges[listOfEdges[from][to]].flow == 0) {
                    ++positionInSequence[from];
                    return listOfEdges[from][to];
                }
                if(excess[from] == 0)
                    return listOfEdges[from][to];
                if(IsVisualizeNeeded)
                    return listOfEdges[from][to];
            }
            else
                ++positionInSequence[from];
        }
        else {
            Lift(from);
            positionInSequence[from] = 0;
            if(IsVisualizeNeeded)
                return 0;
        }
    }
    return INT16_MAX;
}

bool Net::RelabelToFront() {
    for(auto &to : listOfEdges[source]) {
        if(edges[to].capacity - edges[to].flow != 0) {
            if(IsVisualizeNeeded) {
                Push(to, (to % 2 == 0)? to + 1 : to - 1); //tk rebra dobavlyayutsa po 2
                return true;
            }
        }
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

void Net::DoInitialize(const size_t &callNumber) {
    if(callNumber == 1) {
        for(size_t to = source + 1; to < vertexNumber - 1; ++to)
            vertexQueue.push_back(to);
    }
    for(size_t to = listOfEdges[source].size() - callNumber; to <  listOfEdges[source].size(); ++to) {
        size_t positionInEdgeList = listOfEdges[source][to];
        if(edges[positionInEdgeList].capacity - edges[positionInEdgeList].flow != 0) {
                Push(positionInEdgeList, (positionInEdgeList % 2 == 0)? positionInEdgeList + 1 : positionInEdgeList - 1); //tk rebra dobavlyayutsa po 2
                return;
        }
    }
}

AlgoStep Net::DoQueueStep(std::list<size_t>::iterator &currentPoint) {
    std::cout << *currentPoint << "Эх\n";
    while(currentPoint != vertexQueue.end()) {
        size_t thisNode = *currentPoint;
//        for(auto &th : vertexQueue)
//            std::cout << th << " ";
//        std::cout << "\n " << thisNode << "\n";
        if(excess[thisNode] == 0) {
            ++currentPoint;
            continue;
        }
//        std::cout << thisNode << "obidno\n";
        size_t oldHeightOfVertex = oldHeight[thisNode];
        size_t notSoOldHeight = height[thisNode];
        size_t pushingEdgeNumber = Discharge(thisNode);
        std::cout << pushingEdgeNumber << "\n\n"; //mozhet ne viyty
        if(IsVisualizeNeeded && excess[thisNode] != 0) {//opasno mb massiv height v nachale
            if(notSoOldHeight != height[thisNode]) {
                std::cout << "DEBUG " << thisNode << " " << height[thisNode] << " " << excess[thisNode] << "\n";
                return {thisNode, pushingEdgeNumber, 'l'};
            }
            else {
                std::cout << "3apushil " << thisNode << " " << pushingEdgeNumber << "\n";
                return {thisNode, pushingEdgeNumber, 'p'};
            }
        }
        if(oldHeightOfVertex != height[thisNode]) {
            oldHeight[thisNode] = height[thisNode];
            std::cout << "THISNODE IS " << thisNode << "____" << excess[thisNode] << "\n";
            toFront(currentPoint);
            ++currentPoint;
            return {thisNode, pushingEdgeNumber, 'p'};
        }
        else {
            ++currentPoint;
            return {thisNode, pushingEdgeNumber, 'p'};
        }
        return {thisNode, pushingEdgeNumber, 'p'};
    }
    return {0, 0, '-'};
}


bool Net::DoAlgoStep() {
    return false;
}

std::list<size_t>::iterator Net::getQueueBeginning() {
    return vertexQueue.begin();
}

size_t Net::getNumberOfEdgesFromSource() {
    return listOfEdges[source].size();
}

size_t Net::getVertexHeight(const size_t &vertex) {
    return height[vertex];
}

