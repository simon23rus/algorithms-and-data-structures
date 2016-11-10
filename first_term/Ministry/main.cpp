//
//  main.cpp
//  Ministry
//
//  Created by Simon Fedotov on 04.12.14.
//  Copyright (c) 2014 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>

void FindTheWay(const std::vector<std::vector<long long>> &OurMin, const std::vector<std::vector<size_t>> &OurTrace, std::vector<size_t>& JustOurWay, size_t RoomNumber, size_t FloorNumber) {
    size_t OurPosition = 0;
    for(size_t i = 1; i < RoomNumber; ++i) {
        if(OurMin[FloorNumber - 1][i] < OurMin[FloorNumber - 1][OurPosition])
            OurPosition = i;
    }
    ptrdiff_t WhereAmI = FloorNumber - 1;
    JustOurWay.push_back(OurPosition);
    while (WhereAmI > 0) {
        if(OurPosition == OurTrace[WhereAmI][OurPosition]) {
                JustOurWay.push_back(OurTrace[WhereAmI][OurPosition]);
                --WhereAmI;
        }
        else {
            JustOurWay.push_back(OurTrace[WhereAmI][OurPosition]);
            OurPosition = OurTrace[WhereAmI][OurPosition];
        }
    }
}

void OurSolve(std::vector<std::vector<long long>> &OurMin, const std::vector<std::vector<int>> &OurCosts, std::vector<std::vector<size_t>> &OurTrace, size_t RoomNumber, size_t FloorNumber) {
    for(ptrdiff_t i = 0; i < RoomNumber; ++i) {
        OurMin[0][i] = OurCosts[0][i];
        OurTrace[0][i] = i;
    }
    for(ptrdiff_t OurFloor = 1; OurFloor < FloorNumber; ++OurFloor) {
        OurMin[OurFloor][0] = OurMin[OurFloor - 1][0] + OurCosts[OurFloor][0];
        OurTrace[OurFloor][0] = 0;
        for(ptrdiff_t OurRoom = 1; OurRoom < RoomNumber; ++OurRoom) {
            if(OurMin[OurFloor - 1][OurRoom] < OurMin[OurFloor][OurRoom - 1]) {
                OurMin[OurFloor][OurRoom] = OurMin[OurFloor - 1][OurRoom] + OurCosts[OurFloor][OurRoom];
                OurTrace[OurFloor][OurRoom] = OurRoom;
                
            }
            else {
                OurMin[OurFloor][OurRoom] = OurMin[OurFloor][OurRoom - 1] + OurCosts[OurFloor][OurRoom];
                OurTrace[OurFloor][OurRoom] = OurRoom - 1;
                
            }
        }
        for(ptrdiff_t RoomsAgain = RoomNumber - 2; RoomsAgain >= 0; --RoomsAgain) {
            if(OurMin[OurFloor][RoomsAgain + 1] + OurCosts[OurFloor][RoomsAgain] < OurMin[OurFloor][RoomsAgain]) {
                OurMin[OurFloor][RoomsAgain] = OurMin[OurFloor][RoomsAgain + 1] + OurCosts[OurFloor][RoomsAgain];
                OurTrace[OurFloor][RoomsAgain] = RoomsAgain + 1;
            }
        }
    }

}



int main() {
    std::ios_base::sync_with_stdio(false);
    size_t FloorNumber, RoomNumber;
    std::cin >> FloorNumber >> RoomNumber;
    std::vector<std::vector<int>> OurPrices;
    OurPrices.resize(FloorNumber);
    for(ptrdiff_t i = FloorNumber - 1; i >= 0 ; --i) {
        for(ptrdiff_t j = 0; j < RoomNumber; ++j) {
            int OurCost;
            std::cin >> OurCost;
            OurPrices[i].push_back(OurCost);
        }
    }
    std::vector<std::vector<long long>> OurMinimal;
    OurMinimal.resize(FloorNumber);
    for(size_t i = 0; i < FloorNumber; ++i)
        OurMinimal[i].resize(RoomNumber);
    
    std::vector<std::vector<size_t>> OurWay;
    OurWay.resize(FloorNumber);
    for(size_t i = 0; i < FloorNumber; ++i)
        OurWay[i].resize(RoomNumber);
    
    OurSolve(OurMinimal, OurPrices, OurWay, RoomNumber, FloorNumber);
    std::vector<size_t> OurSteps;
    FindTheWay(OurMinimal, OurWay, OurSteps, RoomNumber, FloorNumber);
    for(size_t i = 0; i < OurSteps.size(); ++i)
        std::cout << OurSteps[i] + 1 << " ";
    return 0;
}
