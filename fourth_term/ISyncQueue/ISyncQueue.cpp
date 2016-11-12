//
//  main.cpp
//  ISyncQueue
//
//  Created by Simon Fedotov on 15.03.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <random>
#include <limits>
#include <assert.h>


template <typename T>
class ISyncQueue {
private:
    size_t elementsBound;
    std::queue <T> synchronizedQueue;
    std::mutex queueLocker;
    std::condition_variable nonEmpty;
    std::condition_variable nonFull;
    
    
public:
    
    ISyncQueue(const int &elementsBound) : elementsBound(elementsBound) {};
    
    std::queue<T> getData() {
        return synchronizedQueue;
    }
    
    T back() {
        std::unique_lock<std::mutex> footLocker(queueLocker);
        return synchronizedQueue.back();
    }
    
    T front() {
        std::unique_lock<std::mutex> footLocker(queueLocker);
        return synchronizedQueue.front();
    }
    
void push(const T &toAdd) {
    std::unique_lock<std::mutex> footLocker(queueLocker);
    while (synchronizedQueue.size() == elementsBound) {
        nonFull.wait(footLocker);
    }
    synchronizedQueue.push(toAdd);
    for(int i = 0; i < synchronizedQueue.size(); ++i)
    nonEmpty.notify_all();
}
    
void pop() {
    std::unique_lock<std::mutex> footLocker(queueLocker);
    while (synchronizedQueue.size() == 0) {
        nonEmpty.wait(footLocker);
    }
    auto elem = (synchronizedQueue.front());
    synchronizedQueue.pop();
    nonFull.notify_all();
    return;
}

    
};


template <typename T>
std::vector<T> generateData(int elementsNumber) {
    std::default_random_engine randomizer;
    std::uniform_real_distribution<T> generator(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    std::vector<T> data;
    data.reserve(elementsNumber * sizeof(T));
    for(int i = 0; i < elementsNumber; ++i)
        data.push_back(generator(randomizer));
    return data;
}


template <typename T>
void pusher(const T &toPush, ISyncQueue<T> &queue) {
    queue.push(toPush);
}

template <typename T>
void poper(ISyncQueue<T> &queue) {
    queue.pop();
}

template <typename T>
bool sizeTest(int insertionNumber, int firstDeletionNumber, int secondDeletionNumber, int elementsBound, size_t realAnswer) {
    std::vector<std::thread> threads;
    ISyncQueue<int> threadSafeQueue(elementsBound);
    auto data = generateData<T>(insertionNumber);
    for(int i = 0; i < firstDeletionNumber; ++i) {
        threads.push_back(std::thread(poper<T>, std::ref(threadSafeQueue)));
    }
    for(int i = 0; i < insertionNumber; ++i) {
        threads.push_back(std::thread(pusher<T>, data[i],std::ref(threadSafeQueue)));
    }
    for(int i = 0; i < secondDeletionNumber; ++i) {
        threads.push_back(std::thread(poper<T>, std::ref(threadSafeQueue)));
    }
    for(auto &thread : threads)
        thread.join();
    
    auto res = threadSafeQueue.getData();
    size_t resSize = res.size();
    assert(resSize == realAnswer);
    std::cout << res.size() << "saez\n";
    for(int i = 0 ; i < resSize; ++i) {
        std::cout << res.front() << " ";
        res.pop();
    }

    return true;
}

int main(int argc, const char * argv[]) {
    if(sizeTest<int>(10, 4, 5, 5, 2))
        std::cout << "\n\n----------_-_-_-_-_-_OK-_-_-_-_-_-_---------\n\n";
    
    return 0;
}
