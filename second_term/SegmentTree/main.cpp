//
//  main.cpp
//  SegmentTree
//
//  Created by Simon Fedotov on 04.03.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

struct OurNode {
    int value;
    int delta;
};


void Push(std::vector<OurNode> &OurTree, int Position) {
    if (OurTree[Position].delta != 0) {
        if(Position * 2 + 2 > OurTree.size() - 1) {
            OurTree[Position].value += OurTree[Position].delta;
            OurTree[Position].delta = 0;
            return;
        }
        OurTree[Position * 2 + 1].delta += OurTree[Position].delta;
        OurTree[Position * 2 + 2].delta += OurTree[Position].delta;
        OurTree[Position].value += OurTree[Position].delta;
        OurTree[Position].delta = 0;
    }
}


void BuildTree(std::vector<int> &OurVector, std::vector<OurNode> &OurTree, int Position, int Start, int End){
        if(Start == End) {
            OurTree[Position].value = OurVector[Start];
            OurTree[Position].delta = 0;
        }
        else {
            int middle = (Start + End) / 2;
            BuildTree(OurVector, OurTree,  Position * 2 + 1, Start , middle);
            BuildTree(OurVector, OurTree,  Position * 2 + 2, middle + 1, End);
            OurTree[Position].value = std::min(OurTree[Position * 2 + 1].value , OurTree[Position * 2 + 2].value);
            OurTree[Position].delta = 0;
        }
}

int CalcMin(std::vector<int> &OurVector, std::vector<OurNode> &OurTree, int Position, int LeftPos, int RightPos, int Start, int End) {
    if(Start > End)
        return INT32_MAX;
    Push(OurTree, Position);
//    OurTree[(Position - 1) / 2].value = std::min(OurTree[((Position - 1) / 2 )* 2 + 1].value + OurTree[((Position - 1) / 2 )* 2 + 1].delta, OurTree[((Position - 1) / 2)* 2 + 2].value + OurTree[((Position - 1) / 2)* 2 + 2].delta);
    if(LeftPos == Start && RightPos == End) {
        return OurTree[Position].value;
    }
    int middle = (LeftPos + RightPos) / 2;
    return
    std::min
    (CalcMin(OurVector, OurTree, Position * 2 + 1, LeftPos, middle, Start, std::min(middle, End)) ,
    CalcMin(OurVector, OurTree, Position * 2 + 2, middle + 1, RightPos, std::max(middle + 1, Start), End));
}



void UpdateSingle(std::vector<int> &OurTree, int OurPosition , int UpdatePosition, int LeftPos, int RightPos, int delta) {
    if (LeftPos == RightPos)
        OurTree[OurPosition] += delta;
    else {
        int middle = (LeftPos + RightPos) / 2;
        if (UpdatePosition <= middle)
            UpdateSingle(OurTree, OurPosition * 2 + 1, UpdatePosition, LeftPos, middle, delta);
        else
            UpdateSingle(OurTree, OurPosition * 2 + 2, UpdatePosition, middle + 1, RightPos, delta);
        OurTree[OurPosition] = OurTree[OurPosition * 2 + 1] + OurTree[OurPosition * 2 + 2];
    }
}

void UpdateSingleMin(std::vector<OurNode> &OurTree, int OurPosition , int UpdatePosition, int LeftPos, int RightPos, int delta) {
    if (LeftPos == RightPos)
        OurTree[OurPosition].value += delta;
    else {
        int middle = (LeftPos + RightPos) / 2;
        if (UpdatePosition <= middle)
            UpdateSingleMin(OurTree, OurPosition * 2 + 1, UpdatePosition, LeftPos, middle, delta);
        else
            UpdateSingleMin(OurTree, OurPosition * 2 + 2, UpdatePosition, middle + 1, RightPos, delta);
        OurTree[OurPosition].value = std::min(OurTree[OurPosition * 2 + 1].value , OurTree[OurPosition * 2 + 2].value);
    }
}




//debugIt
void UpdateSegment(std::vector<OurNode> &OurTree, int Position, int LeftPos, int RightPos, int Start, int End, int delta) {
    if(Start > End)
        return;
    if(LeftPos == Start && RightPos == End) {
        OurTree[Position].delta += delta;
        Push(OurTree, Position);
    }
    else {
        Push(OurTree, Position);
        int middle = (LeftPos + RightPos) / 2;
        UpdateSegment(OurTree, Position * 2 + 1, LeftPos, middle, Start, std::min(middle, End), delta);
        UpdateSegment(OurTree, Position * 2 + 2, middle + 1, RightPos, std::max(middle + 1, Start), End, delta);
       OurTree[Position].value=std::min(OurTree[Position * 2 + 1].value +
                            OurTree[Position * 2 + 1].delta,OurTree[Position * 2 + 2].value + OurTree[Position * 2 + 2].delta);

    }
}



int main(int argc, const char * argv[]) {
    int size;
    std::cin >> size;
    std::vector<int> OurVector;
    std::vector<OurNode> OurTree;
    for(int i = 0; i < size; ++i) {
        OurVector.push_back(size - i);
    }
    int ModifiedSize = pow(2, ceil(log2(size)));
    int NewSize = ceil(log2(size)) + 1;
    NewSize = pow(2, NewSize) - 1;
    OurVector.resize(ModifiedSize, INT32_MAX);
    OurTree.resize(NewSize);
    for(int i = NewSize - size; i < NewSize; ++i) {
        OurTree[i].value = INT32_MAX;
    }
    BuildTree(OurVector, OurTree, 0, 0, (int)OurVector.size() - 1);
    for(int i = 0; i < OurTree.size(); ++i) {
        std::cout << OurTree[i].value << " ";
    }
    std::cout << "\n";
    for(int i = 0; i < OurVector.size(); ++i) {
        std::cout << OurVector[i] << " ";
    }
    std::cout << "\n";

    std::cout << "\n";

//    for(int i = 0; i < OurVector.size(); ++i) {
//        std::cout << CalcMin(OurVector, OurTree, 0, 0, OurVector.size() -1 , 0, i) << "\n";
//    }
    std::cout << "\n";
//    UpdateSingleMin(OurTree, 0, 2, 0, OurVector.size() - 1, -100);
    UpdateSegment(OurTree, 0, 0, OurVector.size() - 1, 1, 3, -7);
    
    
    for(int i = 0; i < OurVector.size(); ++i) {
        std::cout << CalcMin(OurVector, OurTree, 0, 0, OurVector.size() -1 , 0, i) << "\n";
    }
    
    
    std::cout << "\n";
    for(int i = 0; i < OurTree.size(); ++i) {
        std::cout << OurTree[i].value << " delta = " << OurTree[i].delta << " \n";
    }
    
    UpdateSegment(OurTree, 0, 0, OurVector.size() - 1, 3, 4, -7);
    std::cout << "\n";

    
    for(int i = 0; i < OurTree.size(); ++i) {
        std::cout << OurTree[i].value << " delta " << OurTree[i].delta << " \n";
    }
    
    
    for(int i = 0; i < OurVector.size(); ++i) {
        std::cout << CalcMin(OurVector, OurTree, 0, 0, OurVector.size() -1 , 0, i) << "\n";
    }
    
    
    std::cout << "\nanswer\n";

    
    std::cout << "\n";
    
    return 0;
}
