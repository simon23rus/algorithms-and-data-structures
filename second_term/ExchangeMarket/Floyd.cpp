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


class ArbitrageSolver {
public:
    ArbitrageSolver(const std::vector<std::vector<double>> &DatGraph, const int &ValuesNum);
    void Run();
private:
    int ValuesNumber;
    std::vector<std::vector<double>> OurGraph;
    std::vector<std::vector<int>> Parents;
    std::vector<std::vector<double>> Profits;
    std::vector<int> OurWay;

    void PrintTheWay(const int &StartCurrency);
    bool IsAnswer(const int &StageNumber);

};

ArbitrageSolver::ArbitrageSolver(const std::vector<std::vector<double>> &DatGraph, const int &ValuesNum) {
    ValuesNumber = ValuesNum;
    OurGraph = DatGraph;
    Profits = DatGraph;
    Parents.resize(ValuesNum);
    for(int i = 0; i < ValuesNumber; ++i)
        Parents[i].resize(ValuesNumber, i);

}


void ArbitrageSolver::PrintTheWay(const int &OurCurrency){
    int DatNode = Parents[OurCurrency][OurCurrency];
    OurWay.push_back(OurCurrency);
    while (DatNode != OurCurrency) {
        OurWay.push_back(DatNode);
        DatNode = Parents[OurCurrency][DatNode];
    }
    OurWay.push_back(OurCurrency);
    std::reverse(OurWay.begin(), OurWay.end());
    
    printf("OurWay is \n");
    for(int ThisCurrency = 0; ThisCurrency < OurWay.size(); ++ThisCurrency) {
        printf("%d ", OurWay[ThisCurrency] + 1);
    }
    printf("\nEnd\n");
    OurWay.clear();

}

void ArbitrageSolver::Run() {
    bool IsSolution = false;
    for(int StageNumber = 0; StageNumber < ValuesNumber; ++StageNumber) {
        for(int From = 0; From < ValuesNumber; ++From) {
            for(int To = 0; To < ValuesNumber; ++To){
                if(Profits[From][To] == 0)
                    continue;
    
                if(Profits[From][To] < Profits[From][StageNumber] * Profits[StageNumber][To])
                    Parents[From][To] = Parents[StageNumber][To];
                Profits[From][To] = std::max(Profits[From][To], Profits[From][StageNumber] * Profits[StageNumber][To]);
                
                IsSolution = IsAnswer(StageNumber);
                if(IsSolution)
                    exit(1);
            }
        }
    }
    
    if(!IsSolution) {
        std::cout << "There is no any variants to get more Money!\n";
    }


}

bool ArbitrageSolver::IsAnswer(const int& StageNumber){
    for(int i = 0; i < ValuesNumber; ++i) {
        if(Profits[i][i] > 1) {
            printf("Yes! we can get %lf\nFrom %d Currency\n", Profits[i][i], i + 1);
            printf("On Stage = %d\n", StageNumber);
            
            printf("=-=-=-=Profits Table=-=-=-=\n");
            for(int a = 0; a < ValuesNumber; ++a) {
                for(int b = 0; b < ValuesNumber; ++b)
                    std::cout << Profits[a][b] << " ";
                std::cout << "\n";
            }
            printf("=-=-=-=Profits Table=-=-=-=\n");
            
            PrintTheWay(i);
            
            return true;
            
        }
    }

    return false;
}


int main(int argc, const char * argv[]) {
    int CurrencyNumber;
    std::cin >> CurrencyNumber;
    std::vector<std::vector<double>> ThisGraph(CurrencyNumber);
    for(int i = 0; i < CurrencyNumber; ++i) {
        for(int j = 0; j < CurrencyNumber; ++j){
            double OurCost;
            std::cin >> OurCost;
            ThisGraph[i].push_back(OurCost);
        }
    }
    
    ArbitrageSolver OurSolver = ArbitrageSolver(ThisGraph, CurrencyNumber);
    OurSolver.Run();

    return 0;
}



