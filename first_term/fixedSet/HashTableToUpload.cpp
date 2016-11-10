//
//  main.cpp
//  Hash
//
//  Created by Simon Fedotov on 14.10.14.
//  Copyright (c) 2014 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>






class FixedSet {
private:
    const int BadNumber = 1111111111;
    const int OurPrime = 2000000011;
    size_t HashTableSize;
    std::vector<std::pair<long long, long long>> HashCoeff;
    std::vector<std::vector<int>> HashTable;
    long long HashAFirst, HashBFirst;
public:
    explicit FixedSet(const std::vector<int> &numbers) {
        long long sum = 0;
        long long HashASecond, HashBSecond;
        HashTableSize = numbers.size();
        size_t NumberOfNumbers = HashTableSize;
        HashCoeff.resize(HashTableSize);
        do {
            sum = 0;
            HashTable.clear();
            HashTable.resize(HashTableSize);
            HashAFirst = rand() % (OurPrime - 1) + 1;
            HashBFirst = rand() % OurPrime;
            for(size_t i = 0; i < NumberOfNumbers; i++) {
                HashTable[FirstLevelHash(numbers[i])].push_back(numbers[i]);
            }
            for(size_t i = 0; i < HashTableSize; i++) {
                sum += (long long)HashTable[i].size() * HashTable[i].size();
            }
 
        }
        while (sum > 4 * HashTableSize);
      
        for(int i = 0; i < HashTableSize; i++) {
            std::vector<int> SecondLevel;
            HashASecond = rand() % (OurPrime - 1) + 1;
            HashBSecond = rand() % OurPrime;
            SecondLevel.resize(HashTable[i].size() * HashTable[i].size(), BadNumber);
            bool IsCollision;
            
            do {
                SecondLevel.assign(HashTable[i].size() * HashTable[i].size(), BadNumber);
                HashASecond = rand() % (OurPrime - 1) + 1;
                HashBSecond = rand() % OurPrime;
                IsCollision = false;
                for(int j = 0; j < HashTable[i].size(); j++) {
                    if(SecondLevel[SecondLevelHash(HashTable[i][j], HashASecond, HashBSecond, SecondLevel.size())] != BadNumber) {
                        IsCollision = true;
                        break;
                    }
                    SecondLevel[SecondLevelHash(HashTable[i][j], HashASecond, HashBSecond, SecondLevel.size())] = HashTable[i][j];
                }
            }
            while (IsCollision);
            
                HashTable[i].swap(SecondLevel);
                HashCoeff[i].first = HashASecond;
                HashCoeff[i].second = HashBSecond;
        }
    }
    size_t FirstLevelHash(int key) const ;
    size_t SecondLevelHash(int key, long long a, long long b, size_t HashTableSize) const;
    bool IsHere(int key) const ;
};


size_t FixedSet:: FirstLevelHash(int key) const {
    if(HashTableSize == 0)
        return BadNumber;
    if(key < 0)
        key += OurPrime;
    return (((HashAFirst * key + HashBFirst) % OurPrime) % HashTableSize);
}

size_t FixedSet :: SecondLevelHash(int key, long long a, long long b, size_t HashTableSize) const  {
    if(HashTableSize == 0)
        return BadNumber;
    if(key < 0)
        key += OurPrime;
    return (((a * key + b) % OurPrime) % HashTableSize);
}


bool FixedSet::IsHere(int key) const {
    if(HashTableSize == 0 || HashTable[FirstLevelHash(key)].size() == 0)
        return false;
    size_t OurStringPosition = FirstLevelHash(key);
    size_t OurColumnPosition = SecondLevelHash(key, HashCoeff[OurStringPosition].first, HashCoeff[OurStringPosition].second, HashTable[OurStringPosition].size());
    if(HashTable[OurStringPosition][OurColumnPosition] == key)
        return true;
    else
        return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    srand((unsigned int)time(0));
    size_t HowManyElements;
    std::cin >> HowManyElements;
    std::vector<int>numbers;
    for(size_t i = 0; i < HowManyElements; i++) {
        int OurNum;
        std::cin >> OurNum;
        numbers.push_back(OurNum);
    }
    FixedSet HashTable(numbers);
    size_t NumberOfSearch;
    int key;
    std::cin >> NumberOfSearch;
    std::vector<int> WhatINeedToCheck(NumberOfSearch);
    for(size_t i = 0; i < NumberOfSearch; i++) {
        std::cin >> key;
        WhatINeedToCheck[i] = key;
    }
    for(size_t i = 0; i < WhatINeedToCheck.size(); ++i) {
        if(HashTable.IsHere(WhatINeedToCheck[i]))
            std::cout << "Yes\n";
        else
            std::cout << "No\n";
    }
    return 0;
    }


    
