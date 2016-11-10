//
//  main.cpp
//  LIS
//
//  Created by Simon Fedotov on 02.03.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>

size_t MaxSequence(std::vector<int> &OurVector, std::vector<int> &WayIndex, std::vector<int> &Prev) {
    std::vector<int> LastElement(OurVector.size() + 1, INT32_MAX);
    LastElement[0] = INT32_MIN;
    for(int i = 0; i < OurVector.size(); ++i) {
        
        int OurPosition = (int)(std::upper_bound(LastElement.begin(), LastElement.end(), OurVector[i]) - LastElement.begin());
        if(LastElement[OurPosition - 1] < OurVector[i] && OurVector[i] < LastElement[OurPosition]) {
            LastElement[OurPosition] = OurVector[i];
            WayIndex[OurPosition] = i;
        }
        Prev[i + 1] = WayIndex[OurPosition - 1];
    }
    int position = 1;
    size_t counter = 0;
    while (LastElement[position] != INT32_MAX && position < LastElement.size()) {
        ++counter;
        ++position;
    }
//    for(int i = 0; i < LastElement.size(); ++i) {
//        std::cout << LastElement[i] << " ";
//    }
//    std::cout << "\n";
    return counter;
}

int main(int argc, const char * argv[]) {
    size_t Size;
    std::cin >> Size;
    std::vector<int> Our(Size);
    std::vector<int> WayIndex(Our.size() + 1, -1);
    std::vector<int> Previous(Our.size() + 1, -1);
    for(int i = 0; i < Size; ++i) {
        int value;
        std::cin >> value;
        Our[i] = value;
    }
    std::cout << "\n";
    int result = MaxSequence(Our, WayIndex, Previous);
    std::cout << result << "\n";
//    for(int i = 0; i < WayIndex.size(); ++i) {
//        std::cout << WayIndex[i] << " ";
//    }
//    std::cout << "\n";
//    for(int i = 0; i < Previous.size(); ++i) {
//        std::cout << Previous[i] << " ";
//    }
    int counter = 1;
    while (WayIndex[counter] >= 0 && counter < WayIndex.size()) {
        ++counter;
    }
    --counter;
    std::vector<int> Answer;
    Answer.push_back(Our[WayIndex[counter]]);
    if(result > 1)
        Answer.push_back(Our[Previous[WayIndex[counter] + 1]]);
    int OurPos = Previous[WayIndex[counter] + 1] + 1;
    while (Previous[OurPos] >= 0) {
        Answer.push_back(Our[Previous[OurPos]]);
        OurPos = Previous[OurPos] + 1;
    }
    for(int i = Answer.size() - 1; i >= 0; --i)
        std::cout << Answer[i] << " ";
    return 0;
}