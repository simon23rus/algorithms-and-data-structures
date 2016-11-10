//
//  main.cpp
//  ExchangeMarket
//
//  Created by Simon Fedotov on 11.05.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdint.h>

class ArbitrageSolver {
public:
    ArbitrageSolver(const std::vector<std::vector<double>> &DatGraph, const int &ValuesNum);
    void Run();
private:
    int ValuesNumber;
    std::vector<std::vector<double>> OurGraph;
    std::vector<int> Parents;
    std::vector<double> Profits;
    std::vector<int> OurWay;
    
    void PrintTheWay(const int &StartCurrency);
};

ArbitrageSolver::ArbitrageSolver(const std::vector<std::vector<double>> &DatGraph, const int &ValuesNum){
    ValuesNumber = ValuesNum;
    OurGraph = DatGraph;
}

void ArbitrageSolver::PrintTheWay(const int &MoneyWay){
    
    std::vector<int> OurWay;
    for(int DatNode = MoneyWay; ; DatNode = Parents[DatNode]) {
        OurWay.push_back(DatNode);
        if(DatNode == -1 || (DatNode == MoneyWay && OurWay.size() > 1)) {
            break;
        }
    }
    std::reverse(OurWay.begin(), OurWay.end());
    
    std::cout << "We've got a win strategy!\n";
    
    printf("=-=-=-=-=-=Here it is=-=-=-=-=-=\n");
    for(int i = 0; i < (int)OurWay.size(); ++i) {
        std::cout << OurWay[i] + 1 << " ";
    }
    printf("\n=-=-=-=-=-=Here it is=-=-=-=-=-=\n");


}

void ArbitrageSolver::Run(){
    bool IsSolution = false;
    int Counter = 0;
    while (!IsSolution && Counter < ValuesNumber) {
        Parents.resize(ValuesNumber, -1);
        Profits.resize(ValuesNumber, 0);
        Profits[Counter] = 1;
        int NegativeNode;
        for(int i = 0; i < ValuesNumber; ++i) {
            NegativeNode = -1;
            for(int j = 0; j < ValuesNumber * ValuesNumber; ++j){
                if(OurGraph[j / ValuesNumber][j % ValuesNumber] == 0)
                    continue;
                if(Profits[j / ValuesNumber] > 0) {
                    if(Profits[j % ValuesNumber] < Profits[j / ValuesNumber] * OurGraph[j / ValuesNumber][j % ValuesNumber]) {
                        Profits[j % ValuesNumber] = std::max((double)INT32_MIN, Profits[j / ValuesNumber] * OurGraph[j / ValuesNumber][j % ValuesNumber]);
                        Parents[j % ValuesNumber] = j / ValuesNumber;
                        NegativeNode = j % ValuesNumber;
                    }
                    
                    if(Profits[Counter] > 1) {
                        printf("-_-_-_-Profits Table-_-_-_-\n");
                        for(int a = 0; a < ValuesNumber; ++a)
                            printf("%lf ", Profits[a]);
                        printf("\n-_-_-_-Profits Table-_-_-_-\n");

                        IsSolution = true;
                        printf("Yes! we can get %lf\nFrom %d Currency\n", Profits[Counter], Counter + 1);
                        
                        PrintTheWay(Counter);
                        
                        exit(1);
                    }
                    
                }
            }
        }
        
        
        
        
        ++Counter;
    }
    
    if(!IsSolution) {
        std::cout << "There is no any variants to get more money!\n";
    }
    
    OurGraph.clear();
    Parents.clear();
    Profits.clear();
    OurWay.clear();
}


int main(int argc, const char * argv[]) {
    int ValuesNumber;
    std::cin >> ValuesNumber;
    std::vector<std::vector<double>> OurGraph(ValuesNumber);
    for(int i = 0; i < ValuesNumber; ++i) {
        for(int j = 0; j < ValuesNumber; ++j){
            double OurCost;
            std::cin >> OurCost;
            OurGraph[i].push_back(OurCost);
        }
    }
    
    ArbitrageSolver OurSolver = ArbitrageSolver(OurGraph, ValuesNumber);
    
    OurSolver.Run();
    
    return 0;
}



