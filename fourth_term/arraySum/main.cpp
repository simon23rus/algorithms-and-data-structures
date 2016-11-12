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
std::vector<double> sums;

int NUM_OF_ELEMENTS = 100000;
double result;

class Adder {
    int numberOfThreads;
public:
    Adder(int numberOfThreads);
    std::vector<double> generateArray();
    std::vector<double> data;
    void add(int threadNumber);
    void other();
    double getResult();
    double sum();
};

double Adder::sum() {
    double ans = 0;
    for(int i = 0; i < data.size(); ++i)
        ans += data[i];
    return ans;
        
}
Adder::Adder(int numberOfThreads) : numberOfThreads(numberOfThreads), data(generateArray()){};

void Adder::other() {
    std::vector<std::thread> threads;
    for(int i = 0; i < numberOfThreads; ++i) {
        threads.push_back(std::thread(&Adder::add, *this, i));
    }
    for(int i = 0; i < numberOfThreads; ++i) {
        threads[i].join();
    }
}

std::vector<double> Adder::generateArray() {
    std::default_random_engine randomizer;
    std::uniform_real_distribution<double> generator(-10000.1, 10000.2);
    std::vector<double> data;
    data.reserve(NUM_OF_ELEMENTS * sizeof(double));
    for(int i = 0; i < NUM_OF_ELEMENTS; ++i)
        data.push_back(generator(randomizer));
    return data;
}

void Adder::add(int threadNumber) {
    for(int i = 0; i < NUM_OF_ELEMENTS / numberOfThreads; ++i) {
//        locker.lock();
        sums[threadNumber] += data[i + threadNumber * (NUM_OF_ELEMENTS / numberOfThreads)];
//        std::cout << "threadNum " << threadNumber  << " elem : " << data[i] << "\n" << result << "\n";
        
//        locker.unlock();
        
//        std::cout << result << "res\n";
//        std::cout << getResult() << " \n";
    }
//    std::cout << "alt " << getResult() << " \n";
}

int main(int argc, const char * argv[]) {
    int numberOfThreads;
    result = 0;
    std::cin >> numberOfThreads;
    sums.resize(numberOfThreads, 0);

    if(numberOfThreads > NUM_OF_ELEMENTS) {
        std::cout << "ERROR THREADS QUANTITY IS BIG ENOUGH\n";
        exit(1);
    }
    Adder sum = Adder(numberOfThreads);
    for(int i = (NUM_OF_ELEMENTS / numberOfThreads) * numberOfThreads; i < NUM_OF_ELEMENTS; ++i)
        result += sum.data[i];
//    for(auto &elem : sum.data) {
//        std::cout << elem << "\n";
//    }
//    std::vector<double> data = sum.generateArray();
    auto start = std::chrono::steady_clock::now();
    sum.other();
    for(int i = 0; i < numberOfThreads; ++i) {
        result += sums[i];
    }
//    std::cout << "SUM " << sum.result << "\n";
    auto end = std::chrono::steady_clock::now();
    std::cout << "time " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001 << "\n";
    std::cout << "RESULT IS " << result << "\n";
    std::cout << sum.sum() << "\n";
    return 0;
}
