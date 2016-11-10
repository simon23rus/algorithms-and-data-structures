//
//  main.cpp
//  UFO_Task
//
//  Created by Simon Fedotov on 07.12.14.
//  Copyright (c) 2014 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>


void FenwickAND(ptrdiff_t &OurPosition) {
    OurPosition = (OurPosition & (OurPosition + 1)) - 1;
}

void FenwickOR(size_t &OurPosition) {
    OurPosition |= (OurPosition + 1);
}



void ChangeElement(std::vector<std::vector<std::vector<int>>> &DatField, size_t FirstDim, size_t SecondDim, size_t ThirdDim, int Delta) {
    for(size_t OurX = FirstDim; OurX < DatField.size(); FenwickOR(OurX))
        for(size_t OurY = SecondDim; OurY < DatField.size(); FenwickOR(OurY))
            for(size_t OurZ = ThirdDim; OurZ < DatField.size(); FenwickOR(OurZ))
                DatField[OurX][OurY][OurZ] += Delta;
}

int UfoNumber(const std::vector<std::vector<std::vector<int>>> &DatField, ptrdiff_t FirstDim, ptrdiff_t SecondDim, ptrdiff_t ThirdDim) {
    if(FirstDim < 0 || FirstDim >= DatField.size())
        return 0;
    if(SecondDim < 0 || SecondDim >= DatField.size())
        return 0;
    if(ThirdDim < 0 || ThirdDim >= DatField.size())
        return 0;
    int OurResult = 0;
    for(ptrdiff_t OurX = FirstDim; OurX >= 0; FenwickAND(OurX))
        for(ptrdiff_t OurY = SecondDim; OurY >= 0; FenwickAND(OurY))
            for(ptrdiff_t OurZ = ThirdDim; OurZ >= 0; FenwickAND(OurZ))
                OurResult += DatField[OurX][OurY][OurZ];
    return OurResult;
}

int UfoNumber(const std::vector<std::vector<std::vector<int>>> &DatField, ptrdiff_t FirstPoint1Dim, ptrdiff_t FirstPoint2Dim, ptrdiff_t FirstPoint3Dim, ptrdiff_t SecondPoint1Dim, ptrdiff_t SecondPoint2Dim, ptrdiff_t SecondPoint3Dim) {
        return
          UfoNumber(DatField, SecondPoint1Dim, SecondPoint2Dim, SecondPoint3Dim)
        - UfoNumber(DatField, SecondPoint1Dim, FirstPoint2Dim - 1, SecondPoint3Dim)
        - UfoNumber(DatField, FirstPoint1Dim - 1, SecondPoint2Dim, SecondPoint3Dim)
        - UfoNumber(DatField, SecondPoint1Dim, SecondPoint2Dim, FirstPoint3Dim - 1)
        + UfoNumber(DatField, FirstPoint1Dim - 1, SecondPoint2Dim, FirstPoint3Dim - 1)
        + UfoNumber(DatField, FirstPoint1Dim - 1, FirstPoint2Dim - 1, SecondPoint3Dim)
        + UfoNumber(DatField, SecondPoint1Dim, FirstPoint2Dim - 1, FirstPoint3Dim - 1)
        - UfoNumber(DatField, FirstPoint1Dim - 1, FirstPoint2Dim - 1, FirstPoint3Dim - 1);
}

void MakeField(std::vector<std::vector<std::vector<int>>> &DatField, size_t FieldSize) {
    DatField.resize(FieldSize);
    for(size_t i = 0; i < FieldSize; i++) {
        DatField[i].resize(FieldSize);
    }
    for(size_t i = 0; i < FieldSize; i++) {
        for(size_t j = 0; j < FieldSize; j++)
            DatField[i][j].resize(FieldSize);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t OurSize;
    std::cin >> OurSize;
    std::vector<std::vector<std::vector<int>>> OurField;
    MakeField(OurField, OurSize);
    short OurCommand;
    while(OurCommand != 3) {
        std::cin >> OurCommand;
        if(OurCommand == 1) {
            size_t OurX, OurY, OurZ;
            int UFOsDifference;
            std::cin >> OurX >> OurY >> OurZ >> UFOsDifference;
            ChangeElement(OurField, OurX, OurY, OurZ, UFOsDifference);
        }
        else if(OurCommand == 2) {
            ptrdiff_t FirstPoint1Dim, FirstPoint2Dim, FirstPoint3Dim, SecondPoint1Dim, SecondPoint2Dim, SecondPoint3Dim;
            std::cin >> FirstPoint1Dim >> FirstPoint2Dim >> FirstPoint3Dim >> SecondPoint1Dim >> SecondPoint2Dim >> SecondPoint3Dim;
            std::cout << UfoNumber(OurField, FirstPoint1Dim, FirstPoint2Dim, FirstPoint3Dim, SecondPoint1Dim, SecondPoint2Dim, SecondPoint3Dim) << "\n";
        }
        else
            break;
    }
    return 0;
}
