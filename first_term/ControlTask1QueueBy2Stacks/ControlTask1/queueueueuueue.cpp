//
//  main.cpp
//  ControlTask1
//
//  Created by Simon Fedotov on 15.12.14.
//  Copyright (c) 2014 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <iterator>



struct OurQueue {
    std::vector<std::pair<int, int>> First;
    std::vector<std::pair<int, int>> Second;
    void Push(int Value);
    int Pop();
    int OurMax();
};

void OurQueue :: Push(int Value) {
    int maximum = First.empty() ? Value : std::max(Value, First[First.size() - 1].second);
    First.push_back(std::make_pair(Value, maximum));
    }
    
//    if(First.back() > First.front())
//        std::swap(First.back(), First.front());

int OurQueue:: Pop() {

    if(Second.size() == 0) {
        while (First.size() != 0)   {
            int OurValue = First.back().first;
            First.pop_back();
            int OurMax;
            if(Second.size() == 0) {
                OurMax = OurValue;
            }
            else {
                OurMax = std::max(OurValue, Second.back().second);
            }
            Second.push_back(std::make_pair(OurValue, OurMax));
        }
    }
        int OurResult = Second.back().first;
        Second.pop_back();
        return OurResult;
}

int OurQueue:: OurMax() {
    if(First.size() == 0 || Second.size() == 0) {
        if(First.size() == 0)
            return Second.back().second;
        else
            return First.back().second;
    }
    else
        return std::max(Second.back().second, First.back().second);
}


int main() {
    std::ios_base::sync_with_stdio(false);
    OurQueue Our;
    int OurSize;
    std::cin >> OurSize;
    int OurValue;
    
    for (int i = 0; i < OurSize; ++i) {
        std::cin >> OurValue;
        Our.Push(OurValue);
    }
    
    std::vector<int> OurMaxes;
    
    do
    {
        std::cin >> OurValue;
        if (OurValue != -1)
        {
            OurMaxes.push_back(Our.OurMax());
            Our.Push(OurValue);
            Our.Pop();
        }
    }while (OurValue != -1);
    
    OurMaxes.push_back(Our.OurMax());
    
    for (auto DatMax:OurMaxes)
        std::cout << DatMax << '\n';
//    std::cout << Our.Pop() << " " << Our.Pop() << " " << Our.Pop() << " " << Our.Pop();
    return 0;
}
