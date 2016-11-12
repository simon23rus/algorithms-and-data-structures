//
//  main.cpp
//  A
//
//  Created by Simon Fedotov on 11.04.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>


// ( - 0  [ - 1 { - 2
bool isCBS() {
    int length;
    std::cin >> length;
    std::stack<char> expression;
    for(int i = 0; i < length; ++i) {
        char symbol;
        std::cin >> symbol;
        switch (symbol) {
            case ')':
                if(expression.size() == 0)
                    return false;
                    if(expression.top() == '(')
                        expression.pop();
                    else
                        return false;
                break;
            case ']':
                if(expression.size() == 0)
                    return false;
                if(expression.top() == '[')
                    expression.pop();
                else
                    return false;
                break;
            case '}':
                if(expression.size() == 0)
                    return false;
                if(expression.top() == '{')
                    expression.pop();
                else
                    return false;
                break;
            default:
                expression.push(symbol);
                break;
        }
    }
    
    if(expression.size() == 0)
        return true;
    else
        return false;
}


int main(int argc, const char * argv[]) {
    if(isCBS())
        std::cout << "Yes";
    else
        std::cout << "No";
    
    return 0;
}
