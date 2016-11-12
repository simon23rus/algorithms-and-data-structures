//
//  main.cpp
//  ZFunction
//
//  Created by Simon Fedotov on 19.10.15.
//  Copyright © 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>

void ZFunction(const std::string &toCalculate, std::vector<int> &z) {
    int left = 0; int right = 0;
    int position;
    z[0] = toCalculate.length();
    for(int i = 1; i < toCalculate.length(); ++i) {
        if (i > right) {
            for (position = 0; ((position + i) < toCalculate.length()) && (toCalculate[i + position] == toCalculate[position]); ++position);
            z[i] = position;
            left = i;
            right = i + position - 1;
        }
        else
            if (z[i - left] < right - i + 1)
                z[i] = z[i - left];
            else {
                for (position = 1; ((position + right) < toCalculate.length()) && (toCalculate[right + position] == toCalculate[right - i + position]); ++position);
                z[i] = right - i + position;
                left = i;
                right = right + position - 1;
            }
    }
}

void KnuthMorrisPratt(const std::string &pattern, const std::string &toCalculate, std::vector<int> &z) {
    ZFunction(pattern + "$" + toCalculate, z);
    for(int i = pattern.length() + 1; i <= pattern.length() + toCalculate.length(); ++i) {
    
        if(z[i] == pattern.length()) {
            std::cout << i - pattern.length() - 1 << " ";
        }
        
    }
}



int main(int argc, const char * argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::string pattern;
    std::string toCalculate;
    std::cin >> pattern >> toCalculate;
//    std::vector<int> z(pattern.length() + 1 + ourString.length());
    std::vector<int> z(pattern.length());
    ZFunction(pattern, z);
    z[0] = 0;
    int left = 0; int right = 0;
    int position = 0;
    for(int i = 0; i < toCalculate.length(); ++i) {
        position = 0;
        if (i <= right)
            position = std::min(right - i + 1, z[i - left]);
        while (i + position < toCalculate.length() && pattern[position] == toCalculate[i + position])
            ++position;
        if (i + position - 1 > right) {
            left = i;
            right = i + position - 1;
        }
        
        if(position == pattern.length()) {
            std::cout << left << " ";
        }
    }
    
//    KnuthMorrisPratt(pattern, ourString, z);
    return 0;
}
