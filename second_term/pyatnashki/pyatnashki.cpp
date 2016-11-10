//
//  main.cpp
//  pyatnashki
//
//  Created by Simon Fedotov on 17.04.15.
//  Copyright (c) 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <set>
#include <map>
//#include "Fenwick.cpp"


//FinalPosition = "012345678";
const int FieldSize = 9;


class FieldPosition { //Это Класс нашего положения в Игре
//private:
public:
    std::vector<int> Position; //Та ситуация, в которой мы находимся
    int Level;
    int DatDistance;
    std::vector<int> OurParent; //Родительская ситуация
    
    bool IsWinPosition();
    int CountDistance();
    int GetWeight();
    int CountDistanceNew();
    void CheckNewPosition(FieldPosition &Minimal, std::set<FieldPosition> &Opened, std::map<std::vector<int>, std::pair<int, std::vector<int>>> &DatMap);
    FieldPosition MakeNewNeighbor(const int &WhereIsNull, const short &Shift) const;
    void FindTheWay(std::vector<std::string> &OurWay, std::map<std::vector<int>, std::pair<int, std::vector<int>>> &OurMap);

    

   
    
    FieldPosition(std::vector<int> &ToCreate);
    FieldPosition(std::vector<int> &ToCreate, const int &NewLevel, const std::vector<int> &Parent);
    
    bool operator <(const FieldPosition &ToCompare) const;
};

class GameSolver{
private:
    std::vector<std::string> OurWay;
    std::set<FieldPosition> OpenedPositions;
    std::map<std::vector<int>, std::pair<int, std::vector<int>>> OurMap;
public:
    void Run(FieldPosition& StartPosition);
};



void GameSolver::Run(FieldPosition& StartPosition) {
    
    OpenedPositions.insert(StartPosition);
    OurMap.insert(std::make_pair(StartPosition.Position, std::make_pair(StartPosition.GetWeight(), StartPosition.OurParent)));
    
    while (!OpenedPositions.empty()) {
        FieldPosition Minimal = *OpenedPositions.begin();
        OpenedPositions.erase(OpenedPositions.begin());
        
        if(Minimal.IsWinPosition()) {
            Minimal.FindTheWay(OurWay, OurMap);
            break;
        }
        
        int WhereIsNull = std::find(Minimal.Position.begin(), Minimal.Position.end(), 0) - Minimal.Position.begin();
        
        if(WhereIsNull % 3 != 2) {
            FieldPosition Neighbor = Minimal.MakeNewNeighbor(WhereIsNull, 1);
            Neighbor.CheckNewPosition(Minimal, OpenedPositions, OurMap);
        }
        
        if(WhereIsNull % 3 != 0) {
            FieldPosition Neighbor = Minimal.MakeNewNeighbor(WhereIsNull, -1);
            Neighbor.CheckNewPosition(Minimal, OpenedPositions, OurMap);
        }
        
        if(WhereIsNull / 3 != 2) {
            FieldPosition Neighbor = Minimal.MakeNewNeighbor(WhereIsNull, 3);
            Neighbor.CheckNewPosition(Minimal, OpenedPositions, OurMap);
        }
        
        if(WhereIsNull / 3 != 0) {
            FieldPosition Neighbor = Minimal.MakeNewNeighbor(WhereIsNull, -3);
            Neighbor.CheckNewPosition(Minimal, OpenedPositions, OurMap);
        }
    }
    
    std::reverse(OurWay.begin(), OurWay.end());
    for(int i = 0; i < OurWay.size(); ++i) {
        
        std::cout << OurWay[i] << "\n";
    }
    
}



bool FieldPosition::operator<(const FieldPosition &ToCompare) const {
    if(this -> Level + this -> DatDistance == ToCompare.Level + ToCompare.DatDistance) {
        return this -> Position > ToCompare.Position;
    }
    return this -> Level + this -> DatDistance < ToCompare.Level + ToCompare.DatDistance;
}

int FieldPosition::CountDistance() { //Подсчет не стоящих на своем месте
    int Result = 0;
    for(int TruePosition = 0; TruePosition < FieldSize; ++TruePosition) {
        if(!((this -> Position)[TruePosition]  == TruePosition))
            ++Result;
    }
    return Result;
}

int FieldPosition::GetWeight() {
    return this -> DatDistance + this -> Level;
}


FieldPosition::FieldPosition(std::vector<int> &ToCreate, const int &NewLevel, const std::vector<int> &Parent) {
    Position = ToCreate;
    Level = NewLevel;
    DatDistance = CountDistance();
    OurParent = Parent;
}

FieldPosition::FieldPosition(std::vector<int> &ToCreate) {
    Position = ToCreate;
    Level = 0;
    DatDistance = CountDistance();
    OurParent = {};
}


bool FieldPosition::IsWinPosition() {
    if(CountDistance() == 0)
        return true;
    else
        return false;
    
}

int CountDistance(std::vector<int>& ToCount) {
    int Result = 0;
    for(int TruePosition = 0; TruePosition < FieldSize; ++TruePosition) {
        if(!(ToCount[TruePosition]  == TruePosition))
            ++Result;
    }
    return Result;
}

int FieldPosition::CountDistanceNew() {
    int Result = 0;
    std::vector<int> ToCount = this -> Position;
    for(int i = 0; i < FieldSize; ++i) {
        int RealPosition = ToCount[i] ;
        Result += abs(RealPosition / 3 - i / 3) + abs(RealPosition % 3 - i % 3);
    }
    return Result;
}

FieldPosition FieldPosition::MakeNewNeighbor(const int &WhereIsNull, const short &Shift) const {
    std::vector<int> NeighborPosition = this -> Position;
    std::swap(NeighborPosition[WhereIsNull], NeighborPosition[WhereIsNull + Shift]);
    FieldPosition Neighbor = FieldPosition(NeighborPosition, this -> Level + 1, this -> Position);
    return Neighbor;
}

void FieldPosition::FindTheWay(std::vector<std::string> &OurWay, std::map<std::vector<int>, std::pair<int, std::vector<int>>> &OurMap) {
        std::cout << Level << "\n";
        std::vector<int> Check = OurParent;
        if(Level == 0)
            return;
    
 
    
        int WhereIsNullParent = std::find(Check.begin(), Check.end(), 0) - Check.begin();
    
        if(WhereIsNullParent == 1)
            OurWay.push_back("LEFT");
        else
            OurWay.push_back("UP");
    
  
    
    
    
//    std::cout << "\n";

        int Counter = Level - 1;
        while (Counter > 0) {
            auto OurElement = OurMap.find(Check);
//            for(int i = 0; i < 9; ++i) {
//                std::cout << Check[i] << " ";
//                if(i % 3 ==  2)
//                    std::cout << "\n";
//            }
            
            
//            std::cout << "\n";
            
            
            int WhereIsNullSon = std::find(Check.begin(), Check.end(), 0) - Check.begin();
            int WhereIsNullParent = std::find((OurElement -> second.second).begin(), (OurElement -> second.second).end(), 0) - (OurElement -> second.second).begin();
            
            if(WhereIsNullParent - WhereIsNullSon == 3)
                OurWay.push_back("UP");
            else if(WhereIsNullParent - WhereIsNullSon == -3)
                OurWay.push_back("DOWN");
            else if(WhereIsNullParent - WhereIsNullSon == 1)
                OurWay.push_back("LEFT");
            else if(WhereIsNullParent - WhereIsNullSon == -1)
                OurWay.push_back("RIGHT");
            
            Check = OurElement -> second.second;
            --Counter;
            
        }
}



//Эта функция проверяет, есть ли решение


//bool HasSolution(std::vector<int> &StartPosition) {
//    int Result = 0;
//    std::vector<int> OurVector(FieldSize - 1);
//    FenwickTree OurTree = FenwickTree(FieldSize - 1);
//    
//    int OurNumber;
//    for(int i = 0; i < FieldSize; ++i) {
//        if(StartPosition[i] == 0)
//            continue;
//        
//        OurNumber = StartPosition[i] ;
//        OurVector[i] = OurNumber;
//    }
//    
//    for(int Position = 0; Position < FieldSize - 1; ++Position){
//        Result += Position - OurTree.Sum(OurVector[Position] - 1);
//        OurTree.Update(OurVector[Position] - 1, 1);
//    }
//    
//    int WhereIsNull = std::find(StartPosition.begin(), StartPosition.end(), 0) - StartPosition.begin();
//    
//    Result += (WhereIsNull / 3) + 1;
//    
//    if(Result % 2 == 0)
//        return true;
//    else
//        return false;
//}


void FieldPosition::CheckNewPosition(FieldPosition &Minimal, std::set<FieldPosition> &Opened, std::map<std::vector<int>, std::pair<int, std::vector<int>>> &DatMap) {
    auto OurElement = DatMap.find(Position);
    if(OurElement != DatMap.end()) {
        int OldDistance = OurElement -> second.first;
        if(GetWeight() < OldDistance) {
            FieldPosition ToDelete = FieldPosition(Position, OurElement -> second.first - CountDistance(), Minimal.Position);
            
            Opened.erase(ToDelete);
            Opened.insert(*this);
            
            OurElement -> second.first = GetWeight();
        }
    }
    else {
        Opened.insert(*this);
        DatMap.insert(std::make_pair(Position, std::make_pair(GetWeight(), OurParent)));
    }

}


int main(int argc, const char * argv[]) {
    
    int FieldSize;
    std::vector<int> StartPosition;
    std::cin >> FieldSize;
    
    if(FieldSize != 3) {
        std::cout << "0";
        return 0;
    }
    FieldSize *= FieldSize;
    for(int i = 0; i < FieldSize; ++i) {
        int OurNumber;
        std::cin >> OurNumber;
        StartPosition.push_back( OurNumber);
    }
    
    FieldPosition StartField = FieldPosition(StartPosition);
    
    GameSolver Bubu;
    Bubu.Run(StartField);
    return 0;
}

