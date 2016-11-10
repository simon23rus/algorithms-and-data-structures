//
//  tests.cpp
//  insertSort
//
//  Created by Simon Fedotov on 27.10.14.
//  Copyright (c) 2014 Simon23Rus. All rights reserved.
//
//template <typename Func, typename Container, typename Check>
//
//void TestSort(Func sort, const Container &c) {
//    Container cCopy = c;
//    sort(cCopy.begin(), cCopy.end());
//    Check(c, cCopy);
//}




#include "sorts.h"
#include <random>

std::default_random_engine JustRandom(std::random_device{}());
std::uniform_int_distribution<int> OurInts(1, 100000);
std::uniform_int_distribution<int> JustForStrings(0, 26);
std::uniform_real_distribution<double> OurDoubles(0.0, 100000.0);


struct bestStruct {
    std::string key;
    int value;
};

struct Xcmp {
    bool operator()(const bestStruct &a, const bestStruct &b) const {
        return a.key < b.key;
    }
};

struct XCompareNew {
    bool operator()(const bestStruct &first, const bestStruct &second) const {
        if(first.key < second.key)
            return true;
        if(first.key == second.key) {
            if(first.value < second.value)
                return true;
            return false;
        }
        else
            return false;
    }
};


struct OurEqual {
    bool operator ()(const bestStruct &a, const bestStruct &b) const {
        return (a.key == b.key && a.value == b.value);
    }
};

std::string RandomString(std::string &str) {
    int x, size;
    size = OurInts(JustRandom);
    str.resize(size);
    for(int i = 0; i < str.size(); i++) {
        x = JustForStrings(JustRandom) + 97;
        str[i] = char(x);
    }
    return str;
}





void GenerateContainer(std::vector<int> &RandomVector, int OurSize) {
    int newSize;
    if(OurSize >= 10) {
        newSize = (OurSize + std::uniform_int_distribution<int>(-0.1 * OurSize, 0.1 * OurSize)(JustRandom));
    }
    else newSize = OurSize;
    RandomVector.resize(newSize);
    for(int i = 0; i < newSize; i++) {
        int x = OurInts(JustRandom);
        RandomVector[i] = x;
    }

}
void GenerateContainer(std::vector<double> &RandomVecDouble, int OurSize) {
    int newSize;
    if(OurSize >= 10) {
        newSize = (OurSize + std::uniform_int_distribution<int>(-0.1 * OurSize, 0.1 * OurSize)(JustRandom));
    }
    else newSize = OurSize;
    RandomVecDouble.resize(newSize);
    for(int i = 0; i < newSize; i++) {
        double x = OurDoubles(JustRandom);
        RandomVecDouble[i] = x;
    }
}
void GenerateContainer(std::vector<bestStruct> &RandomBestStructs, int OurSize) {
    int newSize;
    if(OurSize >= 10) {
        newSize = (OurSize + std::uniform_int_distribution<int>(-0.1 * OurSize, 0.1 * OurSize)(JustRandom));
    }
    else newSize = OurSize;
    RandomBestStructs.resize(newSize);
    for(int i = 0; i < newSize; i++) {
        RandomBestStructs[i].key = RandomString(RandomBestStructs[i].key);
        RandomBestStructs[i].value = OurInts(JustRandom);
    }
}
void GenerateContainer(std::deque<int> &RandomDeque, int OurSize) {
    int newSize;
    if(OurSize >= 10) {
        newSize = (OurSize + std::uniform_int_distribution<int>(-0.1 * OurSize, 0.1 * OurSize)(JustRandom));
    }
    else newSize = OurSize;
    RandomDeque.resize(newSize);
    for(int i = 0; i < newSize; i++) {
        int x = OurInts(JustRandom);
        RandomDeque[i] = x;
    }
}

template<typename Container, typename Equal = std::equal_to<typename Container::value_type>>

bool MakeCompareForContainers(Container &First, Container &Second, const Equal &equalik = Equal()) {
    if(First.size()!= Second.size())
        return false;
    for (int position = 0; position < First.size(); ++position) {
        if(!(equalik(First[position], Second[position])))
            return false;
    }
    return true;
}



template<typename Container, typename Comparator = std::less<typename Container::value_type>, typename Equal = std::equal_to<typename Container::value_type>>

void MakeTestsForContainer(int TestNumber, int ContainerSize , const Comparator &comp = Comparator(), const Equal &equalik = Equal()) {
    bool isError = false;
    double InsertionAverageTime = 0.0;
    double InsertionCopyAverageTime = 0.0;
    double SelectionAverageTime = 0.0;
    double QuickSortTunedAverageTime = 0.0;
    double HeapAverageTime = 0.0;
    double MergeAverageTime = 0.0;
    double StdSortAverageTime= 0.0;
    double StdStableSortAverageTime = 0.0;
    for(int WhatTestIsNow = 0; WhatTestIsNow < TestNumber && !isError; ++WhatTestIsNow) {
        Container RandContainer;
        GenerateContainer(RandContainer, ContainerSize);
        Container TestContainer = RandContainer, CompareContainer = RandContainer;
        
        auto startPointVecIntStdSort = std::chrono::steady_clock::now();
        std::sort(CompareContainer.begin(), CompareContainer.end(), comp);
        auto endPointVecIntStdSort = std::chrono::steady_clock::now();
        StdSortAverageTime+= std::chrono::duration_cast<std::chrono::microseconds>(endPointVecIntStdSort - startPointVecIntStdSort).count() * 0.000001;
        
        if(TestContainer.size() <= 12345) {
            auto startPointInsertion = std::chrono::steady_clock::now();
            insertSort(TestContainer.begin(), TestContainer.end(), comp);
            auto endPointInsertion = std::chrono::steady_clock::now();
            if(!MakeCompareForContainers(TestContainer, CompareContainer, equalik)) {
                std::cout << "\nERROR in Insertion!\n";
                exit(1);
            }
            InsertionAverageTime += std::chrono::duration_cast<std::chrono::microseconds>(endPointInsertion - startPointInsertion).count() * 0.000001;
        }
        
        if(TestContainer.size() <= 12345) {
            TestContainer = RandContainer;
            auto startPointInsertionCopy = std::chrono::steady_clock::now();
            insertSortWithCopy(TestContainer.begin(), TestContainer.end(), comp);
            auto endPointInsertionCopy = std::chrono::steady_clock::now();
            if(!MakeCompareForContainers(TestContainer, CompareContainer, equalik)) {
                std::cout << "\nERROR in InsertCopy\n";
                exit(1);
            }
            InsertionCopyAverageTime += std::chrono::duration_cast<std::chrono::microseconds>(endPointInsertionCopy - startPointInsertionCopy).count() * 0.000001;
        }
        
        if(TestContainer.size() <= 12345) {
            TestContainer = RandContainer;
            auto startPointSelection = std::chrono::steady_clock::now();
            selectionSort(TestContainer.begin(), TestContainer.end(), comp);
            auto endPointSelection = std::chrono::steady_clock::now();
            if(!MakeCompareForContainers(TestContainer, CompareContainer, equalik)) {
                std::cout << "\nERROR in Selection\n";
                exit(1);
            }
            SelectionAverageTime += std::chrono::duration_cast<std::chrono::microseconds>(endPointSelection - startPointSelection).count() * 0.000001;
        }
        
        TestContainer = RandContainer;
        auto startPointQSTuned = std::chrono::steady_clock::now();
        quickSortTuned(TestContainer.begin(), TestContainer.end(), comp);
        auto endPointQSTuned = std::chrono::steady_clock::now();
        if(!MakeCompareForContainers(TestContainer, CompareContainer, equalik)) {
            std::cout << "\nERROR in Quick\n";
            exit(1);
        }
        QuickSortTunedAverageTime += std::chrono::duration_cast<std::chrono::microseconds>(endPointQSTuned - startPointQSTuned).count() * 0.000001;
        
        TestContainer = RandContainer;
        auto startPointHeap = std::chrono::steady_clock::now();
        heapSort(TestContainer.begin(), TestContainer.end(), comp);
        auto endPointHeap = std::chrono::steady_clock::now();
        if(!MakeCompareForContainers(TestContainer, CompareContainer, equalik)) {
            std::cout << "\nERROR in Heap\n";
            exit(1);
        }
        HeapAverageTime += std::chrono::duration_cast<std::chrono::microseconds>(endPointHeap - startPointHeap).count() * 0.000001;
        
        TestContainer = RandContainer;
        auto startPointMerge = std::chrono::steady_clock::now();
        mergeSort(TestContainer.begin(), TestContainer.end(), comp);
        auto endPointMerge = std::chrono::steady_clock::now();
        if(!MakeCompareForContainers(TestContainer, CompareContainer, equalik)) {
            std::cout << "\nERROR in Merge\n";
            exit(1);
        }
        MergeAverageTime += std::chrono::duration_cast<std::chrono::microseconds>(endPointMerge - startPointMerge).count() * 0.000001;
        
        TestContainer = RandContainer;
        auto startPointStableSort = std::chrono::steady_clock::now();
        std::stable_sort(TestContainer.begin(), TestContainer.end(), comp);
        auto endPointStableSort = std::chrono::steady_clock::now();
        StdStableSortAverageTime += std::chrono::duration_cast<std::chrono::microseconds>(endPointStableSort - startPointStableSort).count() * 0.000001;
        
    }
    std:: cout.precision(6);
    std:: cout.setf(std::ios::fixed, std:: ios::floatfield);
    if(ContainerSize < 23) {
        std::cout << "           -||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-||-" << "____Size is   " << ContainerSize << "\n";
    }
    
    else if(ContainerSize > 12345) {
        std::cout <<  "           " << "--------" << "      " << "--------" << "       " << "--------" << "    ";
        std::cout <<  QuickSortTunedAverageTime / TestNumber << "      " <<
        HeapAverageTime / TestNumber << "        " <<
        MergeAverageTime / TestNumber  << "      " <<
        StdSortAverageTime / TestNumber << "     " <<
        StdStableSortAverageTime / TestNumber << "   ____Size is   " <<
        ContainerSize; std::cout << std::endl;
    }
    else {
        std:: cout << "           " <<
        InsertionAverageTime /TestNumber << "      " <<
        InsertionCopyAverageTime / TestNumber << "       " <<
        SelectionAverageTime /TestNumber << "    " <<
        QuickSortTunedAverageTime / TestNumber << "      " <<
        HeapAverageTime / TestNumber << "        " <<
        MergeAverageTime / TestNumber  << "      " <<
        StdSortAverageTime / TestNumber << "     " <<
        StdStableSortAverageTime / TestNumber << "   ____Size is   " << ContainerSize; std::cout << std::endl;
    }
    
}


int main() {
    std:: cout << "                       Insert      InsertCopy     Selection     Quick          Heap           Merge       Std::Sort     Std::Stable"; std::cout << "\n";
    std::cout << "Vector<int>"; MakeTestsForContainer<std::vector<int>>(33, 0);
    std::cout << "Vector<int>"; MakeTestsForContainer<std::vector<int>>(22, 1);
    std::cout << "Vector<int>"; MakeTestsForContainer<std::vector<int>>(11, 2);
    std::cout << "Vector<int>"; MakeTestsForContainer<std::vector<int>>(10, 1000);
    std::cout << "Vector<int>"; MakeTestsForContainer<std::vector<int>>(5, 10000);
    std::cout << "Vector<int>"; MakeTestsForContainer<std::vector<int>>(2, 100000);
    std::cout << "Vector<int>"; MakeTestsForContainer<std::vector<int>>(1, 1000000);
    std::cout << "\n" << "__________________________________________________________________________________________________________________________________\n";
    std::cout << "Deque<int> "; MakeTestsForContainer<std::deque<int>>(33, 0);
    std::cout << "Deque<int> "; MakeTestsForContainer<std::deque<int>>(22, 1);
    std::cout << "Deque<int> "; MakeTestsForContainer<std::deque<int>>(11, 2);
    std::cout << "Deque<int> "; MakeTestsForContainer<std::deque<int>>(10, 1000);
    std::cout << "Deque<int> "; MakeTestsForContainer<std::deque<int>>(5, 10000);
    std::cout << "Deque<int> "; MakeTestsForContainer<std::deque<int>>(2, 100000);
    std::cout << "Deque<int> "; MakeTestsForContainer<std::deque<int>>(1, 1000000);
    std::cout << "\n" << "__________________________________________________________________________________________________________________________________\n";
    std::cout << "Vec<double>"; MakeTestsForContainer<std::vector<double>>(33, 0);
    std::cout << "Vec<double>"; MakeTestsForContainer<std::vector<double>>(22, 1);
    std::cout << "Vec<double>"; MakeTestsForContainer<std::vector<double>>(11, 2);
    std::cout << "Vec<double>"; MakeTestsForContainer<std::vector<double>>(10, 1000);
    std::cout << "Vec<double>"; MakeTestsForContainer<std::vector<double>>(5, 10000);
    std::cout << "Vec<double>"; MakeTestsForContainer<std::vector<double>>(2, 100000);
    std::cout << "Vec<double>"; MakeTestsForContainer<std::vector<double>>(1, 1000000);
    std::cout << "\n" << "__________________________________________________________________________________________________________________________________\n";
    std::cout << "Vec<Best>  "; MakeTestsForContainer<std::vector<bestStruct>>(33, 0, Xcmp(), OurEqual());
    std::cout << "Vec<Best>  "; MakeTestsForContainer<std::vector<bestStruct>>(22, 1, Xcmp(), OurEqual());
    std::cout << "Vec<Best>  "; MakeTestsForContainer<std::vector<bestStruct>>(11, 2, Xcmp(), OurEqual());
    std::cout << "Vec<Best>  "; MakeTestsForContainer<std::vector<bestStruct>>(4, 1000, Xcmp(), OurEqual());
    std::cout << "Vec<Best>  "; MakeTestsForContainer<std::vector<bestStruct>>(3, 10000, Xcmp(), OurEqual());
    std::cout << "Vec<Best>  "; MakeTestsForContainer<std::vector<bestStruct>>(2, 100000, Xcmp(), OurEqual());
    std::cout << "Vec<Best>  "; MakeTestsForContainer<std::vector<bestStruct>>(1, 1000000, Xcmp(), OurEqual());
    std::cout << "\n" <<
"__________________________________________________________________________________________________________________________________\n";
    
    return 0;
}