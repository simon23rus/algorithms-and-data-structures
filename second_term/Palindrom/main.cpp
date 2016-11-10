//
//  main.cpp
//  Palindrom
//
//  Created by Simon Fedotov on 03.03.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int MaxPalindrom(std::string myStr, std::vector<std::vector<int>> &Costs) {
    for(int i = 0; i < myStr.size(); ++i) {
        Costs[i][i] = 1;
    }
    for(size_t column = 1; column < myStr.size(); ++column) {
        size_t row = 0;
        size_t myCol = column;
        while (row != myStr.size() - column) {
            if(myStr[row] == myStr[myCol]) {
                Costs[row][myCol] = Costs[row + 1][myCol - 1] + 2;
            }
            else {
                Costs[row][myCol] = std::max(Costs[row][myCol - 1], Costs[row + 1][myCol]);
            }
            ++row;
            ++myCol;
        }
     
    }
    
    
    return Costs[0][myStr.size() - 1];
}


int main(int argc, const char * argv[]) {
    std::string myString;
    std::cin >> myString;
    std::vector<std::vector<int>> Cost;
    std::string answer;
    Cost.resize(myString.size());
    for(int i = 0; i < myString.size(); ++i)
        Cost[i].resize(myString.size() , 0);
    auto ans =  MaxPalindrom(myString, Cost);
    std::cout << ans << "\n";
    if(ans == 1)
        answer += myString.front();

    else {
        int lower = 0, upper = myString.size() - 1;
        while (Cost[lower][upper] != 0) {
            int value = Cost[lower][upper];
            while (lower < myString.size() && Cost[lower][upper] == value) {
                ++lower;
            }
            --lower;
            while (upper >= 0 && Cost[lower][upper] == value) {
                --upper;
            }
            ++upper;
            answer += myString[upper];
            --upper;
            ++lower;
        }
    }
    
    for(int i = 0; i < Cost.size(); ++i) {
        for(int j = 0; j < Cost.size(); ++j) {
            std::cout << Cost[i][j] << " ";
        }
        std::cout << "\n";
    }
    
        
//       std::cout << answer << "\n";
        if(ans % 2 == 0) {
            for(int i = answer.size() - 1; i >= 0; --i)
                answer += answer[i];
        }
        else {
            for(int i = answer.size() - 2; i >= 0; --i)
                answer += answer[i];
        }
    std::cout << answer;
    return 0;
}
