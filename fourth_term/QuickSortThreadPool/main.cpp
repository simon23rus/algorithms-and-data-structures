//
//  main.cpp
//  QuickSortThreadPool
//
//  Created by Simon Fedotov on 25.05.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <functional>
#include "threadpool.h"

thread_pool<void> tp(4);

template <typename T>
std::ostream &operator <<(std::ostream& os, const std::vector<T> &vec) {
    os << '[';
    for (auto &it: vec) {
        os << it << ", ";
    }
    os << ']';
    return os;
}

std::mutex mut;

struct qSortTask {
    std::vector<int> &data;
    int left;
    int right;
    int depth;
};

int qsort_partition(std::vector<int> &data, int left, int right) {
    int middle = left + (right-left)/2;
    
    // left <=> middle
    std::swap(data[left], data[middle]);
    
    int current;
    int last = left;
    for (current = left + 1; current < right; ++current) {
        if (data[current] < data[left]) {
            ++last;
            // last <=> current
            std::swap(data[last], data[current]);
        }
    }
    
    // left <=> last
    std::swap(data[left], data[last]);
    
    std::unique_lock<std::mutex> ul(mut);
    std::cout << data << std::endl;
    std::cout << left << "ri" << right << " \n";
    return last;
}

////void sort_parallel(std::vector<int> &data, int left, int right) {
//    if(left >= right) {
//        return;
//    }
//    int pivotPlace = qsort_partition(std::ref(data), left, right);
//    
//    tp.submit(std::bind(sort_parallel, std::ref(data), left, pivotPlace - 1));
//    tp.submit(std::bind(sort_parallel, std::ref(data), pivotPlace + 1, right));
//    
//    std::unique_lock<std::mutex> ul(mut);
//    std::cout << pivotPlace << std::endl;
//}

void quicksort(std::vector<int>& vec, int st, int end) {
    if(st >= end - 1) return;
    
    int position = qsort_partition(vec, st, end);
    auto left = std::bind(quicksort, std::ref(vec), st, position);
    auto right = std::bind(quicksort, std::ref(vec), position + 1, end);
    
    tp.submit(left);
    tp.submit(right);
}


int main(int argc, const char * argv[]) {
    int sizeOfArray;
    std::cin >> sizeOfArray;
    std::vector<int> data;
    
    for(int i = 0; i < sizeOfArray; ++i) {
        int elem;
        std::cin >> elem;
        data.push_back(elem);
    }
    //Все, хотим отсортить этот массив
    
    auto f = std::bind(quicksort, std::ref(data), 0, data.size());
    tp.submit(f);
    tp.joinThreads();
//    tp.shutdown();
    
//    auto f = std::bind(qsort_partition, data, 0, data.size());
//    std::cout << f() << std::endl;
    
    std::cout << data;
    
    return 0;
}
