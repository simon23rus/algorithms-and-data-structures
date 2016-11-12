//
//  main.cpp
//  PrefixFunction
//
//  Created by Simon Fedotov on 18.10.15.
//  Copyright © 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <vector>

void prefixFunction(const std::string &toCalculate, std::vector<size_t> &pi) {
    pi[0] = 0;
    size_t counter;
    for(int i = 1; i < toCalculate.length(); ++i) {
        counter = pi[i - 1];
        while(counter > 0 && toCalculate[counter] != toCalculate[i])
            counter = pi[counter - 1];
        if(toCalculate[counter] == toCalculate[i])
            ++counter;
        pi[i] = counter;
    }
}

void KnuthMorrisPratt(const std::string &pattern, const std::string &toCalculate, std::vector<size_t> &pi) {
    prefixFunction(pattern, pi);
    size_t counter = 0;
    for(int i = 0; i < toCalculate.length(); ++i) {
        while (counter > 0 && (counter >= pattern.length() || pattern[counter] != toCalculate[i]))
            counter = pi[counter - 1];
        if(pattern[counter] == toCalculate[i])
            ++counter;
        
        if(counter == pattern.length()) {
            std::cout << i - counter + 1 << " ";
        }
        
    }
}

int main(int argc, const char * argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::string pattern;
    std::string ourString;
    std::cin >> pattern >> ourString;
    std::vector<size_t> pi(pattern.size());
    KnuthMorrisPratt(pattern, ourString, pi);
    return 0;
}
