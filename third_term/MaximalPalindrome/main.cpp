//
//  main.cpp
//  MaximalPalindrome
//
//  Created by Simon Fedotov on 21.10.15.
//  Copyright © 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>

//нечет
bool calculateLengths(const std::string &toCalculate, std::vector<int> &results, const int shift = 0){
    int left = 0; int right = -1;
    for(int center = 0; center < toCalculate.length(); ++center) {
        
        int position = (center > right ? (shift ^ 1) : std::min(results[left + right - center + shift], right - center + shift));
        while (center + position < toCalculate.length() && center - position - shift >= 0 && toCalculate[center + position] == toCalculate[center - position - shift])
            ++position;
        results[center] = position;
        if(position != 0 && center + position - 1 > right) {
            left = center - position + (shift ^ 1); right = center + position - 1;
        }
    }
    return (right != -1);
}

void getMaximalPalindrome(const std::string & toCalculate, std::vector<int> &results) {
//    if((firstResult || secondResult) == false) {
//        std::cout << "NO";
//        return;
//    }
//    
    int maximalLength = 0;
    int center = 0;
    bool isOdd = false;
//    for(int i = 0; i < toCalculate.length(); ++i)
//        std::cout << odd[i] << " " << even[i] << "\n";
    bool firstResult = calculateLengths(toCalculate, results);

    for(int pos = 0; pos < results.size(); ++pos) {
        if(results[pos] > maximalLength) {
            maximalLength = results[pos];
            center = pos;
            isOdd = true;
        }
    }

    results.clear();
    results.resize(toCalculate.length(), 0);
    bool secondResult = calculateLengths(toCalculate, results, true);

    
    for(int pos = 0; pos < results.size(); ++pos) {
        if(results[pos] + 1 > maximalLength) {
            maximalLength = results[pos];
            center = pos;
            isOdd = false;
        }
    }
    if(isOdd) {
//        std::cout << 2 * (maximalLength - 1) + 1 << "\n";
        std::cout << toCalculate.substr(center - maximalLength + 1, 2 * (maximalLength - 1) + 1);
    }
    else {
//        std::cout << 2 * maximalLength << "\n";
        std::cout << toCalculate.substr(center - maximalLength, 2 * maximalLength);
    }
}


int main(int argc, const char * argv[]) {
    std::string ourString;
    std::cin >> ourString;
    std::vector<int> results(ourString.length(), 0);
    getMaximalPalindrome(ourString, results);
    
    return 0;
}
