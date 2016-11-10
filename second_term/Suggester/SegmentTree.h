//
//  SegmentTree.h
//  Suggester
//
//  Created by Simon Fedotov on 25.03.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//


#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <string>
#include <queue>

struct OurNode {
    int value;
    int delta;
};

struct Query {
    std::string query;
    int Frequency;
    int Position;
};

struct HeapNode {
    int Position;
    int LeftPos;
    int RightPos;
};



bool lowerBoundComp(const Query First, const std::string &prefix) {
    if(First . query.size() == 0)
        return false;
    return ((First . query) < prefix);
}

bool upperBoundComp(const std::string &prefix, const Query First) {
    if(First . query.size() == 0)
        return true;
        
    if(First . query.size() < prefix.size())
        return prefix < First . query;
    return (prefix < First . query.substr(0, prefix.size()) );
}



bool QueryCmp (const Query First, const Query Second) {
    return First . query < Second . query;
}

class SegmentTree {
    public :
    class OurComp;
    bool Compic(const HeapNode First, const HeapNode Second);
    std::vector<Query> Dictionary;
    std::vector<int> OurTree;
    SegmentTree(std::vector<Query> &OurVector);
    std::vector<Query> PrepareToSuggest(std::string &prefix, int NumberOfSugests);
    int CalcMax(int Position, int LeftPos, int RightPos, int Start, int End);
    std::vector <Query> Suggest(int left, int right, int NumberOfSugests);
    
    bool operator()(const HeapNode& First, const HeapNode& Second) {
        return Dictionary[First . Position] . Frequency < Dictionary[Second . Position] . Frequency;
    }
    bool operator()(HeapNode& First, HeapNode& Second) {
        return Dictionary[First . Position] . Frequency < Dictionary[Second . Position] . Frequency;
    }
    
    private :
//    void Push(int Position);
//    void UpdateSegment(int Position, int LeftPos, int RightPos, int Start, int End, int delta);
    void PrepareTree(int Position, int Start, int End);
    int IndexCMP(const int &First, const int& Second);
};

class SegmentTree::OurComp {
    public :
    std::vector<Query> MyDict;
    OurComp(std::vector<Query> &Dictionary) {
        MyDict = Dictionary;
    }
    bool operator()(const HeapNode First, const HeapNode Second) {
        return MyDict[First . Position] . Frequency > MyDict[Second . Position] . Frequency;
    }
};

bool SegmentTree::Compic(const HeapNode First, const HeapNode Second) {
    return Dictionary[First . Position] . Frequency > Dictionary[Second . Position] . Frequency;
}

//
//void SegmentTree::Push(int Position) {
//    if (OurTree[Position].delta != 0) {
//        if(Position * 2 + 2 > OurTree.size() - 1) {
//            OurTree[Position].value += OurTree[Position].delta;
//            OurTree[Position].delta = 0;
//            return;
//        }
//        OurTree[Position * 2 + 1].delta += OurTree[Position].delta;
//        OurTree[Position * 2 + 2].delta += OurTree[Position].delta;
//        OurTree[Position].value += OurTree[Position].delta;
//        OurTree[Position].delta = 0;
//    }
//}
//



std::vector <Query> SegmentTree::Suggest(int left, int right, int NumberOfSugests) {
    std::priority_queue<HeapNode, std::vector<HeapNode>, SegmentTree> OurHeap(*this);
    int MaxPosition = CalcMax(0, 0, Dictionary.size() - 1, left, right);
    std::vector<Query> OurResult;
    HeapNode DatNode;
    DatNode . LeftPos = left;
    DatNode . RightPos = right;
    DatNode . Position = MaxPosition;
    OurHeap.push(DatNode);
    for(int i = 0; i < std::min(NumberOfSugests, 1 + right - left); ++i) {
        HeapNode OurNode = OurHeap.top();
        OurHeap.pop();
        
        OurResult.push_back(Dictionary[OurNode.Position]);
        
        
        HeapNode ToPush;
        int NewLeft;
        int NewRight;
        if(left <= OurNode.Position - 1) {
            NewLeft = CalcMax(0, 0, Dictionary.size() - 1, OurNode.LeftPos, OurNode.Position - 1);
            ToPush = {NewLeft, OurNode.LeftPos, OurNode.Position - 1};
            OurHeap.push(ToPush);
        }
        
        
        if(right >= OurNode.Position + 1) {
            NewRight = CalcMax(0, 0, Dictionary.size() - 1, OurNode.Position + 1, OurNode.RightPos);
            ToPush = {NewRight, OurNode.Position + 1, OurNode.RightPos};
            OurHeap.push(ToPush);
        }
        
    
        

        
    }
    
    return OurResult;
}

std::vector<Query> SegmentTree::PrepareToSuggest(std::string &prefix, int NumberOfSugests) {
    int start = std::lower_bound(Dictionary.begin(), Dictionary.end(), prefix, lowerBoundComp) - Dictionary.begin();
    int end = std::upper_bound(Dictionary.begin(), Dictionary.end(), prefix, upperBoundComp) - Dictionary.begin() - 1;
    
    return Suggest(start, end, NumberOfSugests);
}


int SegmentTree::IndexCMP(const int &First, const int& Second) {
    if(Dictionary[First] . Frequency > Dictionary[Second] . Frequency)
        return First;
    else
        return Second;
}

void SegmentTree::PrepareTree(int Position, int Start, int End) {
    if(Start == End) {
        OurTree[Position] = Dictionary[Start] . Position;
    }
    else {
        int middle = (Start + End) / 2;
        PrepareTree(Position * 2 + 1, Start , middle);
        PrepareTree(Position * 2 + 2, middle + 1, End);
        OurTree[Position] = IndexCMP(OurTree[Position * 2 + 1] , OurTree[Position * 2 + 2]);
    }
}

SegmentTree::SegmentTree(std::vector<Query> &OurVector){
    int size = OurVector.size();
    int ModifiedSize = pow(2, ceil(log2(size)));
    int NewSize = ceil(log2(size)) + 1;
    NewSize = pow(2, NewSize) - 1;
    OurTree.resize(NewSize);
    Dictionary = OurVector;
    for(int i = Dictionary.size(); i < ModifiedSize; ++i) {
        Query BAD_QUERY;
        BAD_QUERY . Frequency = -1;
        BAD_QUERY . Position = ModifiedSize - 1;
        Dictionary.push_back(BAD_QUERY);

    }
    PrepareTree(0, 0, Dictionary.size() - 1);

}

int SegmentTree::CalcMax(int Position, int LeftPos, int RightPos, int Start, int End) {
    if(Start > End || Position > OurTree.size() - 1)
        return Dictionary.size() - 1;
    if(LeftPos == Start && RightPos == End) {
        return OurTree[Position];
    }
    int middle = (LeftPos + RightPos) / 2;
    return
    IndexCMP(CalcMax(Position * 2 + 1, LeftPos, middle, Start, std::min(middle, End)) ,
     CalcMax(Position * 2 + 2, middle + 1, RightPos, std::max(middle + 1, Start), End));
}