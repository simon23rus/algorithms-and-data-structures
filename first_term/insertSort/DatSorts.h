// Completed улучшить

//  main.cpp
//  insertSort

//  Created by Simon Fedotov on 04.10.14.
//  Copyright (c) 2014 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <deque>
#include <typeinfo>
#include <string>



/*class myIt:
public std::iterator<std::random_access_iterator_tag, std::vector<int>> {
};*/
/*template <typename SortFunc>

void Test (const SortFunc &sort_func)
 vector int   ->   Test( [](const vector<int> &v) Sort(v.begin(), v.end());
 vector X     ->   Test( [](const vector<X> &v) Sort(v.begin(), v.end(), Xcmp())
 
 
 void Test() {
 SortContainer < Container, typename Container :: value_type> (v);
 }
 template < typename C, typename T >
 void SortContainer (C &v) {
 sort(v.begin, v.end);
 }
 for Structs
 SortContainer <C, X> (C &v) {
 }
 
 
*/

struct bestStruct {
    std::string key;
    int value;
};

struct Xcmp {
    bool operator()(const bestStruct &a, const bestStruct &b) const {
        return a.key < b.key;
    }
};

std::string RandomString(std::string &str) {
    //srand(time(0));
    int x, size;
    size = rand() % 20 + 1;
    str.resize(size);
    for(int i = 0; i < str.size(); i++) {
        x = rand() % 26 + 97;
        str[i] = char(x);
    }
    return str;
}


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

template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
void mergeSort(Iterator begin, Iterator end, const Comparator &comp = Comparator()) {
    if(end - begin == 0) return;
    std::deque<typename std::iterator_traits<Iterator>::value_type> buffer(end - begin);
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
    Iterator InsertIt, StartToCopyIt;
    for(; step - begin < end - begin; ++step) {
        auto needToInsert = *step;
        InsertIt = begin + (step - begin);
        StartToCopyIt = begin;
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
    Iterator step = begin;
    Iterator InsertIt, StartToCopy;
    for(; step - begin < end - begin; ++step) {
        auto needToInsert = *step;
        InsertIt = begin + (step - begin);
        StartToCopy = begin;
        while(!comp(*InsertIt, *StartToCopy) && (StartToCopy < InsertIt))
            ++StartToCopy;
        for(; InsertIt > StartToCopy; --InsertIt)
            *InsertIt = *(InsertIt - 1);
        *StartToCopy = needToInsert;
    }
}

template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>

void selectionSort(Iterator begin, Iterator end, const Comparator &comp = Comparator()) {
    if(end - begin == 0) return;
        Iterator FirstIt, SecondIt, minimal;
    for(FirstIt = begin; FirstIt < end; ++FirstIt) {
        minimal = FirstIt;
        for(SecondIt = FirstIt; SecondIt < end; ++SecondIt) {
            if(comp(*SecondIt , *minimal))
                minimal = SecondIt;
        }
        std::iter_swap(FirstIt, minimal);
        minimal = FirstIt;
    }
}


template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>

void insertionForQuickSort(Iterator start, Iterator end, const Comparator &comp = Comparator()) {
    if((end - start) == 0)
        return;
    Iterator step = start;
    Iterator InsertIt, StartToCopyIt;
    for(; step - start < end - start; ++step) {
        auto needToInsert = *step;
        InsertIt = start + (step - start);
        StartToCopyIt = start;
        while(!(comp(*InsertIt, *StartToCopyIt)) && StartToCopyIt < InsertIt)
            ++StartToCopyIt;
        std::copy_backward(StartToCopyIt, InsertIt, InsertIt + 1);
        *StartToCopyIt = needToInsert;
    }

}




template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>

void quickSort(Iterator start, Iterator end, const Comparator &comp = Comparator()) {
    if(end - start == 0) return;
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
    if(end - start == 0) return;
    if((end - start) <= 16)
        insertionForQuickSort(start, end, comp);
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





//template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>

bool makeTestsForVectorInt(double TestNumber, int VectorSize) {
    bool isError = false;
    double InsertionAverageTimeVecInt = 0.0;
    double InsertionCopyAverageTimeVecInt = 0.0;
    double SelectionAverageTimeVecInt = 0.0;
    double QuickSortTunedAverageTimeVecInt = 0.0;
    double HeapAverageTimeVecInt = 0.0;
    double MergeAverageTimeVecInt = 0.0;
    double StdSortAverageTimeVecInt = 0.0;
    double StdStableSortAverageTimeVecInt = 0.0;
    int WhatTestIsNow = 0;
    srand(time(0));
    
    while(WhatTestIsNow != TestNumber && !isError) {
        
        int newSize;
        if(VectorSize >= 10) {
        newSize = (VectorSize  + (rand() % (int)(0.2 * VectorSize) - (int)(0.1 * VectorSize)));
        }
        else newSize = VectorSize;
        std::deque<int> TestVectorInt(newSize), CompareVectorInt(newSize), RandomVectorInt(newSize);
        for(int i = 0; i < newSize; i++) {
            int x = rand() % 1000000;
            RandomVectorInt[i] = x;
        }
        TestVectorInt = RandomVectorInt;
        CompareVectorInt = RandomVectorInt;
    
        auto startPointVecIntStdSort = std::chrono::steady_clock::now();
        std::sort(CompareVectorInt.begin(), CompareVectorInt.end());
        auto endPointVecIntStdSort = std::chrono::steady_clock::now();
        StdSortAverageTimeVecInt += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecIntStdSort - startPointVecIntStdSort).count() * 0.000001;
        
        
        if(TestVectorInt.size() <= 9999) {
        auto startPointVecIntInsertion = std::chrono::steady_clock::now();
        insertSort(TestVectorInt.begin(), TestVectorInt.end());
        auto endPointVecIntInsertion = std::chrono::steady_clock::now();
        if(TestVectorInt != CompareVectorInt) {
            isError = true;
            break;
        }
        InsertionAverageTimeVecInt += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecIntInsertion - startPointVecIntInsertion).count() * 0.000001;
        }
       
        
        if(TestVectorInt.size() <= 9999) {
        TestVectorInt = RandomVectorInt;
        auto startPointVecIntInsertionCopy = std::chrono::steady_clock::now();
        insertSortWithCopy(TestVectorInt.begin(), TestVectorInt.end());
        auto endPointVecIntInsertionCopy = std::chrono::steady_clock::now();
        if(TestVectorInt != CompareVectorInt) {
            isError = true;
            break;
        }
        InsertionCopyAverageTimeVecInt += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecIntInsertionCopy - startPointVecIntInsertionCopy).count() * 0.000001;
        }
        
       
        if(TestVectorInt.size() <= 9999) {
        TestVectorInt = RandomVectorInt;
        auto startPointVecIntSelection = std::chrono::steady_clock::now();
        selectionSort(TestVectorInt.begin(), TestVectorInt.end());
        auto endPointVecIntSelection = std::chrono::steady_clock::now();
        if(TestVectorInt != CompareVectorInt) {
            isError = true;
            break;
        }
        SelectionAverageTimeVecInt += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecIntSelection - startPointVecIntSelection).count() * 0.000001;
        }
        
        
        TestVectorInt = RandomVectorInt;
        auto startPointVecIntQSTuned = std::chrono::steady_clock::now();
        quickSortTuned(TestVectorInt.begin(), TestVectorInt.end());
        auto endPointVecIntQSTuned = std::chrono::steady_clock::now();
        if(TestVectorInt != CompareVectorInt) {
            isError = true;
            break;
        }
        QuickSortTunedAverageTimeVecInt += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecIntQSTuned - startPointVecIntQSTuned).count() * 0.000001;
        
       
        TestVectorInt = RandomVectorInt;
        auto startPointVecIntHeap = std::chrono::steady_clock::now();
        heapSort(TestVectorInt.begin(), TestVectorInt.end());
        auto endPointVecIntHeap = std::chrono::steady_clock::now();
        if(TestVectorInt != CompareVectorInt) {
            isError = true;
            break;
        }
        HeapAverageTimeVecInt += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecIntHeap - startPointVecIntHeap).count() * 0.000001;
        
      
        TestVectorInt = RandomVectorInt;
        auto startPointVecIntMerge = std::chrono::steady_clock::now();
        mergeSort(TestVectorInt.begin(), TestVectorInt.end());
        auto endPointVecIntMerge = std::chrono::steady_clock::now();
        if(TestVectorInt != CompareVectorInt) {
            isError = true;
            std::cout << "___!!!ERROR!!!___ has found in " << WhatTestIsNow << " test";
            break;
        }
        MergeAverageTimeVecInt += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecIntMerge - startPointVecIntMerge).count() * 0.000001;
        
   
        TestVectorInt = RandomVectorInt;
        auto startPointVecIntStableSort = std::chrono::steady_clock::now();
        std::stable_sort(TestVectorInt.begin(), TestVectorInt.end());
        auto endPointVecIntStableSort = std::chrono::steady_clock::now();
        StdStableSortAverageTimeVecInt += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecIntStableSort - startPointVecIntStableSort).count() * 0.000001;
        
        
        WhatTestIsNow++;
    }
    std:: cout.precision(6);
    std:: cout.setf(std::ios::fixed, std:: ios::floatfield);
    std:: cout << "Vector<int>           " << InsertionAverageTimeVecInt /TestNumber << "      " << InsertionCopyAverageTimeVecInt / TestNumber << "       " << SelectionAverageTimeVecInt /TestNumber << "    " << QuickSortTunedAverageTimeVecInt / TestNumber << "      " << HeapAverageTimeVecInt / TestNumber << "        " << MergeAverageTimeVecInt / TestNumber  << "      " << StdSortAverageTimeVecInt / TestNumber << "   ____Size is   " << VectorSize; std::cout << std::endl;
    return isError;
    
    //Также для вектора даблов и Структур
}


bool makeTestsForDeque(int TestNumber, int DequeSize) {
    bool isError = false;
    double InsertionAverageTimeDeque = 0.0;
    double InsertionCopyAverageTimeDeque = 0.0;
    double SelectionAverageTimeDeque = 0.0;
    double QuickSortTunedAverageTimeDeque = 0.0;
    double HeapAverageTimeDeque = 0.0;
    double MergeAverageTimeDeque = 0.0;
    double StdSortAverageTimeDeque = 0.0;
    double StdStableSortAverageTimeDeque = 0.0;
    int WhatTestIsNow = 0;
    srand(time(0));
    
    while(WhatTestIsNow != TestNumber && !isError) {
        
        int newSize;
        if(DequeSize >= 10) {
            newSize = (DequeSize  + (rand() % (int)(0.2 * DequeSize) - (int)(0.1 * DequeSize)));
        }
        else newSize = DequeSize;
        std::deque<int> TestDeque(newSize), CompareDeque(newSize), RandomDeque(newSize);
        for(int i = 0; i < newSize; i++) {
            int x = rand() % 1000000;
            RandomDeque[i] = x;
        }
        TestDeque = RandomDeque;
        CompareDeque = RandomDeque;
        
        auto startPointVecIntStdSort = std::chrono::steady_clock::now();
        std::sort(CompareDeque.begin(), CompareDeque.end());
        auto endPointVecIntStdSort = std::chrono::steady_clock::now();
        StdSortAverageTimeDeque += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecIntStdSort - startPointVecIntStdSort).count() * 0.000001;
        
        
        if(TestDeque.size() <= 9999) {
            auto startPointDequeInsertion = std::chrono::steady_clock::now();
            insertSort(TestDeque.begin(), TestDeque.end());
            auto endPointDequeInsertion = std::chrono::steady_clock::now();
            if(TestDeque != CompareDeque) {
                isError = true;
                break;
            }
            InsertionAverageTimeDeque += std::chrono::duration_cast<std::chrono::microseconds>(endPointDequeInsertion - startPointDequeInsertion).count() * 0.000001;
        }
        
        
        if(TestDeque.size() <= 9999) {
            TestDeque = RandomDeque;
            auto startPointDequeInsertionCopy = std::chrono::steady_clock::now();
            insertSortWithCopy(TestDeque.begin(), TestDeque.end());
            auto endPointDequeInsertionCopy = std::chrono::steady_clock::now();
            if(TestDeque != CompareDeque) {
                isError = true;
                break;
            }
            InsertionCopyAverageTimeDeque += std::chrono::duration_cast<std::chrono::microseconds>(endPointDequeInsertionCopy - startPointDequeInsertionCopy).count() * 0.000001;
        }
        
        
        if(TestDeque.size() <= 9999) {
            TestDeque = RandomDeque;
            auto startPointDequeSelection = std::chrono::steady_clock::now();
            selectionSort(TestDeque.begin(), TestDeque.end());
            auto endPointDequeSelection = std::chrono::steady_clock::now();
            if(TestDeque != CompareDeque) {
                isError = true;
                break;
            }
            SelectionAverageTimeDeque += std::chrono::duration_cast<std::chrono::microseconds>(endPointDequeSelection - startPointDequeSelection).count() * 0.000001;
        }
        
        
        TestDeque = RandomDeque;
        auto startPointDequeQSTuned = std::chrono::steady_clock::now();
        quickSortTuned(TestDeque.begin(), TestDeque.end());
        auto endPointDequeQSTuned = std::chrono::steady_clock::now();
        if(TestDeque != CompareDeque) {
            isError = true;
            break;
        }
        QuickSortTunedAverageTimeDeque += std::chrono::duration_cast<std::chrono::microseconds>(endPointDequeQSTuned - startPointDequeQSTuned).count() * 0.000001;
        
        
        TestDeque = RandomDeque;
        auto startPointDequeHeap = std::chrono::steady_clock::now();
        heapSort(TestDeque.begin(), TestDeque.end());
        auto endPointDequeHeap = std::chrono::steady_clock::now();
        if(TestDeque != CompareDeque) {
            isError = true;
            break;
        }
        HeapAverageTimeDeque += std::chrono::duration_cast<std::chrono::microseconds>(endPointDequeHeap - startPointDequeHeap).count() * 0.000001;
        
        
        TestDeque = RandomDeque;
        auto startPointDequeMerge = std::chrono::steady_clock::now();
        mergeSort(TestDeque.begin(), TestDeque.end());
        auto endPointDequeMerge = std::chrono::steady_clock::now();
        if(TestDeque != CompareDeque) {
            isError = true;
            std::cout << "___!!!ERROR!!!___ has found in " << WhatTestIsNow << " test";
            break;
        }
        MergeAverageTimeDeque += std::chrono::duration_cast<std::chrono::microseconds>(endPointDequeMerge - startPointDequeMerge).count() * 0.000001;
        
        
        TestDeque = RandomDeque;
        auto startPointDequeStableSort = std::chrono::steady_clock::now();
        std::stable_sort(TestDeque.begin(), TestDeque.end());
        auto endPointDequeStableSort = std::chrono::steady_clock::now();
        StdStableSortAverageTimeDeque += std::chrono::duration_cast<std::chrono::microseconds>(endPointDequeStableSort - startPointDequeStableSort).count() * 0.000001;
        
        
        WhatTestIsNow++;
    }
    std:: cout.precision(6);
    std:: cout.setf(std::ios::fixed, std:: ios::floatfield);
    std:: cout << "Deque<int>            " << InsertionAverageTimeDeque /TestNumber << "      " << InsertionCopyAverageTimeDeque / TestNumber << "       " << SelectionAverageTimeDeque /TestNumber << "    " << QuickSortTunedAverageTimeDeque / TestNumber << "      " << HeapAverageTimeDeque / TestNumber << "        " << MergeAverageTimeDeque / TestNumber << "      " << StdSortAverageTimeDeque / TestNumber << "   ____Size is   " << DequeSize; std::cout << std::endl;
    return isError;
}

bool makeTestsForVecDouble(int TestNumber, int DequeSize) {
    bool isError = false;
    double InsertionAverageTimeVecDouble = 0.0;
    double InsertionCopyAverageTimeVecDouble = 0.0;
    double SelectionAverageTimeVecDouble = 0.0;
    double QuickSortTunedAverageTimeVecDouble = 0.0;
    double HeapAverageTimeVecDouble = 0.0;
    double MergeAverageTimeVecDouble = 0.0;
    double StdSortAverageTimeVecDouble = 0.0;
    double StdStableSortAverageTimeVecDouble = 0.0;
    int WhatTestIsNow = 0;
    srand(time(0));
    
    while(WhatTestIsNow != TestNumber && !isError) {
        
        int newSize;
        if(DequeSize >= 10) {
            newSize = (DequeSize  + (rand() % (int)(0.2 * DequeSize) - (int)(0.1 * DequeSize)));
        }
        else newSize = DequeSize;
        std::vector<double> TestVecDouble(newSize), CompareVecDouble(newSize), RandomVecDouble(newSize);
        for(int i = 0; i < newSize; i++) {
            double x = rand() % 1000000 * 1.312467581;
            RandomVecDouble[i] = x;
        }
        TestVecDouble = RandomVecDouble;
        CompareVecDouble = RandomVecDouble;
        
        auto startPointVecDoubleStdSort = std::chrono::steady_clock::now();
        std::sort(CompareVecDouble.begin(), CompareVecDouble.end());
        auto endPointVecDoubleStdSort = std::chrono::steady_clock::now();
        StdSortAverageTimeVecDouble += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecDoubleStdSort - startPointVecDoubleStdSort).count() * 0.000001;
        
        
        if(TestVecDouble.size() <= 9999) {
            auto startPointVecDoubleInsertion = std::chrono::steady_clock::now();
            insertSort(TestVecDouble.begin(), TestVecDouble.end());
            auto endPointVecDoubleInsertion = std::chrono::steady_clock::now();
            if(TestVecDouble != CompareVecDouble) {
                isError = true;
                break;
            }
            InsertionAverageTimeVecDouble += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecDoubleInsertion - startPointVecDoubleInsertion).count() * 0.000001;
        }
        
        
        if(TestVecDouble.size() <= 9999) {
            TestVecDouble = RandomVecDouble;
            auto startPointVecDoubleInsertionCopy = std::chrono::steady_clock::now();
            insertSortWithCopy(TestVecDouble.begin(), TestVecDouble.end());
            auto endPointVecDoubleInsertionCopy = std::chrono::steady_clock::now();
            if(TestVecDouble != CompareVecDouble) {
                isError = true;
                break;
            }
            InsertionCopyAverageTimeVecDouble += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecDoubleInsertionCopy - startPointVecDoubleInsertionCopy).count() * 0.000001;
        }
        
        
        if(TestVecDouble.size() <= 9999) {
            TestVecDouble = RandomVecDouble;
            auto startPointVecDoubleSelection = std::chrono::steady_clock::now();
            selectionSort(TestVecDouble.begin(), TestVecDouble.end());
            auto endPointVecDoubleSelection = std::chrono::steady_clock::now();
            if(TestVecDouble != CompareVecDouble) {
                isError = true;
                break;
            }
            SelectionAverageTimeVecDouble += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecDoubleSelection - startPointVecDoubleSelection).count() * 0.000001;
        }
        
        
        TestVecDouble = RandomVecDouble;
        auto startPointVecDoubleQSTuned = std::chrono::steady_clock::now();
        quickSortTuned(TestVecDouble.begin(), TestVecDouble.end());
        auto endPointVecDoubleQSTuned = std::chrono::steady_clock::now();
        if(TestVecDouble != CompareVecDouble) {
            isError = true;
            break;
        }
        QuickSortTunedAverageTimeVecDouble += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecDoubleQSTuned - startPointVecDoubleQSTuned).count() * 0.000001;
        
        
        TestVecDouble = RandomVecDouble;
        auto startPointVecDoubleHeap = std::chrono::steady_clock::now();
        heapSort(TestVecDouble.begin(), TestVecDouble.end());
        auto endPointVecDoubleHeap = std::chrono::steady_clock::now();
        if(TestVecDouble != CompareVecDouble) {
            isError = true;
            break;
        }
        HeapAverageTimeVecDouble += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecDoubleHeap - startPointVecDoubleHeap).count() * 0.000001;
        
        
        TestVecDouble = RandomVecDouble;
        auto startPointVecDoubleMerge = std::chrono::steady_clock::now();
        mergeSort(TestVecDouble.begin(), TestVecDouble.end());
        auto endPointVecDoubleMerge = std::chrono::steady_clock::now();
        if(TestVecDouble != CompareVecDouble) {
            isError = true;
            std::cout << "___!!!ERROR!!!___ has found in " << WhatTestIsNow << " test";
            break;
        }
        MergeAverageTimeVecDouble += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecDoubleMerge - startPointVecDoubleMerge).count() * 0.000001;
        
        
        TestVecDouble = RandomVecDouble;
        auto startPointVecDoubleStableSort = std::chrono::steady_clock::now();
        std::stable_sort(TestVecDouble.begin(), TestVecDouble.end());
        auto endPointVecDoubleStableSort = std::chrono::steady_clock::now();
        StdStableSortAverageTimeVecDouble += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecDoubleStableSort - startPointVecDoubleStableSort).count() * 0.000001;
        
        
        WhatTestIsNow++;
    }
    std:: cout.precision(6);
    std:: cout.setf(std::ios::fixed, std:: ios::floatfield);
    std:: cout << "Vector<double>        " << InsertionAverageTimeVecDouble /TestNumber << "      " << InsertionCopyAverageTimeVecDouble / TestNumber << "       " << SelectionAverageTimeVecDouble /TestNumber << "    " << QuickSortTunedAverageTimeVecDouble / TestNumber << "      " << HeapAverageTimeVecDouble / TestNumber << "        " << MergeAverageTimeVecDouble / TestNumber  << "      " << StdSortAverageTimeVecDouble / TestNumber << "   ____Size is   " << DequeSize; std::cout << std::endl;
    return isError;
}




bool makeTestsForVecBestStructures(int TestNumber, int VecSize) {
    bool isError = false;
    double InsertionAverageTimeVecBest = 0.0;
    double InsertionCopyAverageTimeVecBest = 0.0;
    double SelectionAverageTimeVecBest = 0.0;
    double QuickSortTunedAverageTimeVecBest = 0.0;
    double HeapAverageTimeVecBest = 0.0;
    double MergeAverageTimeVecBest = 0.0;
    double StdSortAverageTimeVecBest = 0.0;
    double StdStableSortAverageTimeVecBest = 0.0;
    int WhatTestIsNow = 0;
    srand(time(0));
    
    while(WhatTestIsNow != TestNumber && !isError) {
        
        int newSize;
        if(VecSize >= 10) {
            newSize = (VecSize  + (rand() % (int)(0.2 * VecSize) - (int)(0.1 * VecSize)));
        }
        else newSize = VecSize;
        std::vector<bestStruct> TestVecBest(newSize), CompareVecBest(newSize), RandomVecBest(newSize);
        for(int i = 0; i < newSize; i++)
            RandomVecBest[i].key = RandomString(RandomVecBest[i].key);
        TestVecBest = RandomVecBest;
        CompareVecBest = RandomVecBest;
        
        auto startPointVecBestStdSort = std::chrono::steady_clock::now();
        std::sort(CompareVecBest.begin(), CompareVecBest.end(), Xcmp());
        auto endPointVecBestStdSort = std::chrono::steady_clock::now();
        StdSortAverageTimeVecBest += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecBestStdSort - startPointVecBestStdSort).count() * 0.000001;
        
        
        if(TestVecBest.size() <= 9999) {
            auto startPointVecBestInsertion = std::chrono::steady_clock::now();
            insertSort(TestVecBest.begin(), TestVecBest.end(), Xcmp());
            auto endPointVecBestInsertion = std::chrono::steady_clock::now();
//            if(TestVecBest != CompareVecBest) {
//                isError = true;
//                break;
//            }
            InsertionAverageTimeVecBest += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecBestInsertion - startPointVecBestInsertion).count() * 0.000001;
        }
        
        
        if(TestVecBest.size() <= 9999) {
            TestVecBest = RandomVecBest;
            auto startPointVecBestInsertionCopy = std::chrono::steady_clock::now();
            insertSortWithCopy(TestVecBest.begin(), TestVecBest.end(), Xcmp());
            auto endPointVecBestInsertionCopy = std::chrono::steady_clock::now();
//            if(TestVecBest != CompareVecBest) {
//                isError = true;
//                break;
//            }
            InsertionCopyAverageTimeVecBest += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecBestInsertionCopy - startPointVecBestInsertionCopy).count() * 0.000001;
        }
        
        
        if(TestVecBest.size() <= 9999) {
            TestVecBest = RandomVecBest;
            auto startPointVecBestSelection = std::chrono::steady_clock::now();
            selectionSort(TestVecBest.begin(), TestVecBest.end(), Xcmp());
            auto endPointVecBestSelection = std::chrono::steady_clock::now();
//            if(TestVecBest != CompareVecBest) {
//                isError = true;
//                break;
//            }
            SelectionAverageTimeVecBest += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecBestSelection - startPointVecBestSelection).count() * 0.000001;
        }
        
        
        TestVecBest = RandomVecBest;
        auto startPointVecBestQSTuned = std::chrono::steady_clock::now();
        quickSortTuned(TestVecBest.begin(), TestVecBest.end(), Xcmp());
        auto endPointVecBestQSTuned = std::chrono::steady_clock::now();
//        if(TestVecBest != CompareVecBest) {
//            isError = true;
//            break;
//        }
        QuickSortTunedAverageTimeVecBest += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecBestQSTuned - startPointVecBestQSTuned).count() * 0.000001;
        
        
        TestVecBest = RandomVecBest;
        auto startPointVecBestHeap = std::chrono::steady_clock::now();
        heapSort(TestVecBest.begin(), TestVecBest.end(), Xcmp());
        auto endPointVecBestHeap = std::chrono::steady_clock::now();
//        if(TestVecBest != CompareVecBest) {
//            isError = true;
//            break;
//        }
        HeapAverageTimeVecBest += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecBestHeap - startPointVecBestHeap).count() * 0.000001;
        
        
        TestVecBest = RandomVecBest;
        auto startPointVecBestMerge = std::chrono::steady_clock::now();
        mergeSort(TestVecBest.begin(), TestVecBest.end(), Xcmp());
        auto endPointVecBestMerge = std::chrono::steady_clock::now();
//        if(TestVecBest != CompareVecBest) {
//            isError = true;
//            std::cout << "___!!!ERROR!!!___ has found in " << WhatTestIsNow << " test";
//            break;
//        }
        MergeAverageTimeVecBest += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecBestMerge - startPointVecBestMerge).count() * 0.000001;
        
        
        TestVecBest = RandomVecBest;
        auto startPointVecBestStableSort = std::chrono::steady_clock::now();
        std::stable_sort(TestVecBest.begin(), TestVecBest.end(), Xcmp());
        auto endPointVecBestStableSort = std::chrono::steady_clock::now();
        StdStableSortAverageTimeVecBest += std::chrono::duration_cast<std::chrono::microseconds>(endPointVecBestStableSort - startPointVecBestStableSort).count() * 0.000001;
        
        
        WhatTestIsNow++;
    }
    std:: cout.precision(6);
    std:: cout.setf(std::ios::fixed, std:: ios::floatfield);
    std:: cout << "Vector<bestStruct>    " << InsertionAverageTimeVecBest /TestNumber << "      " << InsertionCopyAverageTimeVecBest / TestNumber << "       " << SelectionAverageTimeVecBest /TestNumber << "    " << QuickSortTunedAverageTimeVecBest / TestNumber << "      " << HeapAverageTimeVecBest / TestNumber << "        " << MergeAverageTimeVecBest / TestNumber << "      " << StdSortAverageTimeVecBest / TestNumber << "   ____Size is   " << VecSize; std::cout << std::endl;
    return isError;
}












//template <typename SortFunction>
//
//void makeTest(const SortFunction &DatSort) {
//    makeTest([&](const std::vector<int> &v) {
//        DatSort(v.begin(), v.end());
//    });
//    makeTest( [&](const std::vector<bestStruct> &v) {
//        DatSort(v.begin(), v.end(), Xcmp());
//    });
//}

