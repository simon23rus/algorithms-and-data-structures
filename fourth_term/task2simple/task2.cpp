//
//  main.cpp
//  Sem1task2
//
//  Created by Simon Fedotov on 22.02.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>

std::mutex locker;
double result;
std::vector<double> sums;
std::vector<double> data;
std::vector<double> times;

int NUM_OF_ELEMENTS = 100000000;

std::vector<double> generateArray() {
    std::default_random_engine randomizer;
    std::uniform_real_distribution<double> generator(-0.0051, 0.0023);
    std::vector<double> data;
    data.reserve(NUM_OF_ELEMENTS * sizeof(double));
    for(int i = 0; i < NUM_OF_ELEMENTS; ++i)
        data.push_back(generator(randomizer));
    return data;
}

void add(int threadNumber, int numberOfThreads) {
    sums[threadNumber] = std::accumulate(data.begin() + threadNumber * (NUM_OF_ELEMENTS / numberOfThreads), data.begin() + (threadNumber + 1) * (NUM_OF_ELEMENTS / numberOfThreads), 0.0);
}

void measure(int numberOfThreads) {
    sums.clear();
    sums.resize(numberOfThreads, 0);
    
    if(numberOfThreads > NUM_OF_ELEMENTS) {
        std::cout << "ERROR THREADS QUANTITY IS BIG ENOUGH\n";
        exit(1);
    }
    result = std::accumulate(data.begin() + (NUM_OF_ELEMENTS / numberOfThreads) * numberOfThreads, data.end(), 0);
    std::vector<std::thread> threads;
    auto start = std::chrono::steady_clock::now();
    for(int i = 0; i < numberOfThreads; ++i) {
        threads.push_back(std::thread(add, i, numberOfThreads));
    }
    for(int i = 0; i < numberOfThreads; ++i) {
        threads[i].join();
    }
    auto end = std::chrono::steady_clock::now();
    result = std::accumulate(sums.begin(), sums.end(), result);
    times[numberOfThreads - 1] += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001;
    double ans = std::accumulate(data.begin(), data.end(), 0.0);
    std::cout << result << " " << ans << " equals?\n";
}

int main(int argc, const char * argv[]) {
    data = generateArray();
    double summa = 0, summa2 = 0;
    for(auto elem : data)
        summa2 += elem;
    
    summa = std::accumulate(data.begin(), data.end(), 0.0);
    std::cout << "summaa " << summa << " " << summa2 <<   "\n";
    times.resize(21, 0);
    result = 0;
//    for(int i = 1; i <= 20; ++i) {
//        std::cout << i << "igogo\n\n";
//        for(int j = 0; j < 10; ++j) {
//            measure(i);
//            result = 0;
//        }
//    }
    int num;
    std::cin >> num;
    measure(num);
    std::cout <<  "___________SUMMARY_____________\n";
    int q = 0;
    for(auto elem : times) {
        ++q;
        std::cout << q << " " << elem / 1.0 <<  "\n";
    }
    return 0;
}
