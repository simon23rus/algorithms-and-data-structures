//
//  main.cpp
//  mergeMultitreaded
//
//  Created by Simon Fedotov on 24.02.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <random>
#include <assert.h>

std::vector<double> generateArray(int numOfElements) {
    std::default_random_engine randomizer;
    std::uniform_real_distribution<double> generator(-0.0051, 0.0023);
    std::vector<double> data;
    data.reserve(numOfElements * sizeof(double));
    for(int i = 0; i < numOfElements; ++i)
        data.push_back(generator(randomizer));
    return data;
}


void mtMerge(std::vector<double> &dataFrom, int leftFirst, int rightFirst, int leftSecond, int rightSecond, std::vector<double> &dataTo, int startTo) {
    //dlina intervalov, nuzhno, chtoby perviy byl bol'shhe
    int first = rightFirst - leftFirst;
    int second = rightSecond - leftSecond;
    
    if(first < second) {
        std::swap(first,second);
        std::swap(leftFirst, leftSecond);
        std::swap(rightFirst, rightSecond);
    }
    if(first <= 0)
        return;
    else {
        int middleFirst = (leftFirst + rightFirst) / 2;
        unsigned int middleSecond;
        middleSecond = dataFrom.size() - (dataFrom.end() - std::lower_bound(dataFrom.begin() + leftSecond, dataFrom.begin() + rightSecond, dataFrom[middleFirst]));
        int middleThird = startTo + (middleFirst - leftFirst) +  (middleSecond - leftSecond);
        dataTo[middleThird] = dataFrom[middleFirst];
        std::thread leftSorter(mtMerge, std::ref(dataFrom), leftFirst, middleFirst, leftSecond, middleSecond, std::ref(dataTo), startTo);
        mtMerge(dataFrom, middleFirst + 1, rightFirst, middleSecond, rightSecond, dataTo, middleThird + 1);
        leftSorter.join();
        
    }
}



void sortMT(std::vector<double> &dataFrom, int leftFrom, int rightFrom, std::vector<double> &dataTo, int leftTo) {
    int pos = rightFrom - leftFrom;
    if(pos <= 0)
        return;
    if(pos == 1) {
        dataTo[leftTo] = dataFrom[leftFrom];
        
    }
    else {
        std::vector<double> temp(dataTo.size());
        int middle = (leftFrom +  rightFrom) / 2;
        int leftMiddle = middle - leftFrom;
        std::thread leftSort(sortMT, std::ref(dataFrom), leftFrom, middle, std::ref(temp), 0);
        sortMT(dataFrom, middle, rightFrom, temp, leftMiddle);
        leftSort.join();
        mtMerge(temp, 0, leftMiddle, leftMiddle, pos, dataTo, leftTo);
    }
}


int main(int argc, const char * argv[]) {
    // insert code here...
    int size;
    std::cin >> size;
    auto vec = generateArray(size);
    auto vec1 = vec;
    auto vec2 = vec;
    std::sort(vec1.begin(), vec1.end());
    sortMT(vec, 0, vec.size(), vec2, 0);
    for(int i = 0; i < vec1.size(); ++i) {
        assert(vec1[i] == vec2[i]);
    }
    std::cout << " \n";
    return 0;
}
