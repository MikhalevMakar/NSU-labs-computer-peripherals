//  main.cpp
//  ClassicalImplement
//
//  Created by Макар Михалёв on 19.11.2022.
//

#include "Matrix.h"
#include <chrono>

void checkMatrix(Matrix InversionMatrix) {
    Matrix defaultMatrix(2048);
    defaultMatrix.setMatrix();
    InversionMatrix = InversionMatrix * defaultMatrix;
    InversionMatrix.findNormal();
    //InversionMatrix.printMatrix();
    std::cout << "\n" << InversionMatrix.getFirstNorm() << " " << InversionMatrix.getEndlessNorm();
}

int main() {
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime,
            endTime;
    startTime = std::chrono::high_resolution_clock::now();

    Matrix InversionA = matrixInversion(2048, 10);

    endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> totalTime = (endTime - startTime);
    std::cout << "\nTotal Time: " << totalTime.count() << "\n";

    checkMatrix(InversionA);
    return 0;
}
