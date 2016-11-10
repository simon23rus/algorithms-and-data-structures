

//  main.cpp
//  Sorting Task
//  Created by Simon Fedotov on 04.10.14.
//  Copyright (c) 2014 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <deque>
#include <string>



template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>

void heapSort(Iterator begin, Iterator end, const Comparator &comp = Comparator()) {
    if(end - begin == 0) return;
    std::make_heap(begin, end, comp);
    for(Iterator i = end; i != begin; --i) {
        std::pop_heap(begin, i, comp);
    }
}


template < typename FSourceIterator, typename SSourceIterator,
typename TargetIterator, typename Comparator>

TargetIterator Merge(FSourceIterator first_begin, FSourceIterator first_end,
                     SSourceIterator second_begin, SSourceIterator second_end,
                     TargetIterator target, const Comparator &comp) {
    while (first_begin != first_end &&
           second_begin != second_end) {
        if(!comp(*second_begin, *first_begin)) {
            *target = *first_begin;
            ++target;
            ++first_begin;
        }
        else {
            *target = *second_begin;
            ++target;
            ++second_begin;
        }
    }
    target = std::copy(first_begin, first_end, target);
    target = std::copy(second_begin, second_end, target);
    return target;
}

template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
void mergeSort(Iterator begin, Iterator end, const Comparator &comp = Comparator()) {
    if(end - begin == 0) return;
    std::vector<typename std::iterator_traits<Iterator>::value_type> buffer(end - begin);
    for(size_t chunk_size = 1; chunk_size < end - begin; chunk_size *= 2) {
        auto target = buffer.begin();
        for(Iterator first_begin = begin;
            first_begin < end;
            first_begin += std::min<size_t>(2 * chunk_size, end - first_begin)) {
                Iterator second_begin = first_begin + std::min<size_t>(chunk_size, end - first_begin);
                Iterator second_end = second_begin + std::min<size_t>(chunk_size, end - second_begin);
                target = Merge(first_begin, second_begin, second_begin, second_end, target, comp);
        }
        std::copy(buffer.begin(), buffer.end(), begin);
    }
}





template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>

void insertSortWithCopy(Iterator begin, Iterator end, const Comparator &comp = Comparator()) {
    if(end - begin == 0)
        return;
    Iterator step = begin;
    for(; step - begin < end - begin; ++step) {
        auto needToInsert = *step;
        Iterator InsertIt = step, StartToCopyIt = begin;
        while((!comp(*InsertIt, *StartToCopyIt)) && (StartToCopyIt < InsertIt))
            ++StartToCopyIt;
        std::copy_backward(StartToCopyIt, InsertIt, InsertIt + 1);
        *StartToCopyIt = needToInsert;
    }
    
}


template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>

void insertSort(Iterator begin, Iterator end, const Comparator &comp = Comparator()) {
    if(end - begin == 0)
        return;
    Iterator step = begin + 1;
    Iterator InsertIt;
    for(; step - begin < end - begin; ++step) {
        InsertIt = step - 1;
        while ((InsertIt >= begin) && comp(*(InsertIt + 1), *InsertIt)) {
            std::swap(*InsertIt, *(InsertIt + 1));
            if(InsertIt == begin) {
                break;
            }
            --InsertIt;
        }
        
    }
}




template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>

void selectionSort(Iterator begin, Iterator end, const Comparator &comp = Comparator()) {
    if(end - begin == 0)
        return;
    Iterator FirstIt;
    for(FirstIt = begin; FirstIt < end; ++FirstIt) {
        Iterator SecondIt, minimal = FirstIt;
        for(SecondIt = FirstIt; SecondIt < end; ++SecondIt) {
            if(comp(*SecondIt , *minimal))
                minimal = SecondIt;
        }
        std::iter_swap(FirstIt, minimal);
        minimal = FirstIt;
    }
}

template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>

void quickSort(Iterator start, Iterator end, const Comparator &comp = Comparator()) {
    if(end - start == 0)
        return;
    Iterator firstIt = start, secondIt = end - 1;
    
    auto middle = *(start + (end - start) / 2);
    while (firstIt <= secondIt) {
        while (comp(*firstIt , middle))
            ++firstIt;
        while (comp(middle , *secondIt))
            --secondIt;
        if (firstIt <= secondIt) {
            std::swap(*firstIt, *secondIt);
            ++firstIt; --secondIt;
        }
    }
    if(firstIt < end - 1)
        quickSort(firstIt, end, comp);
    if(secondIt > start)
        quickSort(start, secondIt + 1, comp);
}


template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>

void quickSortTuned(Iterator start, Iterator end, const Comparator &comp = Comparator()) {
    if(end - start == 0)
        return;
    if((end - start) <= 16)
        insertSort(start, end, comp);
    else {
        Iterator firstIt = start, secondIt = end - 1;
        auto middle = *(start + (end - start) / 2);
        while (firstIt <= secondIt) {
            while (comp(*firstIt , middle))
                ++firstIt;
            while (comp(middle , *secondIt))
                --secondIt;
            if (firstIt <= secondIt) {
                std::swap(*firstIt, *secondIt);
                ++firstIt; --secondIt;
            }
        }
        if(firstIt < end - 1)
            quickSortTuned(firstIt, end, comp);
        if(secondIt > start)
            quickSortTuned(start, secondIt + 1, comp);
        
    }
}