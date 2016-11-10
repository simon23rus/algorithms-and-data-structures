//
//  main.cpp
//  Suggester
//
//  Created by Simon Fedotov on 25.03.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include "SegmentTree.h"
#include <algorithm>



int main(int argc, const char * argv[]) {
    std::vector<Query> Vocabulary;
    int size;
    std::cin >> size;
    for(int i = 0; i < size; ++i) {
        Query ToInsert;
        std::string OurString;
        int OurFreq;
        std::cin >> OurString >> OurFreq;
        ToInsert.Frequency = OurFreq;
        ToInsert . query = OurString;
        Vocabulary.push_back(ToInsert);
    }
    std::sort(Vocabulary.begin(), Vocabulary.end(), QueryCmp);
    for(int i = 0; i < Vocabulary.size(); ++i) {
        Vocabulary[i] . Position = i;
        std::cout << Vocabulary[i] . query << " ";
    }
    std::cout <<  "\n";
    SegmentTree DatTree = SegmentTree(Vocabulary);
    for(int i = 0; i < DatTree.OurTree.size(); ++i) {
        std::cout << DatTree.OurTree[i] << " ";
    }
    std::cout <<  "\n";
    std::string prefix;
    std::cin >> prefix;
    
    int SuggestNumber;
    std::cin >> SuggestNumber;
    
    std::vector<Query> Result = DatTree.PrepareToSuggest(prefix, SuggestNumber);
    for(int i = 0; i < Result.size(); ++i) {
        std::cout << Result[i] . query << " " << Result[i] . Frequency << "\n";
    }


    return 0;
}
