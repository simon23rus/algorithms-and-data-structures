//
//  Karkkainen.h
//  Karkkainen
//
//  Created by Simon Fedotov on 10.11.15.
//  Copyright © 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <list>
#include <assert.h>
#include <fstream>
#include <sstream>


struct Element {
    int letter;
    int suffix;
};

static const int NEW_ALPHABET_SIZE = 256;
static int OUR_STRING_SIZE;



class SuffixArray {
private:
    std::vector<int> suffArr;
    void RadixSort(std::vector<std::vector<int>> &newSymbols,  std::vector<std::vector<int>> &sortedNumbers, int maxDigits);
    void RadixSort(std::vector<Element> &newSymbols, int maxDigits);
    int GetIndex(const std::vector<int> &suffArr12, int position);
    int GetPos0(const std::vector<int> &suffArr12, int position);
    void MergeDivByThree(std::vector<int> &toCalculate, const SuffixArray &stringSuffArr, SuffixArray &stringSuffArrDiv3);
    void MergeTwoArrays(std::vector<int> &toCalculate, const SuffixArray &suffArr12, const SuffixArray &suffArrDiv3);
    bool RepairWay(std::vector<std::vector<int>> &newSymbols, std::vector<int> &way, std::vector<int> &wayInversed, std::vector<std::vector<int>> &sortedNumbers);
    SuffixArray CalculateSuffixArray(std::vector<int> &toCalculate);
public:
    SuffixArray(const std::vector<int> &thisArr) : suffArr(thisArr){};
    SuffixArray() = default;
    SuffixArray(size_t size);
    void buildSuffArray(std::vector<int> &toCalculate);
    std::vector<int> GetSuffArray() const;
};