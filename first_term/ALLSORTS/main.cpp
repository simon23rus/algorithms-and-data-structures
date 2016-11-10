////
////  main.cpp
////  ALLSORTS
////
////  Created by Simon Fedotov on 13.12.14.
////  Copyright (c) 2014 Simon23Rus. All rights reserved.
////
//
#include <iostream>
#include "Header.h"
//
//
int main(int argc, const char * argv[]) {
    std:: cout << "                       Insert      InsertCopy     Selection     Quick          Heap           Merge       Std::Sort"; std::cout << std::endl;
    makeTestsForVectorInt(10, 1000);
    makeTestsForVectorInt(5, 10000);
    makeTestsForVectorInt(2, 100000);
    makeTestsForVectorInt(1, 1000000);
    std::cout << "\n" << "____________________________________________________________________________________________________________________\n";
    makeTestsForDeque(10, 1000);
    makeTestsForDeque(5, 10000);
    makeTestsForDeque(2, 100000);
    makeTestsForDeque(1, 1000000);
    std::cout << "\n" << "____________________________________________________________________________________________________________________\n";
    makeTestsForVecDouble(10, 1000);
    makeTestsForVecDouble(5, 10000);
    makeTestsForVecDouble(2, 100000);
    makeTestsForVecDouble(1, 1000000);
    std::cout << "\n" << "____________________________________________________________________________________________________________________\n";
    makeTestsForVecBestStructures(10, 1000);
    makeTestsForVecBestStructures(5, 10000);
    makeTestsForVecBestStructures(2, 100000);
    makeTestsForVecBestStructures(1, 1000000);
    std::cout << "\n" << "____________________________________________________________________________________________________________________\n";

    //for(int i = 0; i < n; i++) std::cout<< V[i] << " ";
    std::cout << "This is the end! \n\n";
    
    return 0;
}
//}
