//
//  main.cpp
//  Sem5MergeTuned
//
//  Created by Simon Fedotov on 13.10.14.
//  Copyright (c) 2014 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <iterator>
#include <chrono>
#include <ctime>
#include <vector>
#include <cmath>
#include <typeinfo>

using namespace std;

template < typename FSourceIterator, typename SSourceIterator,
typename TargetIterator >

TargetIterator Merge(FSourceIterator first_begin, FSourceIterator first_end,
                     SSourceIterator second_begin, SSourceIterator second_end,
                     TargetIterator target) {
    while (first_begin != first_end &&
           second_begin != second_end) {
        if(*first_begin <= *second_begin) {
            //*(target++) = *(first_begin++);
            *target = *first_begin;
            ++target;
            ++first_begin;
        }
        else {
            //*(target++) = *(second_begin++);
            *target = *second_begin;
            ++target;
            ++second_begin;
        }
    }
    target = std::copy(first_begin, first_end, target);
    target = std::copy(second_begin, second_end, target);
    return target;
}


template <typename Iterator ,typename OutputIterator>

void MergeSubarrays(Iterator begin, Iterator end, OutputIterator target, ptrdiff_t chunk_size) {
        for(Iterator first_begin = begin;
            first_begin < end;
            first_begin += std::min<size_t>(2 * chunk_size, end - first_begin)) {
            Iterator second_begin = first_begin + std::min<size_t>(chunk_size, end - first_begin);
            Iterator second_end = second_begin + std::min<size_t>(chunk_size, end - second_begin);
            target = Merge(first_begin, second_begin, second_begin, second_end, target);
        }
    }

template <typename Iterator>

void MergeSort(Iterator begin, Iterator end) {
    std::vector<typename std::iterator_traits<Iterator>::value_type> buffer(end - begin);
    ptrdiff_t chunk_size = 1;
    while (chunk_size < end - begin) {
        MergeSubarrays(begin, end, buffer.begin(), chunk_size);
        chunk_size *= 2;
        if(chunk_size < end - begin)
            MergeSubarrays(buffer.begin(), buffer.end(), begin, chunk_size);
        else
            std::copy(buffer.begin(), buffer.end(), begin);
        chunk_size *= 2;
    }
}

int main() {
    vector <int> really1, really2;
    int n; cin >> n;
    srand(time(0));
    for(int i = 0; i < n; i++) {
        int x;
        x = rand() % 10 + 1;
        really1.push_back(x);
    }
    vector <int> target(really1.size() + really2.size());
    auto start = std::chrono::steady_clock::now();
    MergeSort(really1.begin(), really1.end());
    auto end = std::chrono::steady_clock::now();
    //for(int i = 0; i < really1.size(); i++) cout << really1[i] << " ";
    cout << endl << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001 << " seconds";
    return 0;
}

