//
//  main.cpp
//  Reversi
//
//  Created by Simon Fedotov on 01.12.15.
//  Copyright © 2015 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <chrono>
#include <cstdlib>
static short ROWS_NUMBER = 8;
static int EARLY_GAME = 20;
static int MID_GAME = 40;

std::vector<std::vector<int>> positionCost = {
    {120, -20,  20,   5,   5,  20, -20, 120},
    {-20, -40,  -5,  -5,  -5,  -5, -40, -20},
    {20 ,  -5,  15,   3,   3,  15,  -5,  20},
    {5  ,  -5,   3,   3,   3,   3,  -5,   5},
    {5  ,  -5,   3,   3,   3,   3,  -5,   5},
    {20 ,  -5,  15,   3,   3,  15,  -5,  20},
    {-20, -40,  -5,  -5,  -5,  -5, -40, -20},
    {120, -20,  20,   5,   5,  20, -20, 120}
};




enum KindOfTile {
    BLANK,
    WHITE,
    BLACK
};

enum PlayerColour {
    WHITE_COLOR,
    BLACK_COLOR
};

enum Strategy {
    HUMAN,
    RANDOM,
    
};

struct Position {
    int row;
    int column;
    
    Position(int row, int column) : row(row), column(column) {};
    bool operator <(const Position &other) const{
        if(row == other.row)
            return column < other.column;
        return row < other.row;
    }
};


class Player {
private:
    PlayerColour myColour;
    Strategy myStrategy;
public:
    PlayerColour GetColour() const;
    Player() = default;
    Player(const PlayerColour &playerColour, const Strategy &playerStrategy) : myColour(playerColour), myStrategy(playerStrategy) {}
};

PlayerColour Player::GetColour() const {
    return myColour;
}

class Game {
private:
    std::vector<std::vector<char>> isStable;
    int stepsNumber;
    int numberOfBlacks;
    Player first;
    Position currentPosition;
    std::set<Position> badCells;
    std::set<Position> goodCells;
    std::vector<Position> AvailableMoves(KindOfTile myColour);
    
    
public:
    std::vector<std::vector<KindOfTile>> table;
    int alphaBetaNegaMax(Game &currentGame, int alpha, int beta, int &level, KindOfTile &myColour, int bound = 8, bool canGo = true);
    int negaMax(Game &currentGame, int &level, KindOfTile &myColour, const int &bound, bool canGo);
    Position lookThrough(int bound);
    
    Game(const Player &first, int row = 0, int column = 0, Position thisPosition = {-1, -1}) : first(first), currentPosition(thisPosition), stepsNumber(4), numberOfBlacks(2) {
        srand(time(0));
        table.resize(ROWS_NUMBER);
        isStable.resize(ROWS_NUMBER);
        for(size_t i = 0; i < ROWS_NUMBER; ++i) {
            table[i].resize(ROWS_NUMBER);
            isStable[i].resize(ROWS_NUMBER, false);
        }
        isStable[0][0] = true;
        isStable[0][ROWS_NUMBER - 1] = true;
        isStable[ROWS_NUMBER - 1][0] = true;
        isStable[ROWS_NUMBER - 1][ROWS_NUMBER - 1] = true;
        table[3][3] = WHITE;
        table[4][4] = WHITE;
        table[3][4] = BLACK;
        table[4][3] = BLACK;
        if(row != 0 && column != 0) {
            ++stepsNumber;
            table[row][column] = BLACK;
            if(row == 5 || row == 4)
                table[4][4] = BLACK;
            else
                table[3][3] = BLACK;
        }
        
        badCells.insert({0, 1});
        badCells.insert({1, 0});
        //        badCells.insert({1, 1});
        badCells.insert({0, 6});
        //        badCells.insert({1, 6});
        badCells.insert({1, 7});
        badCells.insert({6, 0});
        //        badCells.insert({6, 1});
        badCells.insert({7, 1});
        //        badCells.insert({6, 6});
        badCells.insert({6, 7});
        badCells.insert({7, 6});
        
        goodCells.insert({0, 0});
        goodCells.insert({0, 7});
        goodCells.insert({7, 0});
        goodCells.insert({7, 7});
        
    }
    
    int getNumber(KindOfTile myColour) const;
    void SetTile(const Position &toSet, const KindOfTile &value);
    void Go();
    void PrintBoard();
    int checkStability();
    bool makeStep();
    std::vector<Position> FlipTheTiles(const Position &thisTile, KindOfTile myColour);
    int countHeuristic(KindOfTile myColour, int level);
    int numberOfMoves(KindOfTile myColour, std::vector<Position> &availablePositions);
};


int Game::checkStability() {
    int resultBlack = 0;
    int resultWhite = 0;
    return 0;
}

int Game::getNumber(KindOfTile myColour) const {
    int result = 0;
    for(int i = 0; i < ROWS_NUMBER; ++i) {
        for(int j = 0; j < ROWS_NUMBER; ++j)
            if(table[i][j] == myColour)
                ++result;
    }
    return result;
}

Position Game::lookThrough(int bound) {
    
    KindOfTile myColour = first.GetColour() == WHITE_COLOR ? WHITE : BLACK;
    KindOfTile opponentColour = myColour == WHITE ? BLACK : WHITE;
    std::vector<Position> availableMoves;
    numberOfMoves(myColour, availableMoves);
    for(auto &step : availableMoves) {
        if(goodCells.count(step))
            return step;
    }
    std::random_shuffle(availableMoves.begin(), availableMoves.end());
    if(availableMoves.size() < 5)
        bound = 5;
    else if(availableMoves.size() < 10)
        bound = 5;
    else
        bound = 5;
    //    std::cout << availableMoves.size() <<  " kolvohodov\n";
    Position optimalPosition = Position(-1, -1);
    int maxValue = INT16_MIN;
    Game clone = *this;
    int childrenCounter = 0;
    for(auto &curPos : availableMoves) {
        //        std::cout << "chil " << childrenCounter << "\n";
        clone.SetTile(curPos, myColour);
        auto flipped = clone.FlipTheTiles(curPos, myColour);
        int alpha = INT16_MIN; int beta = INT16_MAX; int level = 0;
        clone.currentPosition = curPos;
        int heuristic =  (-alphaBetaNegaMax(clone, (int)(-beta) , (int)(-alpha), level, opponentColour, bound));
        //        std::cout << heuristic << "heuheu\n";
        if(heuristic > maxValue) {
            optimalPosition = curPos;
            maxValue = heuristic;
        }
        for(int j = 0; j < flipped.size(); ++j)
            clone.SetTile(flipped[j], opponentColour);
        clone.SetTile(curPos, BLANK);
        ++childrenCounter;
    }
    //    std::cout << "|}|}|}|----eto klon|{|{|{|{\n";
    //    clone.PrintBoard();
    return optimalPosition;
}
//
//int Game::negaMax(Game &currentGame, int &level, KindOfTile &myColour, const int &bound, bool canGo) {
//
//}

int Game::alphaBetaNegaMax(Game &currentGame, int alpha, int beta, int &level, KindOfTile &myColour,  int bound, bool canGo) {
    //    std::cout << "||ALPHA|| " << alpha << " ||BETA|| " << beta << "\n";
    //    std::cout << level << "etoLevel\n";
    KindOfTile opponentColour = myColour == WHITE ? BLACK : WHITE;
    std::vector<Position> childrenStates;
    currentGame.numberOfMoves(myColour, childrenStates);
    if(childrenStates.size() < 5)
        bound = 6;
    else if(childrenStates.size() < 10)
        bound = 4;
    else
        bound = 3;
    int newLevel = level + 1;
    std::random_shuffle(childrenStates.begin(), childrenStates.end());    std::random_shuffle(childrenStates.begin(), childrenStates.end());
    std::random_shuffle(childrenStates.begin(), childrenStates.end());
    
    if(childrenStates.size() == 0 && canGo)
        return -currentGame.alphaBetaNegaMax(currentGame, (int)(-beta) , (int)(-alpha), newLevel, opponentColour, bound, false);
    else if(childrenStates.size() == 0 && !canGo && level == 1) {
        return  INT16_MIN;
    }
    else if(childrenStates.size() == 0 && !canGo) {
        return  currentGame.countHeuristic(myColour, level);
    }
    if(level >= bound) {
        return currentGame.countHeuristic(myColour, level);
    }
    int bestValue = INT16_MIN;
    //    std::cout << childrenStates.size() << "saez detey \n";
    for(int i = 0; i < childrenStates.size(); ++i) {
        currentGame.SetTile(childrenStates[i], myColour);
        auto flipped = currentGame.FlipTheTiles(childrenStates[i], myColour);
        currentGame.currentPosition = childrenStates[i];
        int currentValue = (-currentGame.alphaBetaNegaMax(currentGame, (int)(-beta) , (int)(-alpha), newLevel, opponentColour, bound));
        bestValue = std::max(bestValue, currentValue);
        alpha = std::max(alpha, currentValue);
        for(int j = 0; j < flipped.size(); ++j)
            currentGame.SetTile(flipped[j], opponentColour);
        currentGame.SetTile(childrenStates[i], BLANK);
        if(alpha >= beta)
            break;
    }
    return bestValue;
}

int Game::countHeuristic(KindOfTile myColour, int level) {
    //    PrintBoard();
    std::vector<Position> availablePositionsForMe;
    std::vector<Position> availablePositionsForOpponent;
    KindOfTile opponentColour = myColour == BLACK ? WHITE : BLACK;
    //    int val1 = countHeuristic(opponentColour, level);
    int movesNumber = numberOfMoves(myColour, availablePositionsForMe);
    int opponentNumber = numberOfMoves(opponentColour, availablePositionsForOpponent);
    int diffMoves = movesNumber - opponentNumber;
    int myNum = getNumber(myColour);
    
    int differenceOfTiles = 2 * myNum - (stepsNumber + level);
    int numberOfBadCellsDiff = 0;
    int diagonalNumber = 0;
    int numberOfGoodCellsDiff = 0;
    int borderCellsForMe = 0;
    int borderCellsForOpponent = 0;
    int borderDiff;
    for(auto &step : availablePositionsForMe) {
        if(badCells.count(step))
            ++numberOfBadCellsDiff;
        else if(goodCells.count(step)) {
            ++numberOfGoodCellsDiff;
            //            std::cout << "zuzu\n";
            //            PrintBoard();
        }
        else if(step.row == 1 && step.column == 1) {
            if(table[0][0] == BLANK)
                ++diagonalNumber;
            else
                --diagonalNumber;
        }
        else if(step.row == 1 && step.column == 6) {
            if(table[0][7] == BLANK)
                ++diagonalNumber;
            else
                --diagonalNumber;
        }
        else if(step.row == 6 && step.column == 1) {
            if(table[7][0] == BLANK)
                ++diagonalNumber;
            else
                --diagonalNumber;
        }
        else if(step.row == 6 && step.column == 6) {
            if(table[7][7] == BLANK)
                ++diagonalNumber;
            else
                --diagonalNumber;
        }
        //        if(step.row == 0 || step.row == ROWS_NUMBER - 1 || step.column == 0|| step.column == ROWS_NUMBER - 1)
        //            ++borderCellsForMe;
    }
    for(auto &step : availablePositionsForOpponent) {
        if(badCells.count(step))
            --numberOfBadCellsDiff;
        if(goodCells.count(step)) {
            --numberOfGoodCellsDiff;
            //            std::cout << "zuzu\n";
            //            PrintBoard();
        }
        else if(step.row == 1 && step.column == 1) {
            if(table[0][0] == BLANK)
                --diagonalNumber;
            else
                ++diagonalNumber;
        }
        else if(step.row == 1 && step.column == 6) {
            if(table[0][7] == BLANK)
                --diagonalNumber;
            else
                ++diagonalNumber;
        }
        else if(step.row == 6 && step.column == 1) {
            if(table[7][0] == BLANK)
                --diagonalNumber;
            else
                ++diagonalNumber;
        }
        else if(step.row == 6 && step.column == 6) {
            if(table[7][7] == BLANK)
                --diagonalNumber;
            else
                ++diagonalNumber;
        }
        //        if(step.row == 0 || step.row == ROWS_NUMBER - 1 || step.column == 0|| step.column == ROWS_NUMBER - 1)
        //             ++borderCellsForOpponent;
    }
    int badCellsCoeff = -20;
    borderDiff = borderCellsForMe - borderCellsForOpponent;
    //    int heuristic = movesNumber * (stepsNumber + level - 10) + numberOfTilesWithMyColour * 3 + numberOfBadCells * (-30) + numberOfGoodCells * 500;
    int diffMovesCoeff = 6;
    int diffTilesCoeff = 10;
    if(stepsNumber + level > EARLY_GAME)
        badCellsCoeff = -10;
    else {
        diffMovesCoeff *= -1;
        diffTilesCoeff *= -1;
    }
    
    //    if(level == 0) {
    //        std::cout << "stepsNumber" << stepsNumber << "\n";
    //        std::cout << getNumber(myColour) << "\n";
    //    std::cout << "diffMoves" << diffMoves << "\n";
    //    std::cout << "diffTiles" << differenceOfTiles << "\n";
    //    std::cout << "numberOfBadCells" << numberOfBadCellsDiff << "\n";
    //    std::cout << "numberOfGoodCells" <<numberOfGoodCellsDiff << "\n";
    //    std::cout << "diagNumber" << diagonalNumber << "\n";
    //    }
    //    if(numberOfGoodCellsDiff != 0) {
    //        std::cout << "numOfGoodCells" << numberOfGoodCellsDiff << "\n";
    //    }
    int heuristic = diffMoves * diffMovesCoeff +  differenceOfTiles * diffTilesCoeff + numberOfBadCellsDiff * badCellsCoeff + numberOfGoodCellsDiff * 160 + diagonalNumber * (-40);
    //    std::cout << "EVRISTIKA " << heuristic << " \n";
    //    PrintBoard();
    //    std::cout << heuristic << "hey" << val1 << "\n";
    //    if(myColour == BLACK)
    //        heuristic *= -1;
    return heuristic;
}

int Game::numberOfMoves(KindOfTile myColour, std::vector<Position> &availablePositions) {
    KindOfTile opponentColour = myColour == WHITE ? BLACK : WHITE;
    int result = 0;
    bool isPushed = false;
    for(int i = 0; i < ROWS_NUMBER; ++i) {
        for(int j = 0; j < ROWS_NUMBER; ++j) {
            for(int dx = -1; dx <= 1; ++dx) {
                if(isPushed) {
                    isPushed = false;
                    break;
                }
                for(int dy = -1; dy <= 1; ++dy) {
                    isPushed = false;
                    if(table[i][j] != BLANK ||( dx == 0 && dy == 0))
                        continue;
                    bool isSequence = false;
                    int thisRow = i + dx, thisColumn = j + dy;
                    while(thisRow >= 0 && thisRow < ROWS_NUMBER && thisColumn >= 0 && thisColumn < ROWS_NUMBER && table[thisRow][thisColumn] == opponentColour) {
                        isSequence = true;
                        thisRow += dx;
                        thisColumn += dy;
                    }
                    if(isSequence && thisRow >= 0 && thisRow < ROWS_NUMBER && thisColumn >= 0 && thisColumn < ROWS_NUMBER && table[thisRow][thisColumn] == myColour) {
                        availablePositions.push_back({i, j});
                        ++result;
                        isPushed = true;
                        break;
                    }
                }
            }
        }
    }
    return result;
}

std::vector<Position> Game::FlipTheTiles(const Position &thisTile, KindOfTile myColour) {
    KindOfTile opponentColour = myColour == WHITE ? BLACK : WHITE;
    bool isPushed = false;
    std::vector<Position> result;
    int i = thisTile.row; int j = thisTile.column;
    for(int dx = -1; dx <= 1; ++dx) {
        if(isPushed) {
            isPushed = false;
            break;
        }
        for(int dy = -1; dy <= 1; ++dy) {
            isPushed = false;
            if(( dx == 0 && dy == 0))
                continue;
            bool isSequence = false;
            int thisRow = i + dx, thisColumn = j + dy;
            while(thisRow >= 0 && thisRow < ROWS_NUMBER && thisColumn >= 0 && thisColumn < ROWS_NUMBER && table[thisRow][thisColumn] == opponentColour) {
                isSequence = true;
                thisRow += dx;
                thisColumn += dy;
            }
            if(isSequence && thisRow >= 0 && thisRow < ROWS_NUMBER && thisColumn >= 0 && thisColumn < ROWS_NUMBER && table[thisRow][thisColumn] == myColour) {
                while(thisRow != i || thisColumn != j) {
                    thisRow -= dx;
                    thisColumn -= dy;
                    table[thisRow][thisColumn] = myColour;
                    result.push_back({thisRow, thisColumn});
                }
            }
        }
    }
    
    
    return result;
}

void Game::PrintBoard() {
    std::cout << "  ";
    for(int i = 0; i < table.size(); ++i)
        std::cout << (char)('a' + i) << " ";
    std::cout << "\n";
    for (int i = 0; i < table.size(); ++i) {
        std::cout << i + 1 << " ";
        for(int j = 0; j < table.size(); ++j) {
            if(table[i][j] == BLANK)
                std::cout << "-" << " ";
            else if(table[i][j] == WHITE)
                std::cout << "W" << " ";
            else
                std::cout << "B" << " ";
        }
        std::cout << "\n";
    }
}

void Game::SetTile(const Position &toSet, const KindOfTile &value) {
    table[toSet.row][toSet.column] = value;
}

std::vector<Position> Game::AvailableMoves(KindOfTile myColour) {
    std::vector<Position> result;
    KindOfTile opponentColour;
    opponentColour = myColour == WHITE ? BLACK : WHITE;
    for(int i = 0; i < ROWS_NUMBER; ++i) {
        for(int j = 0; j < ROWS_NUMBER; ++j) {
            //implement UP, DOWN, LEFT, RIGHT
            if(i == 2 && j == 2) {
                
                
            }
            if(myColour == WHITE) {
                
                
                
            }
            if(table[i][j] == BLANK) {
                bool isSequence = false;
                bool isPushed = false;
                int thisRow = i, thisColumn = j;
                thisRow = i - 1;
                while (thisRow >= 0 && table[thisRow][j] == opponentColour) {
                    isSequence = true;
                    --thisRow;
                }
                if(thisRow >= 0 && table[thisRow][j] == myColour && isSequence) {
                    if(!isPushed) {
                        result.push_back({i, j});
                    }
                    isPushed = true;
                }
                isSequence = false;
                thisRow = i + 1;
                while (thisRow < ROWS_NUMBER && table[thisRow][j] == opponentColour) {
                    isSequence = true;
                    ++thisRow;
                }
                if(thisRow < ROWS_NUMBER && table[thisRow][j] == myColour && isSequence) {
                    if(!isPushed)
                        result.push_back({i, j});
                    isPushed = true;
                    
                }
                isSequence = false;
                thisRow = i;
                thisColumn = j - 1;
                while (thisColumn >= 0 && table[i][thisColumn] == opponentColour) {
                    isSequence = true;
                    --thisColumn;
                }
                if(thisColumn >= 0 && table[i][thisColumn] == myColour && isSequence) {
                    if(!isPushed)
                        result.push_back({i, j});
                    isPushed = true;
                    
                }
                isSequence = false;
                thisColumn = j + 1;
                while (thisColumn < ROWS_NUMBER && table[i][thisColumn] == opponentColour) {
                    isSequence = true;
                    ++thisColumn;
                }
                if(thisColumn < ROWS_NUMBER && table[i][thisColumn] == myColour && isSequence) {
                    if(!isPushed)
                        result.push_back({i, j});
                    isPushed = true;
                    
                }
                isSequence = false;
                thisRow = i + 1; thisColumn = j + 1;
                while (thisColumn < ROWS_NUMBER && thisRow < ROWS_NUMBER && table[thisRow][thisColumn] == opponentColour) {
                    isSequence = true;
                    ++thisColumn;
                    ++thisRow;
                }
                if(thisColumn < ROWS_NUMBER && thisRow < ROWS_NUMBER && table[thisRow][thisColumn] == myColour && isSequence) {
                    if(!isPushed)
                        result.push_back({i, j});
                    isPushed = true;
                    
                }
                isSequence = false;
                thisRow = i - 1; thisColumn = j - 1;
                while (thisColumn >= 0 && thisRow >= 0 && table[thisRow][thisColumn] == opponentColour) {
                    isSequence = true;
                    --thisColumn;
                    --thisRow;
                }
                if(thisColumn >= 0 && thisRow >= 0 && table[thisRow][thisColumn] == myColour && isSequence) {
                    if(!isPushed)
                        result.push_back({i, j});
                    isPushed = true;
                    
                }
                isSequence = false;
                thisRow = i - 1; thisColumn = j + 1;
                while (thisColumn < ROWS_NUMBER && thisRow >= 0 && table[thisRow][thisColumn] == opponentColour) {
                    isSequence = true;
                    ++thisColumn;
                    --thisRow;
                }
                if(thisColumn < ROWS_NUMBER && thisRow >= 0 && table[thisRow][thisColumn] == myColour && isSequence) {
                    if(!isPushed)
                        result.push_back({i, j});
                    isPushed = true;
                    
                }
                isSequence = false;
                thisRow = i + 1; thisColumn = j - 1;
                while (thisColumn >= 0 && thisRow < ROWS_NUMBER && table[thisRow][thisColumn] == opponentColour) {
                    isSequence = true;
                    --thisColumn;
                    ++thisRow;
                }
                if(thisColumn >= 0 && thisRow < ROWS_NUMBER && table[thisRow][thisColumn] == myColour && isSequence) {
                    if(!isPushed)
                        result.push_back({i, j});
                    isPushed = true;
                    
                }
                
                
            }
        }
    }
    
    
    return result;
}



void Game::Go() {
    bool endOfGame = false;
    KindOfTile opponentColour;
    KindOfTile myColour = first.GetColour() == WHITE_COLOR ? WHITE : BLACK;
    opponentColour = first.GetColour() == WHITE_COLOR ? BLACK : WHITE;
    while(!endOfGame) {
        std::string command;
        std::cin >> command;
        if(command == "bad")
            return;
        else if(command == "lose" || command == "win" || command == "draw")
            return;
        else if(command == "turn") {
            std::vector<Position> moves;
            int movesAvailable = numberOfMoves(myColour, moves);
            int depth = 4;
            Position bestStep = Position(-1, -1);
            if(movesAvailable == 0) {
                //                std::cout << "ayay, ne mogu \n";
                
            }
            else if(movesAvailable == 1) {
                bestStep = moves[0];
                std::cout << "move " << (char)('a' + bestStep.column) << " " << bestStep.row + 1 << "\n";
                SetTile(bestStep, myColour);
                FlipTheTiles(bestStep, myColour);
//                PrintBoard();
            }
            else {
                ++stepsNumber;
                bestStep = lookThrough(8);
                ++numberOfBlacks;
                if(bestStep.row == -1 || bestStep.column == -1) {
                    int n = rand() % (moves.size());
                    bestStep = moves[n];
                }
                std::cout << "move " << (char)('a' + bestStep.column) << " " << bestStep.row + 1 << "\n";
                
                SetTile(bestStep, myColour);
                FlipTheTiles(bestStep, myColour);
                //                PrintBoard();
//                PrintBoard();

            }
        }
        else if(command == "move") {
            char letter;
            int row, column;
            std::cin >> letter >> row;
            column = letter - 'a' + 1;
            --row; --column;
            if(row < 0 || column < 0) {
                //                std::cout << "haha, sopernik ne mozhet\n";
                continue;
            }
            Position toSet = Position(row, column);
            SetTile(toSet, opponentColour);
            FlipTheTiles(toSet, opponentColour);
            ++stepsNumber;
            //            PrintBoard();
        }
        //        std::cout << stepsNumber << "thisSteps\n";
    }
}


int main(int argc, const char * argv[]) {
    // insert code here...
    std::string init;
    std::string myColour;
    std::cin >> init >> myColour;
    int row, column;
    int color;
    Player first, second;
    second = Player(BLACK_COLOR, HUMAN);
    
    if(myColour == "black") {
        first = Player(BLACK_COLOR, HUMAN);
        Game igra = Game(first);
        std::vector<Position> available;
        igra.numberOfMoves(BLACK, available);
        //        for(auto &elem : available)
        //            std::cout << elem.column + 1 << " " <<(char)('a' + elem.row) << "\n";
        //        igra.PrintBoard();
        //
        igra.Go();
    }
    else {
        char letter;
        std::string move;
        std::cin >> move >> letter >> row;
        column = letter - 'a';
        --row;
        first = Player(WHITE_COLOR, HUMAN);
        Game igra = Game(first, row, column);
        igra.Go();
    }
    
    return 0;
}
