//
//  main.cpp
//  Fenwick
//
//  Created by Simon Fedotov on 03.03.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>



class FenwickTree {
    private :
    std::vector<int> OurTree;
    void FenwickAND(int &OurPosition);
    void FenwickOR(int &OurPosition);
    public :
    FenwickTree(const int &size) {
        OurTree.assign(size, 0);
    }
    void Update(const int &Position, const int &delta);
    int Sum(const int &Position);
    int SegmentSum(const int &StartPosition, const int &EndPosition);
};

void FenwickTree::FenwickAND(int &OurPosition) {
     OurPosition = (OurPosition & (OurPosition + 1)) - 1;
}

void FenwickTree::FenwickOR(int &OurPosition) {
    OurPosition |= (OurPosition + 1);
}

int FenwickTree::Sum(const int &Position) {
    int NewPos = Position;
    int OurRes = 0;
    for(; NewPos >= 0; FenwickAND(NewPos)) {
        OurRes += this -> OurTree[NewPos];
    }
    return OurRes;
}

int FenwickTree::SegmentSum(const int &StartPosition, const int &EndPosition) {
    return Sum(EndPosition) - Sum(StartPosition - 1);
}



void FenwickTree::Update(const int &Position, const int &delta) {
    int NewPos = Position;
    for(; NewPos < this -> OurTree.size(); FenwickOR(NewPos)) {
        this -> OurTree[NewPos] += delta;
    }

}

int main() {
    
        std::vector<int> OurVector;
        int size;
        std::cin >> size;
        FenwickTree OurTree = FenwickTree(size);
        for(int i = 0; i < size; ++i) {
            int x;
            std::cin >> x;
            OurVector.push_back(x);
        }
        int myAnswer = 0;
        for(int Position = 0; Position < size; ++Position) {
            myAnswer += Position - OurTree.Sum(OurVector[Position] - 1);
            OurTree.Update(OurVector[Position] - 1, 1);
        }
        std::cout << myAnswer;
    
    return 0;
}
