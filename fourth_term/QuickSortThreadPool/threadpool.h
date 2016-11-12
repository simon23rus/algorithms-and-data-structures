//
//  threadPool.h
//
//  Created by Simon Fedotov on 19.05.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#ifndef threadpool_h
#define threadpool_h

#include <iostream>
#include <queue>
#include <vector>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <exception>
#include <future>

template <class Value, class Container = std::deque<Value>>
class thread_safe_queue {
public:
    thread_safe_queue(size_t capacity = size_t(-1)) : isShut(false), capacity(capacity) {}
    void enqueue(Value item);
    bool pop(Value& item);
    void shutdown();
private:
    bool isShut;
    Container queue;
    size_t capacity;
    std::condition_variable isFull;
    std::condition_variable isEmpty;
    std::mutex lock;
};

class ClosedQueueException: public std::exception {
public:
    ClosedQueueException(std::string message) : message(message) {}
    virtual const char* what() const throw()
    {
        return message.c_str();
    }
private:
    std::string message;
};

template <class Value, class Container>
void thread_safe_queue<Value, Container>::enqueue(Value item) {
    std::unique_lock<std::mutex> ul(lock);
    if (isShut)
        throw ClosedQueueException("Pushing in closed queue.\n");
    isFull.wait(ul, [this] () { return queue.size() < capacity || isShut; });
    queue.emplace_back(std::move(item));
    isEmpty.notify_one();
}

template <class Value, class Container>
bool thread_safe_queue<Value, Container>::pop(Value& item) {
    std::unique_lock<std::mutex> ul(lock);
    isEmpty.wait(ul, [this] () { return !queue.empty() || isShut; });
    if (isShut && queue.empty())
        return false;
    item = std::move(queue.front());
    queue.pop_front();
    isFull.notify_one();
    return true;
}

template <class Value, class Container>
void thread_safe_queue<Value, Container>::shutdown() {
    isShut = true;
    isFull.notify_all();
    isEmpty.notify_all();
}



template<class T>
class thread_pool {
public:
    explicit thread_pool(size_t numThreads = std::thread::hardware_concurrency()) : isShut(false) {
        for(size_t i = 0; i < numThreads; ++i)
            threads.emplace_back([this] () { startThread(); });
    }
    std::future<T> submit(std::function<T()> func);
    void shutdown();
    ~thread_pool();
    void joinThreads();
private:
    bool isShut;
    thread_safe_queue< std::packaged_task<T()> > queue;
    std::vector<std::thread> threads;
    void startThread();
};

template<typename T>
void thread_pool<T>::startThread() {
    std::packaged_task<T()> func;
    while(queue.pop(func))
        func();
}

template<class T>
std::future<T> thread_pool<T>::submit(std::function<T()> func) {
    if (isShut)
        throw ClosedQueueException("Pushing in closed pool.\n");
    std::packaged_task<T()> task(func);
    std::future<T> result = task.get_future();
    queue.enqueue(std::move(task));
    return result;
}

template<typename T>
void thread_pool<T>::joinThreads() {
    for (size_t i = 0; i < threads.size(); i++)
        if (threads[i].joinable())
            threads[i].join();
}

template<typename T>
thread_pool<T>::~thread_pool() {
    joinThreads();
}

template<class T>
void thread_pool<T>::shutdown() {
    isShut = true;
    queue.shutdown();
    joinThreads();
}

#endif /* threadpool_h */
