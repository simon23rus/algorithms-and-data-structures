//
//  main.cpp
//  futex
//
//  Created by Simon Fedotov on 14.03.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <random>
#include <chrono>
#include <string>
//true - zanyat
//false - svoboden


class UnlockException: public std::exception {
    const char* what() const throw(){
        return "Error found in unlock(). futex has alreade unlocked or you've tried to unlock it from the other thread";
    }
};

class DeadlockException: public std::exception {
    const char* what() const throw(){
        return "You entered Deadlock Empire :(  ";
    }
};


class futex {
private:
    std::atomic<std::thread::id> synchronizer;
public:
    futex() : synchronizer(static_cast<std::thread::id>(0)) {};
    void lock() {
        auto threadNumber = std::this_thread::get_id();
        auto unlocked = static_cast<std::thread::id>(0);
        while(synchronizer.compare_exchange_strong(unlocked, threadNumber) == false) {
            unlocked = static_cast<std::thread::id>(0); // т.к. если у нас там было i, то оно запишется в unlocked
        }
    }
    
    void unlock() {
        auto threadNumber = std::this_thread::get_id();
        if(synchronizer.compare_exchange_strong(threadNumber, static_cast<std::thread::id>()) == true) { //znachit, chto on bil zanyat
        }
        else {
            //EXCEPTIOn
            throw UnlockException();
//            std::cout << "[¯\_(ツ)_/¯]WORST BEHAVIOUR[¯\_(ツ)_/¯]\n";
        }
    }
    
    bool try_lock() {
        auto unlocked = static_cast<std::thread::id>(0);
        auto threadNumber = std::this_thread::get_id();
        if(synchronizer.compare_exchange_strong(unlocked, threadNumber) == true) { //znachit, chto on bil svoboden
//            std::cout << "_-_-_-_[futex has successfully locked by [" << threadNumber << "] thread " << " ]_-_-_-_\n";
            return true;
        }
        else if(synchronizer.compare_exchange_strong(threadNumber, unlocked) == true) {//DEADLOCK
            throw DeadlockException();
            //std::cout << "WE've got a DEADLOCK EMPIRE\n";
            return false;
        }
        else {
//            std::cout << "{}[]Can't lock a mutex[]{}\n";
//            std::cout << "Locked by [" << synchronizer.load() << "] thread\n";
            return false;
        }
        
    }
};

futex footLocker;
std::mutex foodLocker;
std::vector<int> tries;
int result;
int startValue = 0;

void syncronizedIncrementor(int toIncrement) {
    auto threadNumber = std::this_thread::get_id();
    while(result < startValue + toIncrement) {
        footLocker.lock(threadNumber);
        if(result == startValue + toIncrement) {
            footLocker.unlock(threadNumber);
            break;
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        
//        std::cout << "THREAD_NUMBER ===FUTEX=== " << threadNumber << "\n";
        ++tries[threadNumber];
        ++result;
        footLocker.unlock(threadNumber);
    }
}

void syncronizedIncrementorWithMutex(int toIncrement, int threadNumber) {
    while(result < startValue + toIncrement) {
        foodLocker.lock();
        if(result == startValue + toIncrement) {
            foodLocker.unlock();
            break;
        }
//        std::cout << "THREAD_NUMBER ===MUTEX=== " << threadNumber << "\n";
        ++tries[threadNumber];
        ++result;
        foodLocker.unlock();
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << std::thread::hardware_concurrency() << "\n";
    int first = std::thread::hardware_concurrency() / 2;
    int second = 4 * first;
    double Btime = 10;
    double Ctime = 60;
    std::cout << first << " " << second << "\n";
    result = startValue;
    int numOfIncrementionsFor10Seconds = 40000;
    int numOfIncrementionsFor60Seconds = 10000;
    int numOfThreads = 2;
    tries.resize(numOfThreads);
    std::vector<std::thread> incrementors;
    auto start = std::chrono::steady_clock::now();
    for(int i = 0; i < numOfThreads; ++i)
        incrementors.push_back(std::thread(syncronizedIncrementor, numOfIncrementionsFor60Seconds, i));
    
    for(auto &thread : incrementors)
        thread.join();
    auto end = std::chrono::steady_clock::now();
    std::cout << "--------------------------------------------------------------------------------------\n";
    std::cout << "TIME IS " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001 << "\n\n";
    std::cout << "--------------------------------------------------------------------------------------\n";
    int sum = 0;
    for(auto &tri : tries) {
        sum += tri;
        std::cout << tri << " ";
    }
    std::cout << "\n" << result << " equals to " << sum << "\n";
    return 0;
}
