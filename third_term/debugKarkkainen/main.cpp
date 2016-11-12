//
//  main.cpp
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

static const short ALPHABET_SIZE = 27 + 10;
static const int NEW_ALPHABET_SIZE = 256;
static int OUR_STRING_SIZE;

std::vector<int> buildSuffArray(std::vector<int> &toCalculate);

class SuffixArray {
private:
    std::vector<int> suffArr;
    void RadixSort(std::vector<std::vector<int>> &newSymbols1,  std::vector<std::vector<int>> &sortedNumbers, std::vector<std::vector<int>> &sorted,  int maxDigits);
    void RadixSort(std::vector<Element> &newSymbols1, std::vector<Element> &sortedDiv3, int maxDigits);
    int GetIndex(const std::vector<int> &suffArr12, int position);
    int GetPos0(const std::vector<int> &suffArr12, int position);
    void MergeDivByThree(std::vector<int> &toCalculate, const SuffixArray &stringSuffArr, SuffixArray &stringSuffArrDiv3);
    void MergeTwoArrays(std::vector<int> &toCalculate, const SuffixArray &suffArr12, const SuffixArray &suffArrDiv3);
    bool RepairWay(std::vector<std::vector<int>> &newSymbols, std::vector<int> &way, std::vector<int> &wayVMinusPervoy, std::vector<std::vector<int>> &sortedNumbers);
    SuffixArray CalculateSuffixArray(std::vector<int> &toCalculate);
public:
    SuffixArray(const std::vector<int> &thisArr) : suffArr(thisArr){};
    SuffixArray() = default;
    SuffixArray(size_t size);
    void buildSuffArray(std::vector<int> &toCalculate);
    std::vector<int> GetSuffArray() const;
};

std::vector<int> SuffixArray::GetSuffArray() const {
    return suffArr;
}

SuffixArray::SuffixArray(size_t size) {
    suffArr.resize(size);
}
//0 pos for -1 value
void SuffixArray::RadixSort(std::vector<std::vector<int>> &newSymbols1,  std::vector<std::vector<int>> &sortedNumbers, std::vector<std::vector<int>> &sorted,  int maxDigits) {
    int bound = std::max(OUR_STRING_SIZE, NEW_ALPHABET_SIZE);
    
    for(int digit = maxDigits - 1; digit >= 0; --digit) {
        std::vector<int> buckets(bound, 0);
        
        for(int j = 0; j < newSymbols1.size(); ++j) {
            int symbol = newSymbols1[j][digit];
            ++buckets[symbol + 1];
        }
        int counter = 0;
        for(int j = 0; j < bound; ++j) {
            auto tmp = buckets[j];
            buckets[j] = counter;
            counter += tmp;
        }
        for(int j = 0; j < newSymbols1.size(); ++j) {
            int symbol = newSymbols1[j][digit];
            sortedNumbers[j][digit] = buckets[symbol + 1];
            sorted[buckets[symbol + 1]] = newSymbols1[j];
            ++buckets[symbol + 1];
        }
        
        newSymbols1 = sorted;
    }
    
}


void SuffixArray::RadixSort(std::vector<Element> &newSymbols1, std::vector<Element> &sortedDiv3, int maxDigits) {
    int bound = std::max(OUR_STRING_SIZE, NEW_ALPHABET_SIZE);
    std::vector<int> buckets(bound, 0);
    for(int j = 0; j < newSymbols1.size(); ++j) {
        int symbol = newSymbols1[j].letter;
        ++buckets[symbol + 1];
    }
    int counter = 0;
    for(int j = 0; j < bound; ++j) {
        int tmp = buckets[j];
        buckets[j] = counter;
        counter += tmp;
    }
    for(int j = 0; j < newSymbols1.size(); ++j) {
        int symbol = newSymbols1[j].letter;
        sortedDiv3[buckets[symbol + 1]] = newSymbols1[j];
        ++buckets[symbol + 1];
    }
    
    newSymbols1 = sortedDiv3;
}




int SuffixArray::GetIndex(const std::vector<int> &suffArr12, int position) {
    return suffArr12[position] < suffArr12.size() / 2 ? (suffArr12[position] * 3) + 1 : (suffArr12[position] - suffArr12.size() / 2) * 3 + 2;
}

int SuffixArray::GetPos0(const std::vector<int> &suffArr12, int position) {
    if(position % 3 == 1) {
        return  (position - 1) / 3;
    }
    else {
        return (position - 2) / 3 + suffArr12.size() / 2;
    }
}

void SuffixArray::MergeDivByThree(std::vector<int> &toCalculate, const SuffixArray &stringSuffArr, SuffixArray &stringSuffArrDiv3) {
    
    std::vector<Element> newSymbolsDiv3;
    newSymbolsDiv3.resize(stringSuffArr.suffArr.size() / 2);
    
    int counter = 0;
    for(int i = 0; i < stringSuffArr.suffArr.size(); ++i) {
        int position = GetIndex(stringSuffArr.suffArr, i);
        if(position % 3 == 1) {
            newSymbolsDiv3[counter] = {toCalculate[position - 1], position};
            ++counter;
        }
    }
    
    std::vector<Element> sortedDiv3;
    
    sortedDiv3.clear();
    sortedDiv3.resize(newSymbolsDiv3.size());
    
    RadixSort(newSymbolsDiv3, sortedDiv3, 1);
    
    for(int i = 0; i < newSymbolsDiv3.size(); ++i)
        stringSuffArrDiv3.suffArr[i] = newSymbolsDiv3[i].suffix - 1;
    
}


void MakeSuffArrVMinusPervoy(const std::vector<int> &suffArr12, std::vector<int> &result) {
    result.resize(suffArr12.size());
    for(int i = 0; i < suffArr12.size(); ++i)
        result[suffArr12[i]] = i;
}



void SuffixArray::MergeTwoArrays(std::vector<int> &toCalculate, const SuffixArray &suffArr12, const SuffixArray &suffArrDiv3) {
    std::vector<int> suffArr12VMinusPervoy;
    MakeSuffArrVMinusPervoy(suffArr12.suffArr, suffArr12VMinusPervoy);
    suffArr.resize(suffArr12.suffArr.size() + suffArrDiv3.suffArr.size());
    
    int firstShift = 0, secondShift = 0;
    std::vector<int> firstSymbols;
    std::vector<int> secondSymbols;
    while(firstShift < suffArr12.suffArr.size() && secondShift < suffArrDiv3.suffArr.size()) {
        int firstPosition = GetIndex(suffArr12.suffArr, firstShift);
        int secondPosition = suffArrDiv3.suffArr[secondShift];
        
        firstSymbols.clear();
        secondSymbols.clear();
        int firstSuffArrPosition;
        int secondSuffArrPosition;
        
        if(firstPosition % 3 == 1) {
            firstSymbols.push_back(toCalculate[firstPosition]);
            firstSuffArrPosition = suffArr12VMinusPervoy[GetPos0(suffArr12.suffArr, firstPosition + 1)];
            secondSymbols.push_back(toCalculate[secondPosition]);
            secondSuffArrPosition = suffArr12VMinusPervoy[GetPos0(suffArr12.suffArr, secondPosition + 1)];
            if(firstSymbols < secondSymbols || (firstSymbols == secondSymbols && firstSuffArrPosition <= secondSuffArrPosition)) {
                suffArr[firstShift + secondShift] = (firstPosition);
                ++firstShift;
            }
            else {
                suffArr[firstShift + secondShift] = (secondPosition);
                ++secondShift;
            }
        }
        else {
            if(firstPosition < toCalculate.size() - 2) {
                firstSymbols = {toCalculate[firstPosition], toCalculate[firstPosition + 1]};
                firstSuffArrPosition = suffArr12VMinusPervoy[GetPos0(suffArr12.suffArr, firstPosition + 2)];
            }
            else if(firstPosition < toCalculate.size() - 1) {
                firstSymbols = {toCalculate[firstPosition], toCalculate[firstPosition + 1]};
                firstSuffArrPosition = -1;
            }
            else {
                firstSymbols = {toCalculate[firstPosition], -1};
                firstSuffArrPosition = -2;
            }
            
            if(secondPosition < toCalculate.size() - 2) {
                secondSymbols = {toCalculate[secondPosition], toCalculate[secondPosition + 1]};
                secondSuffArrPosition = suffArr12VMinusPervoy[GetPos0(suffArr12.suffArr, secondPosition + 2)];
            }
            else if(secondPosition < toCalculate.size() - 1) {
                secondSymbols = {toCalculate[secondPosition], toCalculate[secondPosition + 1]};
                secondSuffArrPosition = -1;
            }
            else {
                secondSymbols = {toCalculate[secondPosition], -1};
                secondSuffArrPosition = -2;
            }
            
            if(firstSymbols < secondSymbols || (firstSymbols == secondSymbols && firstSuffArrPosition <= secondSuffArrPosition)) {
                suffArr[firstShift + secondShift] = (firstPosition);
                ++firstShift;
            }
            else {
                suffArr[firstShift + secondShift] = (secondPosition);
                ++secondShift;
            }
            
        }
        
    }
    while(firstShift < suffArr12.suffArr.size()) {
        int toPush = GetIndex(suffArr12.suffArr, firstShift);
        suffArr[firstShift + secondShift] = (toPush);
        ++firstShift;
    }
    while(secondShift < suffArrDiv3.suffArr.size()) {
        int toPush = suffArrDiv3.suffArr[secondShift];
        suffArr[firstShift + secondShift] = (toPush);
        ++secondShift;
    }
}



bool SuffixArray::RepairWay(std::vector<std::vector<int>> &newSymbols, std::vector<int> &way, std::vector<int> &wayVMinusPervoy, std::vector<std::vector<int>> &sortedNumbers) {
    bool result = true;
    for(int i = 0; i < way.size(); ++i) {
        way[i] = sortedNumbers[sortedNumbers[sortedNumbers[i][2]][1]][0];
    }
    for (int i = 0; i < way.size(); ++i) {
        wayVMinusPervoy[way[i]] = i;
    }
    
    int classes = 0;
    way[wayVMinusPervoy[0]] = classes;
    for(int i = 0; i < wayVMinusPervoy.size() - 1; ++i) {
        if(newSymbols[i + 1] == newSymbols[i])
            result = false;
        if(newSymbols[i + 1] != newSymbols[i])
            ++classes;
        
        way[wayVMinusPervoy[i + 1]] = classes;
        
    }
    
    
    return result;
}

SuffixArray SuffixArray::CalculateSuffixArray(std::vector<int> &toCalculate) {
    
    if(toCalculate.size() % 3 == 1) {
        toCalculate.insert(toCalculate.end(), 2, -1);
    }
    else if(toCalculate.size() % 3 == 2)
        toCalculate.push_back(-1);
    
    std::vector<std::vector<int>> newSymbols((toCalculate.size() * 2) / 3);
    int counter = 0;
    for(int start = 1; start < toCalculate.size(); start += 3) {
        if(start < toCalculate.size() - 2)
            newSymbols[counter] = {toCalculate[start], toCalculate[start + 1], toCalculate[start + 2]};
        else if(start < toCalculate.size() - 1)
            newSymbols[counter] = {toCalculate[start], toCalculate[start + 1], -1};
        else
            newSymbols[counter] = {toCalculate[start], -1, -1};
        ++counter;
    }
    for(int start = 2; start < toCalculate.size(); start += 3) {
        if(start < toCalculate.size() - 2)
            newSymbols[counter] = {toCalculate[start], toCalculate[start + 1], toCalculate[start + 2]};
        else if(start < toCalculate.size() - 1)
            newSymbols[counter] = {toCalculate[start], toCalculate[start + 1], -1};
        else
            newSymbols[counter] = {toCalculate[start], -1, -1};
        ++counter;
    }
    
    std::vector<std::vector<int>> sortedNumbers(newSymbols.size(), {0, 0, 0});
    std::vector<std::vector<int>> sorted;
    
    sorted.clear();
    sorted.resize(newSymbols.size());
    
    
    RadixSort(newSymbols, sortedNumbers, sorted, 3);
    
    std::vector<int> way(newSymbols.size());
    std::vector<int> wayVMinusPervoy(way.size());
    
    
    SuffixArray suffArr12;
    if(!RepairWay(newSymbols, way, wayVMinusPervoy, sortedNumbers)) {
        std::vector<int> newStringEncoded(way.size());
        for(int i = 0; i < way.size(); ++i)
            newStringEncoded[i] = (static_cast<int>(way[i]));
        
        suffArr12.buildSuffArray(newStringEncoded);
    }
    
    else {
        suffArr12 = SuffixArray(wayVMinusPervoy);
    }
    
    return suffArr12;
}


void SuffixArray::buildSuffArray(std::vector<int> &toCalculate) {
    if(toCalculate.size() == 4) {
        std::vector<std::vector<int>> toSort(4);
        toSort[0] = (toCalculate);
        toSort[1] = {toCalculate[1], toCalculate[2], toCalculate[3], -1};
        toSort[2] = {toCalculate[2], toCalculate[3], -1, -1};
        toSort[3] = {toCalculate[3], -1, -1, -1};
        std::vector<std::vector<int>> sorted;
        sorted = toSort;
        std::vector<std::vector<int>> sortedNumbers(toSort.size(), {0, 0, 0, 0});
        RadixSort(toSort, sortedNumbers, sorted, 4);
        std::vector<int> way(toSort.size());
        for(int i = 0; i < way.size(); ++i) {
            way[i] = sortedNumbers[sortedNumbers[sortedNumbers[sortedNumbers[i][3]][2]][1]][0];
        }
        
        std::vector<int>wayVMinusPervoy(way.size());
        for (int i = 0; i < way.size(); ++i) {
            wayVMinusPervoy[way[i]] = i;
        }
        
        suffArr = wayVMinusPervoy;
        return;
    }
    
    int oldSize = toCalculate.size();
    SuffixArray suffArr12 = CalculateSuffixArray(toCalculate);
    
    SuffixArray suffArr0 = SuffixArray(suffArr12.suffArr.size() / 2);
    MergeDivByThree(toCalculate, suffArr12, suffArr0);
    
    MergeTwoArrays(toCalculate, suffArr12, suffArr0);
    
    std::vector<int> realAnswer(suffArr.begin() + suffArr.size() - oldSize, suffArr.end());

    suffArr = realAnswer;
    return;
}


void Kasai(const std::string &toCalculate, const std::vector<int> &suffArr, std::vector<int> &LCP) {
    std::vector<int> suffArrVMinusPervoy;
    MakeSuffArrVMinusPervoy(suffArr, suffArrVMinusPervoy);
    int pos = 0;
    for(int i = 0; i < toCalculate.length(); ++i) {
        if(pos > 0)
            --pos;
        if(suffArrVMinusPervoy[i] == toCalculate.length() - 1) {
            LCP[toCalculate.length() - 1] = -1;
            pos = 0;
        }
        else {
            int j = suffArr[suffArrVMinusPervoy[i] + 1];
            while (std::max(i + pos, j + pos) < toCalculate.length() && toCalculate[i + pos] == toCalculate[j + pos])
                ++pos;
            
            LCP[suffArrVMinusPervoy[i]] = pos;
        }
    }
}

long long FindDifferentSubstrings(const std::string &toCalculate, const std::vector<int> &suffArr) {
    
    std::string old = toCalculate;
    old.append("$");
    
    auto newAnswer = suffArr;
    newAnswer.insert(newAnswer.begin(), old.length() - 1);
    std::vector<int>LCP(old.length());
    Kasai(old, newAnswer, LCP);
    long long result = 0;
    for(int i = 0; i < old.length() - 1; ++i) {
        result += old.length() - 1 - suffArr[i] - LCP[i];
    }
    
    
    return result;
}


int main(int argc, const char * argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in;
//    in.open("/Users/semenfedotov/Desktop/3SemestrCoding/Karkkainen/Karkkainen/test.txt");
    //    long long differentSubstrings = 0;
//    int windowSize;
    
    //    std::cin >> windowSize;
    //    char* str = (char*)malloc(100000000);
    //    scanf("%s", str);
    std::string ourString;
        std::cin >> ourString;
    //    ourString = str;
//    in >> ourString;
//    std::cout << ourString.length() << "zuzka\n";
    OUR_STRING_SIZE = ourString.length();
    
    //    OUR_STRING_SIZE = ourString.length();
    //    std::cout << ourString;
    //    std::cin >> ourString;
    //    std::cin >> ourString;
    //    in >> ourString;
    //    std::string ourString = old;
    //    OUR_STRING_SIZE = ourString.length();
    
    
    //    std::vector<int> encodedOurString;
    //    for(int i = 0; i < windowSize; ++i) {
    //        std::string ourString;
    //        std::cin >> ourString;
    //        encodedOurString.clear();
    //        encodedOurString.resize(ourString.length());
    //            for(int i = 0; i <  ourString.size(); ++i)
    //                encodedOurString[i] = (int)ourString[i];
    //        std::string old = ourString;
    //        auto suffArr = buildSuffArray(encodedOurString);
    //
    //                    std::cout << FindDifferentSubstrings(old, suffArr) + 1 << " ";
    //    }
    
    //    std::cout << "TOOT RAZMER "<< ourString.length() << "AWJEI\n";
    //    std::vector<std::pair<std::string, int>> strs;
    //    for(int i = 0; i < ourString.size(); ++i)
    //        strs.push_back({ourString.substr(i), i});
    //
    //    std::stable_sort(strs.begin(), strs.end());
    //    for(auto elem : strs)
    //        std::cout << elem.second << " ";
    //
    //    std::cout << "\n bulk \n";
    //
    //
    
    long long result = 0;
    
    std::vector<int> encodedOurString(ourString.length());
    for(int i = 0; i <  ourString.size(); ++i)
        encodedOurString[i] = (int)ourString[i];
    
    
    std::string differentSubstrings;
    
    
    
    //    for(int i = 0; i < ourString.length(); ++i) {
    //        if(i + windowSize <= ourString.length()) {
    //            std::string toCalculate = ourString.substr(i, windowSize);
    //            std::string old = toCalculate;
    //            std::vector<int> encodedOurString(toCalculate.length());
    //            for(int i = 0; i <  toCalculate.size(); ++i)
    //                encodedOurString[i] = (int)toCalculate[i];
    //
    //            auto suffArr = buildSuffArray(encodedOurString);
    //
    //            std::cout << FindDifferentSubstrings(old, suffArr) << " ";
    //        }
    //        else {
    //            std::string toCalculate = ourString.substr(i);
    //            toCalculate += ourString.substr(0, windowSize -( ourString.length() - i));
    //            std::string old = toCalculate;
    //            std::vector<int> encodedOurString(toCalculate.length());
    //            for(int i = 0; i <  toCalculate.size(); ++i)
    //                encodedOurString[i] = (int)toCalculate[i];
    //
    //            auto suffArr = buildSuffArray(encodedOurString);
    //
    //            std::cout << FindDifferentSubstrings(old, suffArr) << " ";
    //        }
    //    }
    //
    //
    
    
    
    //SADASDASDASDa
    //    std::string toCalculate = ourString + ourString.substr(0, windowSize);
    //    std::vector<int> encodedOurString(toCalculate.length());
    //    for(int i = 0; i <  toCalculate.size(); ++i)
    //        encodedOurString[i] = (int)toCalculate[i];
    //
    //    for(int i = 0; i < ourString.length(); ++i) {
    //        std::vector<int> toCount(encodedOurString.begin() + i, encodedOurString.begin() + i + windowSize);
    //        sorted.clear();
    //        sortedDiv3.clear();
    //        OUR_STRING_SIZE = windowSize;
    //        std::vector<int> suffArr = buildSuffArray(toCount);
    //        std::string thisString = toCalculate.substr(i, windowSize);
    //        std::string toPush = std::to_string(FindDifferentSubstrings(thisString, suffArr)) + " ";
    //        differentSubstrings += toPush;
    //    }
    //
    //    std::cout << differentSubstrings;
    
    //SADIOWJIDOAIJDIO
    
    
    //    auto answer = buildSuffArray(encodedOurString);
    
    
    //    for(int i = 0; i < answer.size(); ++i)
    //        if(answer[i] != strs[i].second)
    //            assert(false);
    
    //    std::cout << "etolcp\n";
    //    for(auto elem : LCP)
    //        std::cout << elem << " ";
    //    std::cout << "\n";
    //    for(auto elem : answer)
    //        std::cout << elem << " ";
    
    //    std::cout << FindDifferentSubstrings(old, answer);
    SuffixArray mySuffixArray(0);
    mySuffixArray.buildSuffArray(encodedOurString);
    std::cout << FindDifferentSubstrings(ourString, mySuffixArray.GetSuffArray());
    return 0;
}
