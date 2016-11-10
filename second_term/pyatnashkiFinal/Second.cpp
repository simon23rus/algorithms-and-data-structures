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
#include <chrono>


class FenwickTree {
    private :
    std::vector<int> OurTree;
    void FenwickAND(int &OurPosition);
    void FenwickOR(int &OurPosition);
    public :
    FenwickTree(const int &size) {
        OurTree.assign(size, 0);
    }
    void Update(const int &Position, const int &delta);
    int Sum(const int &Position);
    int SegmentSum(const int &StartPosition, const int &EndPosition);
};

void FenwickTree::FenwickAND(int &OurPosition) {
    OurPosition = (OurPosition & (OurPosition + 1)) - 1;
}

void FenwickTree::FenwickOR(int &OurPosition) {
    OurPosition |= (OurPosition + 1);
}

int FenwickTree::Sum(const int &Position) {
    int NewPos = Position;
    int OurRes = 0;
    for(; NewPos >= 0; FenwickAND(NewPos)) {
        OurRes += this -> OurTree[NewPos];
    }
    return OurRes;
}

int FenwickTree::SegmentSum(const int &StartPosition, const int &EndPosition) {
    return Sum(EndPosition) - Sum(StartPosition - 1);
}



void FenwickTree::Update(const int &Position, const int &delta) {
    int NewPos = Position;
    for(; NewPos < this -> OurTree.size(); FenwickOR(NewPos)) {
        this -> OurTree[NewPos] += delta;
    }
    
}




//FinalPosition =
/*
 1 2 3 4
 5 6 7 8
 9 10 11 12
 13 14 15 0
 */
int FieldSize = 16;
int RowNumber = 4;

std::vector<int> LinConflicts;
std::vector<int> FinalPosition = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};

void Random(std::vector<int> &ToShuffle, int HowManyShuffles) {
    srand(time(0));
    for(int i = 0; i < HowManyShuffles ; ++i) {
        
        
        int WhereIsNull = std::find(ToShuffle.begin(), ToShuffle.end(), 0) - ToShuffle.begin();
        int bubu = rand() % 4;
        if(bubu == 0 && WhereIsNull % RowNumber != RowNumber - 1) {
            std::swap(ToShuffle[WhereIsNull], ToShuffle[WhereIsNull + 1]);
        }
        else if(bubu == 1 && WhereIsNull % RowNumber != 0) {
            std::swap(ToShuffle[WhereIsNull], ToShuffle[WhereIsNull - 1]);
        }
        else if(bubu == 2 && WhereIsNull / RowNumber != 0) {
            std::swap(ToShuffle[WhereIsNull], ToShuffle[WhereIsNull - RowNumber]);
        }
        else if(bubu == 3 && WhereIsNull / RowNumber != RowNumber - 1) {
            std::swap(ToShuffle[WhereIsNull], ToShuffle[WhereIsNull + RowNumber]);
        }
    }
//    
//    for(int i = 0; i < ToShuffle.size(); ++i) {
//        std::cout << ToShuffle[i] << " ";
//        if(i % RowNumber == RowNumber - 1)
//            std::cout << "\n";
//    }
//    std::cout << "\n\n";
}


class FieldPosition { //Ð­Ñ‚Ð¾ ÐšÐ»Ð°ÑÑ Ð½Ð°ÑˆÐµÐ³Ð¾ Ð¿Ð¾Ð»Ð¾Ð¶ÐµÐ½Ð¸Ñ Ð² Ð˜Ð³Ñ€Ðµ
    //private:
public:
    std::vector<int> Position; //Ð¢Ð° ÑÐ¸Ñ‚ÑƒÐ°Ñ†Ð¸Ñ, Ð² ÐºÐ¾Ñ‚Ð¾Ñ€Ð¾Ð¹ Ð¼Ñ‹ Ð½Ð°Ñ…Ð¾Ð´Ð¸Ð¼ÑÑ
    int Level;
    int DatDistance;
    std::vector<int> OurParent; //Ð Ð¾Ð´Ð¸Ñ‚ÐµÐ»ÑŒÑÐºÐ°Ñ ÑÐ¸Ñ‚ÑƒÐ°Ñ†Ð¸Ñ
    
    bool IsWinPosition();
    int CountDistance();
    int GetWeight();
    int CountDistanceNew();
    int LinearConflict();
    int CornerConflict();
    int TilesOut();
    int ConflictLinear();
    
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
    ~GameSolver();
    std::vector<std::string> Run(FieldPosition& StartPosition);
    bool idaStar(FieldPosition &StartPosition);
    int idaStarSearch(FieldPosition &StartPosition, FieldPosition& OurParent, int Distance ,int Bound);
    std::vector<std::string> RunIDA(FieldPosition &StartPosition);
};

std::vector<std::string> GameSolver::RunIDA(FieldPosition &StartPosition) {
    bool Result = idaStar(StartPosition);
    if(Result)
        return OurWay;
    else
        return {};
}

int FieldPosition::ConflictLinear(){
    int Result = 0;
    
    for(int i = 0; i< RowNumber; ++i)
    {
        for(int first = 0; first < RowNumber; ++first)
        {
            // rows distance
            // the right position of this (first)  cell is in this line
            if(Position[i*RowNumber + first] > i*RowNumber && Position[i*RowNumber + first] <= (i + 1)*RowNumber)
            {
                for(int second = first + 1; second < RowNumber; ++second)
                {
                    //if the right position of this (second)  cell is in this line
                    if(Position[i*RowNumber + second] > i*RowNumber && Position[i*RowNumber + second] <= (i + 1)*RowNumber)
                    {
                        if(Position[i*RowNumber + first] > Position[i*RowNumber + second])
                        {
                            if(Position[i*RowNumber + first] != 0 && Position[i*RowNumber + second] != 0)
                            {
                                Result += 2;
                                LinConflicts.push_back(i*RowNumber + first);
                                LinConflicts.push_back(i*RowNumber + second);
                            }
                        }
                    }
                }
            }
            
            //column distance
            if(Position[first*RowNumber + i] % RowNumber == (i + 1) % RowNumber) // the right position of this cell is in this column
            {
                for(int second = first + 1; second < RowNumber; ++second)
                {
                    if(Position[second * RowNumber + i] % RowNumber == (i + 1) % RowNumber)
                    {
                        if(Position[first*RowNumber + i] > Position[second*RowNumber + i])
                        {
                            if(Position[first * RowNumber + i] != 0 && Position[second * RowNumber + i] != 0)
                            {
                                Result += 2;
                                LinConflicts.push_back(first * RowNumber + i);
                                LinConflicts.push_back(second * RowNumber + i);
                            }
                        }
                    }
                }
            }
        }
    }
    return Result;

}


bool GameSolver::idaStar(FieldPosition &StartPosition) {
    int Bound = StartPosition.CountDistance();
    while (true) {
        int IsSolution = idaStarSearch(StartPosition, StartPosition, 0, Bound);
        if (IsSolution == 1){
            std::reverse(OurWay.begin(), OurWay.end());
            return true;
        }
        if (IsSolution == INT32_MAX){
            return false;
        }
        Bound = IsSolution;
    }
}


int GameSolver::idaStarSearch(FieldPosition &NowPosition, FieldPosition &Parent,
                                    int OurDistance, int Bound){
    int Weight = OurDistance + NowPosition.CountDistance();
    if (Weight > Bound){
        return Weight;
    }
    if (NowPosition.CountDistance() == 0){
        OurWay.clear();
        return 1;
    }
    int Minimal = INT32_MAX;
    int WhereIsNull = std::find(NowPosition.Position.begin(), NowPosition.Position.end(), 0) - NowPosition.Position.begin();
    if(WhereIsNull % RowNumber != RowNumber - 1) {
        FieldPosition Neighbor = NowPosition.MakeNewNeighbor(WhereIsNull, 1);
        if(Neighbor.Position != Parent.Position) {
            int OurResult = idaStarSearch(Neighbor, NowPosition, OurDistance + 1, Bound);
            Minimal = std::min(OurResult, INT32_MAX);
            if(OurResult == 1) {
                OurWay.push_back("R");
                return 1;
            }
        }
    }
    if(WhereIsNull % RowNumber != 0) {
        FieldPosition Neighbor = NowPosition.MakeNewNeighbor(WhereIsNull, -1);
        if(Neighbor.Position != Parent.Position) {
            int OurResult = idaStarSearch(Neighbor, NowPosition, OurDistance + 1, Bound);
            Minimal = std::min(OurResult, INT32_MAX);
            if(OurResult == 1) {
                OurWay.push_back("L");
                return 1;
            }
        }
    }
    if(WhereIsNull / RowNumber != RowNumber - 1) {
        FieldPosition Neighbor = NowPosition.MakeNewNeighbor(WhereIsNull, RowNumber);
        if(Neighbor.Position != Parent.Position) {
            int OurResult = idaStarSearch(Neighbor, NowPosition, OurDistance + 1, Bound);
            Minimal = std::min(OurResult, INT32_MAX);
            if(OurResult == 1) {
                OurWay.push_back("D");
                return 1;
            }
        }
    }
    if(WhereIsNull / RowNumber != 0) {
        FieldPosition Neighbor = NowPosition.MakeNewNeighbor(WhereIsNull, -RowNumber);
        if(Neighbor.Position != Parent.Position) {
            int OurResult = idaStarSearch(Neighbor, NowPosition, OurDistance + 1, Bound);
            Minimal = std::min(OurResult, INT32_MAX);
            if(OurResult == 1) {
                OurWay.push_back("U");
                return 1;
            }
        }
    }
    return Minimal;
}




GameSolver::~GameSolver(){
    OurWay.clear();
    OpenedPositions.clear();
    OurMap.clear();
}


std::vector<std::string> GameSolver::Run(FieldPosition& StartPosition) {
    
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
        
        if(WhereIsNull % RowNumber != RowNumber - 1) {
            FieldPosition Neighbor = Minimal.MakeNewNeighbor(WhereIsNull, 1);
            Neighbor.CheckNewPosition(Minimal, OpenedPositions, OurMap);
        }
        
        if(WhereIsNull % RowNumber != 0) {
            FieldPosition Neighbor = Minimal.MakeNewNeighbor(WhereIsNull, -1);
            Neighbor.CheckNewPosition(Minimal, OpenedPositions, OurMap);
        }
        
        if(WhereIsNull / RowNumber != RowNumber - 1) {
            FieldPosition Neighbor = Minimal.MakeNewNeighbor(WhereIsNull, RowNumber);
            Neighbor.CheckNewPosition(Minimal, OpenedPositions, OurMap);
        }
        
        if(WhereIsNull / RowNumber != 0) {
            FieldPosition Neighbor = Minimal.MakeNewNeighbor(WhereIsNull, -RowNumber);
            Neighbor.CheckNewPosition(Minimal, OpenedPositions, OurMap);
        }
    }
    
    std::reverse(OurWay.begin(), OurWay.end());
    
    return OurWay;
}


bool Find(const int &ToFind) {
    for(int i = 0; i < LinConflicts.size(); ++i) {
        if(LinConflicts[i] == ToFind)
            return true;
    }
    return false;
}



int FieldPosition::LinearConflict() { //ÐŸÐ¾Ð´Ð¿Ñ€Ð°Ð²Ð¸Ñ‚ÑŒ
    std::vector<int> ToCount = this -> Position;
    LinConflicts.clear();
    int Result = 0;
    for(int i = 0 ; i < FieldSize; ++i) {
        if (ToCount[i] == 0)
            continue;
        if((ToCount[i] - 1) / RowNumber == i / RowNumber) {
            for(int j = i - i % RowNumber; j < (i - i % RowNumber) + RowNumber; ++j) {
                if(j != i && (ToCount[j] - 1) / RowNumber == j / RowNumber && ToCount[i] > ToCount[j] && i < j) {
                    Result += 2;
                    LinConflicts.push_back(ToCount[i]);
                    LinConflicts.push_back(ToCount[j]);
                }
            }
        }
        
        if(ToCount[i] % RowNumber == (i + 1) % RowNumber && !Find(ToCount[i])) {
            for(int j = i % RowNumber; j <= i % RowNumber + RowNumber * (RowNumber - 1); j += RowNumber) {
                if(j != i && ToCount[j] % RowNumber == (j + 1) % RowNumber && ToCount[i] > ToCount[j] && i < j && ToCount[j] != 0) {
                    Result += 2;
                    LinConflicts.push_back(ToCount[i]);
                    LinConflicts.push_back(ToCount[j]);
                }
            }
        }
        
    }
    return Result;
}


int FieldPosition::CornerConflict() {
    std::vector<int> ToCount = this -> Position;
    int Result = 0;
    
    if(ToCount[1] == 2 && ToCount[RowNumber] == RowNumber + 1 && ToCount[0] != 1 && !Find(ToCount[1]) && !Find(ToCount[0]) && !Find(ToCount[RowNumber]))
        Result += 2;
    
    if(ToCount[RowNumber - 2] == RowNumber - 1 && ToCount[2 * RowNumber - 1] == 2 * RowNumber && ToCount[RowNumber - 1] != RowNumber && !Find(ToCount[RowNumber - 2]) && !Find(ToCount[2 * RowNumber -1]) && !Find(ToCount[RowNumber - 1]))
        Result += 2;
    
    if(ToCount[RowNumber * (RowNumber - 2)] == RowNumber * (RowNumber - 2) + 1 && ToCount[RowNumber * (RowNumber - 1) + 1] == RowNumber * (RowNumber - 1) + 2 && ToCount[RowNumber * (RowNumber - 1)] != RowNumber * (RowNumber - 1) + 1 && !Find(ToCount[RowNumber * (RowNumber - 1)]) && !Find(ToCount [RowNumber * (RowNumber - 1) + 1]) && !Find(ToCount[RowNumber * (RowNumber - 2)]))
        Result += 2;
    
    
    if(ToCount[RowNumber * (RowNumber - 1) - 1] == RowNumber * (RowNumber - 1) && ToCount[RowNumber * RowNumber - 2] == RowNumber * RowNumber - 1 && ToCount[RowNumber * RowNumber - 1] != 0 && !Find(ToCount[RowNumber * (RowNumber - 1) - 1]) && !Find(ToCount[RowNumber * RowNumber - 2]) && !Find(ToCount[RowNumber * RowNumber - 1]))
        Result += 2;
    
    LinConflicts.clear();
    return Result;
}


int FieldPosition::TilesOut() {
    std::vector<int> ToCount = this -> Position;
    int Result = 0;
    for(int i = 0; i < FieldSize; ++i) {
        if(ToCount[i] == 0) {
            if(i / RowNumber != RowNumber - 1)
                ++Result;
            if((i + 1) % RowNumber != 0)
                ++Result;
        }
        else {
            if((ToCount[i] - 1) / RowNumber != i / RowNumber)
                ++Result;
            if(ToCount[i] % RowNumber != (i + 1) % RowNumber)
                ++Result;
        }
    }
    return Result;
}



bool FieldPosition::operator<(const FieldPosition &ToCompare) const {
    if(this -> Level + this -> DatDistance == ToCompare.Level + ToCompare.DatDistance) {
        return this -> Position < ToCompare.Position;
    }
    return this -> Level + this -> DatDistance < ToCompare.Level + ToCompare.DatDistance;
}

int FieldPosition::CountDistance() { //ÐŸÐ¾Ð´ÑÑ‡ÐµÑ‚ Ð´Ð¸ÑÑ‚Ð°Ð½Ñ†Ð¸Ð¸
    int Result = 0;
//    for(int TruePosition = 0; TruePosition < FieldSize; ++TruePosition) {
//        if(!((this -> Position)[TruePosition]  == TruePosition + 1) && (this -> Position)[TruePosition] != 0)
//            ++Result;
//
//    }
//    if((this -> Position)[FieldSize - 1] != 0)
//        ++Result;
//    Result += TilesOut();
    Result += ConflictLinear();
    Result += CountDistanceNew();
    Result += CornerConflict();
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
        if(ToCount[i] != 0) {
            int RealPosition = ToCount[i] - 1;
            Result += abs(RealPosition / RowNumber - i / RowNumber) + abs(RealPosition % RowNumber - i % RowNumber);
        }
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
    //    std::cout << Level << "\n";
    std::vector<int> Check = OurParent;
    if(Level == 0)
        return;
    
    int WhereIsNullParent = std::find(Check.begin(), Check.end(), 0) - Check.begin();
    
    if(WhereIsNullParent == FieldSize - 2)
        OurWay.push_back("R");
    else
        OurWay.push_back("D");
    
    int Counter = Level - 1;
    while (Counter > 0) {
        auto OurElement = OurMap.find(Check);
        
        //        for(int i = 0; i < FieldSize; ++i) {
        //            std::cout << Check[i] << " ";
        //            if(i % RowNumber == RowNumber - 1)
        //                std::cout << "\n";
        //        }
        //        std::cout << "\n";
        
        int WhereIsNullSon = std::find(Check.begin(), Check.end(), 0) - Check.begin();
        int WhereIsNullParent = std::find((OurElement -> second.second).begin(), (OurElement -> second.second).end(), 0) - (OurElement -> second.second).begin();
        
        if(WhereIsNullParent - WhereIsNullSon == RowNumber)
            OurWay.push_back("U");
        else if(WhereIsNullParent - WhereIsNullSon == -RowNumber)
            OurWay.push_back("D");
        else if(WhereIsNullParent - WhereIsNullSon == 1)
            OurWay.push_back("L");
        else if(WhereIsNullParent - WhereIsNullSon == -1)
            OurWay.push_back("R");
        
        Check = OurElement -> second.second;
        --Counter;
        
    }
}



//Ð­Ñ‚Ð° Ñ„ÑƒÐ½ÐºÑ†Ð¸Ñ Ð¿Ñ€Ð¾Ð²ÐµÑ€ÑÐµÑ‚, ÐµÑÑ‚ÑŒ Ð»Ð¸ Ñ€ÐµÑˆÐµÐ½Ð¸Ðµ


bool HasSolution(std::vector<int> &StartPosition) {
    int Result = 0;
    std::vector<int> OurVector;
    FenwickTree OurTree = FenwickTree(FieldSize - 1);
    
    int OurNumber;
    for(int i = 0; i < FieldSize; ++i) {
        if(StartPosition[i] == 0)
            continue;
        
        OurNumber = StartPosition[i] ;
        OurVector.push_back(OurNumber);
    }
    
    for(int Position = 0; Position < FieldSize - 1; ++Position){
        Result += Position - OurTree.Sum(OurVector[Position] - 1);
        OurTree.Update(OurVector[Position] - 1, 1);
    }
    
    
    int WhereIsNull = std::find(StartPosition.begin(), StartPosition.end(), 0) - StartPosition.begin();
    
    Result += (WhereIsNull / RowNumber) + 1;
    
    if(Result % 2 == 0)
        return true;
    else
        return false;
}


void FieldPosition::CheckNewPosition(FieldPosition &Minimal, std::set<FieldPosition> &Opened, std::map<std::vector<int>, std::pair<int, std::vector<int>>> &DatMap) {
    auto OurElement = DatMap.find(Position);
    if(OurElement != DatMap.end()) {
        int OldDistance = OurElement -> second.first;
        int jo;
        
        
        if((jo = GetWeight()) < OldDistance) {
            FieldPosition ToDelete = FieldPosition(Position, OurElement -> second.first - CountDistance(), Minimal.Position);
            
            
            Opened.erase(ToDelete);
            Opened.insert(*this);
            
            OurElement -> second.first = GetWeight();
            OurElement -> second.second = OurParent;
            
        }
    }
    else {
        Opened.insert(*this);
        DatMap.insert(std::make_pair(Position, std::make_pair(GetWeight(), OurParent)));
    }
    
}

bool CheckSolution(std::vector<int> &StartPosition, std::vector<std::string> &Answer){
    int WhereIsNull = std::find(StartPosition.begin(), StartPosition.end(), 0) - StartPosition.begin();
    for(int i = 0; i < Answer.size(); ++i){
        if(Answer[i] == "R"){
            std::swap(StartPosition[WhereIsNull], StartPosition[WhereIsNull + 1]);
            WhereIsNull += 1;
        }
        else if(Answer[i] == "L"){
            std::swap(StartPosition[WhereIsNull], StartPosition[WhereIsNull - 1]);
            WhereIsNull -= 1;

        }
        else if(Answer[i] == "U"){
            std::swap(StartPosition[WhereIsNull], StartPosition[WhereIsNull - RowNumber]);
            WhereIsNull -= RowNumber;
        }
        else if(Answer[i] == "D"){
            std::swap(StartPosition[WhereIsNull], StartPosition[WhereIsNull + RowNumber]);
            WhereIsNull += RowNumber;

        }
        
    }
    
    
    return StartPosition == FinalPosition;
}


int main(int argc, const char * argv[]) {
    //    std::ios_base::sync_with_stdio(false);
    int GameMode;
    std::cin >> GameMode;
    RowNumber = 4;
    double AllTime;
    
    FieldSize = RowNumber * RowNumber;
    std::vector<int> StartPosition(FieldSize);
    
    for(int j = 0; j < GameMode; ++j) {
        for(int i = 0; i < FieldSize; ++i) {
            int OurNumber;
            std::cin >> OurNumber;
            StartPosition[i] = OurNumber;
            
        }
//
        if(!HasSolution(StartPosition)) {
            std::cout << "This puzzle is not solvable.\n";
            continue;
        }
        
//        Random(StartPosition, 100);
        
        FieldPosition StartField = FieldPosition(StartPosition);
        
        GameSolver SolveMaster;
        
        std::vector<std::string> Answer;
        auto Start = std::chrono::steady_clock::now();
//        Answer = Bubu.Run(StartField);
        Answer = SolveMaster.RunIDA(StartField);
        auto End = std::chrono::steady_clock::now();

        
        for(int i = 0; i < Answer.size(); ++i){
            std::cout << Answer[i];
        }
            std::cout << "\n";
        
//        std::cout << "Time is = " << std::chrono::duration_cast<std::chrono::microseconds>(End - Start).count() * 0.000001 << "\n";
//        if(!CheckSolution(StartPosition, Answer))
//            std::cout << "\nVse ochen' ploho\n";

    }
    
    
    return 0;
}
