//  main.cpp
//  ClassicalImplement
//
//  Created by Макар Михалёв on 19.11.2022.
//

#include "Matrix.h"

Matrix matrixInversion(const Matrix& A) {
    Matrix I, B, SuppA(A), R;
    I.fillI();
    B.fillB(A);
    std::cout<< "B\n";
    B.printMatrix();
    R.fillR(I, A, B);
    std::cout<< "R\n";
    R.printMatrix();
    Matrix CurrentR(R);
    Matrix InversionMatrix = I;

    InversionMatrix += R;

    for(int i = 2; i < M; ++i) {
        CurrentR =  R * CurrentR;
        InversionMatrix += CurrentR;
    }
    std::cout<< "InversionMatrix\n";
    InversionMatrix.printMatrix();
    InversionMatrix = InversionMatrix * B;
    return InversionMatrix;
}

int main() {
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime,
                                                                endTime;
    startTime = std::chrono::high_resolution_clock::now();
    Matrix A;
    A.fillA();

    Matrix inversionA = matrixInversion(A);
    inversionA =  inversionA*A;
    std::cout<< "inversionA\n";
    inversionA.printMatrix();

    std::cout << inversionA.getFirstNorm() << " " << inversionA.getEndlessNorm();
    endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalTime = (endTime - startTime);
    std::cout << "\nTotal Time: " << totalTime.count();
    return 0;
}