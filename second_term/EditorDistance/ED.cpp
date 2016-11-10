//
//  main.cpp
//  EditorDistance
//
//  Created by Simon Fedotov on 02.03.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <chrono>

/*int main()
{
    setlocale(LC_ALL,"Russian");
    ifstream in;
    string s; Hashtable B;
    in.open("book1.txt");
    while( !in.eof() )
    {
        in >> s; B.Addnew(s); s.clear();
    }
    while(true)
    {
        SetConsoleCP(1251); cin >> s; SetConsoleCP(866);
        B.Searchstring(s);
    }
    in.close();
    return 0;
}*/



unsigned int Wagner(const std::string &First,const std::string &Second, std::vector<std::pair<size_t, size_t>> &OurWay, size_t XShift, size_t YShift) {
    if(First.size() == 0) {
        for(int i = Second.size() - 1; i >= 0; --i)
            OurWay.push_back(std::make_pair(0 + XShift, i + YShift));
        return Second.size();
    }
    if(Second.size() == 0) {
        for(int i = First.size() - 1; i >= 0; --i)
            OurWay.push_back(std::make_pair(i + XShift, 0 + YShift));
        return First.size();
    }
    std::vector<std::vector<int>> Cost;
    int saizik = First.size();
    Cost.resize(First.size() + 1);
    for(int i = 0; i <= First.size(); ++i)
        Cost[i].resize(Second.size() + 1, 0);
    for(int i = 0; i < Second.size() + 1; ++i) {
        Cost[0][i] = i;
    }
    for(int i = 0; i < First.size() + 1; ++i) {
        Cost[i][0] = i;
    }

    for(int i = 1; i < First.size() + 1; ++i) {
        for(int j = 1; j < Second.size() + 1; ++j) {
            if(Cost[i - 1][j - 1] <= std::min(Cost[i - 1][j], Cost[i][j - 1])) {
                First[i - 1] == Second[j - 1] ? Cost[i][j] = Cost[i - 1][j - 1] : Cost[i][j] = Cost[i - 1][j - 1] + 1;
            }
            else
                Cost[i][j] = std::min(Cost[i][j - 1], Cost[i - 1][j]) + 1;
            
        }

    }
    ptrdiff_t X = First.size(), Y = Second.size();
//    OurWay.push_back(std::make_pair(X + XShift, Y + YShift));
    while (X >= 0 && Y >= 0) {
        if(X == 0) {
            --Y;
            while(Y >= 0) {
                OurWay.push_back(std::make_pair(0 + XShift, Y + YShift));
                --Y;
            }
        }
        else if(Y == 0) {
            --X;
            while (X >= 0) {
                OurWay.push_back(std::make_pair(X + XShift, 0 + YShift));
                --X;
            }
        }
        else {
            if(Cost[X - 1][Y - 1] <= std::min(Cost[X][Y - 1], Cost[X - 1][Y])) {
                OurWay.push_back(std::make_pair(X - 1 + XShift, Y - 1 + YShift));
                --X;
                --Y;
            }
            else if(Cost[X][Y - 1] <= std::min(Cost[X - 1][Y - 1], Cost[X - 1][Y])) {
                OurWay.push_back(std::make_pair(X + XShift, Y - 1 + YShift));
                --Y;
            }
            else {
                OurWay.push_back(std::make_pair(X - 1 + XShift, Y + YShift));
                --X;
            }
        }
    }
    return Cost[First.size()][Second.size()];
}




std::vector<size_t> Calculation(const std::string &First,const std::string &Second) {
    std::vector<std::vector<size_t>> Cost;
    Cost.resize(2);
    for(int i = 0; i < 2; ++i)
        Cost[i].resize(First.size() + 1, 0);
    for(int i = 0; i < First.size() + 1; ++i)
        Cost[0][i] = i;


    size_t counter = 1;
    while (counter < Second.size() + 1) {
        Cost[1][0] = Cost[0][0] + 1;
        for(int i = 1; i < First.size() + 1; ++i) {
            if(Cost[0][i - 1] <= std::min(Cost[0][i], Cost[1][i - 1])) {
                First[i - 1] == Second[counter - 1] ? Cost[1][i] = Cost[0][i - 1] : Cost[1][i] = Cost[0][i - 1] + 1;
            }
            else
                Cost[1][i] = std::min(Cost[1][i - 1], Cost[0][i]) + 1;
            
          }
        Cost[0].swap(Cost[1]);
        ++counter;
    }

    return Cost[0];
}


std::string ReturnAnswer(std::vector<std::pair<size_t, size_t>> &OurWay,const std::string &First,const std::string &Second) {
  
    std::string Result;
    for(int i = OurWay.size() - 2; i >= 0; --i) {
        if(OurWay[i].first + 1 == OurWay[i + 1].first && OurWay[i].second + 1 == OurWay[i + 1].second) {
            if(First[OurWay[i].first] == Second[OurWay[i].second])
                Result += "N";
            else
                Result += "R";
        }
        else if(OurWay[i].first + 1 == OurWay[i + 1].first && OurWay[i].second == OurWay[i + 1].second)
            Result += "D";
        else
            Result += "I";
    }
    std::reverse(Result.begin(), Result.end());
    return  Result;
}



std::vector<size_t> CCalculation(const std::string &First,const std::string &Second) {
    std::vector<std::vector<size_t>> Cost;
    Cost.resize(2);
    for(int i = 0; i < 2; ++i)
        Cost[i].resize(First.size() + 1, 0);
    for(int i = First.size(); i >= 0; --i)
        Cost[0][i] = First.size() - i;

    
    int counter = Second.size() - 1;
    while (counter >= 0) {
        Cost[1][First.size()] = Cost[0][First.size()] + 1;
        for(int i = First.size() - 1; i >= 0; --i) {
            if(Cost[0][i + 1] <= std::min(Cost[0][i], Cost[1][i + 1])) {
                First[i] == Second[counter] ? Cost[1][i] = Cost[0][i + 1] : Cost[1][i] = Cost[0][i + 1] + 1;
            }
            else
                Cost[1][i] = std::min(Cost[1][i + 1], Cost[0][i]) + 1;
            
        }
        Cost[0].swap(Cost[1]);
        --counter;
    }

    return Cost[0];
}




unsigned long Levenstein(const std::string &First,const std::string &Second, std::vector<std::pair<size_t, size_t>> & OurWay, size_t FirstShift, size_t SecondShift) {
    if(First.size() <= 1 || Second.size() <= 1) {

       return Wagner(First, Second, OurWay, FirstShift, SecondShift);
    }
    size_t middle = Second.size() / 2;
    std::string FirstLeftPart;
    std::string FirstRightReversed = First;
    std::string SecLeftPart;
    std::string SecRightPartReversed(Second.begin() + middle, Second.end());

    SecLeftPart = Second.substr(0, middle);
    std::reverse(FirstRightReversed.begin(), FirstRightReversed.end());
    std::reverse(SecRightPartReversed.begin(), SecRightPartReversed.end());
    std::vector<size_t> ToCompareFirst = Calculation(First, SecLeftPart);
        std::vector<size_t> ToCompareSecond = Calculation(FirstRightReversed, SecRightPartReversed);
    std::reverse(ToCompareSecond.begin(), ToCompareSecond.end());
        size_t OurPosition = 0;
        for(int i = 1; i < ToCompareSecond.size(); ++i) {
            if(ToCompareFirst[i] + ToCompareSecond[i] <= ToCompareFirst[OurPosition] + ToCompareSecond  [OurPosition])
                OurPosition = i;
        }
//    FirstShift += OurPosition;
//    SecondShift += middle;
//        OurWay.push_back(std::make_pair(OurPosition + FirstPos, SecondPos / 2)); //Переделать
        FirstLeftPart = First.substr(0, OurPosition);
    std::string FirstRightPart = First.substr(OurPosition);
    std::string SecRightPart(SecRightPartReversed);
    std::reverse(SecRightPart.begin(), SecRightPart.end());
    
    unsigned int res = Levenstein(FirstLeftPart, SecLeftPart, OurWay, FirstShift, SecondShift) + Levenstein(FirstRightPart, SecRightPart, OurWay, FirstShift + OurPosition, SecondShift + middle);
    return res;
}


int main() {
    std::ifstream in;
    in.open("/Users/semenfedotov/Desktop/2SemestrCoding/EditorDistance/EditorDistance/First.txt");
    std::string First;
    std::string Second;
    char MyChar;
    while(!in.eof()) {
       in >> MyChar;
       First += MyChar;
    }
    in.close();
    in.open("/Users/semenfedotov/Desktop/2SemestrCoding/EditorDistance/EditorDistance/Second.txt");
    while(!in.eof()) {
        in >> MyChar;
        Second += MyChar;
    }
//
//    getline(std::cin, First);
//    getline(std::cin, Second);
    std::vector<std::pair<size_t, size_t>> OurTrace;
//    Wagner(First, Second, OurTrace, 0, 0);
//    std::cout << "\n" << Calculation(First, Second)[First.size()] << "\n";
//    std::cout << CCalculation(First, Second)[0] << "\n";
    auto start = std::chrono::steady_clock::now();
    std::cout << "This is the Levenstein " << Levenstein(First, Second, OurTrace, 0, 0) << "\n";
    auto end = std::chrono::steady_clock::now();
    std::cout << "\n" << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001 << "\n";
    OurTrace.push_back(std::make_pair(First.size(), Second.size()));
    std::sort(OurTrace.begin(), OurTrace.end());
    std::cout << "\n" << ReturnAnswer(OurTrace, First, Second) << "\n\n";
//    for(int i = 0; i < OurTrace.size(); ++i) std::cout << OurTrace[i].first << " " << OurTrace[i].second << " \n";
    return 0;
}
