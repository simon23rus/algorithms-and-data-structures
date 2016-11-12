//
//  main.cpp
//  B
//
//  Created by Simon Fedotov on 11.04.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>

void countArea(std::vector<std::vector<int>> &maze, int &result, int dotX, int dotY) {
    if(maze[dotX][dotY] == 1) {
        ++result;
        maze[dotX][dotY] = -1;
        countArea(maze, result, dotX, dotY + 1);
        countArea(maze, result, dotX, dotY - 1);
        countArea(maze, result, dotX + 1, dotY);
        countArea(maze, result, dotX - 1, dotY);
    }
    else
        return;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    int mazeSize;
    std::cin >> mazeSize;
    std::vector<std::vector<int>> maze(mazeSize);
    for(int i = 0; i < mazeSize; ++i) {
        for(int j = 0; j < mazeSize; ++j) {
            char c;
            std::cin >> c;
            if(c == '.')
                maze[i].push_back(1);
            else
                maze[i].push_back(0);
        }
    }
    int dotX, dotY;
    std::cin >> dotX >> dotY;
    --dotX; --dotY;
//    for(auto row : maze) {
//        for(auto elem : row)
//            std::cout << elem << " ";
//        std::cout << "\n";
//    }
    
    //0 = * ; 1 = .; -1 = visited
    int result = 0;
    countArea(maze, result, dotX, dotY);
    std::cout << result;
//    std::cout << "\nResult is  " << result;
    return 0;
}
