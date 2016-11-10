//
//  main.cpp
//  QuantileTask
//
//  Created by Simon Fedotov on 17.11.14.
//  Copyright (c) 2014 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>


template <typename DatType, typename Comparator>

void SiftDown(std::vector<DatType>&OurHeap, size_t OurNode, const Comparator &comparer = Comparator())
{
    while (2 * OurNode + 1 < OurHeap.size()) {
        size_t left, right;
        size_t smallest;
        left = 2 * OurNode + 1, right = 2 * OurNode + 2;
        if(comparer(OurHeap[OurNode] , OurHeap[left]))
            smallest = OurNode;
        else
            smallest = left;
        if(right < OurHeap.size())
        {
            if(comparer(OurHeap[right] , OurHeap[smallest]))
                smallest = right;
        }
        if(smallest != OurNode) {
            std::swap(OurHeap[OurNode], OurHeap[smallest]);
            OurNode = smallest;
            smallest = OurNode;
        }
        else
            break;
    }
}


template <typename DatType, typename Comparator>

void SiftUp(std::vector<DatType>&OurHeap, size_t OurNode, const Comparator &comparer = Comparator())
{
    while(OurNode != 0) {
        size_t parent;
        parent = (OurNode + 1) / 2 - 1;
        if(comparer(OurHeap[OurNode] , OurHeap[parent])) {
            std::swap(OurHeap[OurNode], OurHeap[parent]);
            OurNode = parent;
        }
        else
            break;
    }
}


template <typename DatType>

void QuantilePrint(double Quantile, std::vector<DatType> &OurVector, std::vector<DatType> &OurAnswers, std::vector<DatType> &DatMinHeap, std::vector<DatType> &DatMaxHeap) {
    double OurQuantile = 0;
    DatMinHeap.push_back(OurVector.front());
    DatMaxHeap.push_back(OurVector.front());
    std::cout << OurVector.front() << " \n";
    for(size_t i = 1; i < OurVector.size(); i++) {
        size_t OurPosition = (size_t)(Quantile * i);
        size_t OurNum = OurVector[i];
        if(DatMinHeap.empty() || OurNum <= DatMinHeap.front())
            DatMinHeap.push_back(OurNum);
        else
            DatMaxHeap.push_back(OurNum);
        
        if(OurPosition == OurQuantile) {
            if(DatMinHeap.size() - 1 == OurPosition) {
                SiftUp(DatMaxHeap, DatMaxHeap.size() - 1, std::less<int>());
                
            }
            else {
                std::swap(DatMinHeap.front(), DatMinHeap.back());
                DatMaxHeap.push_back(DatMinHeap.back());
                DatMinHeap.pop_back();
                SiftDown(DatMinHeap, 0, std::greater<int>());
                DatMaxHeap.front() = DatMinHeap.front();
                SiftUp(DatMaxHeap ,DatMaxHeap.size() - 1, std::less<int>());
                
            }
        }
        else {
            if(DatMinHeap.size() - 1 == OurPosition) {
                SiftUp(DatMinHeap, DatMinHeap.size() - 1, std::greater<int>());
                OurQuantile = OurPosition;
                
            }
            else {
                std::swap(DatMaxHeap.front(), DatMaxHeap.back());
                DatMinHeap.push_back(DatMaxHeap.back());
                DatMaxHeap.pop_back();
                SiftDown(DatMaxHeap, 0, std::less<int>());
                DatMinHeap.front() = DatMaxHeap.front();
                SiftUp(DatMinHeap, DatMinHeap.size() - 1, std::greater<int>());
                OurQuantile = OurPosition;
                
            }
        }
        
        OurAnswers.push_back(DatMinHeap.front());
        
    }

}



int main() {
    std::ios_base::sync_with_stdio(false);
    size_t num;
    std:: cin >> num;
    double quantile;
    std::cin >> quantile;
    std::vector<int> DatVector;
    std::vector<int> OurHeapMin;
    std::vector<int> OurHeapMax;
    std::vector<int> OurQuantiles;
    for(size_t i = 0; i < num; i++) {
        int x;
        std::cin >> x;
        DatVector.push_back(x);
    }
    QuantilePrint(quantile, DatVector, OurQuantiles, OurHeapMin, OurHeapMax);
    for(size_t i = 0; i < OurQuantiles.size(); ++i)
        std::cout << OurQuantiles[i] << "\n";
    return 0;
}
