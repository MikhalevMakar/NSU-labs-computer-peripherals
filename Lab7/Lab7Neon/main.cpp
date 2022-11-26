#include <iostream>
#include "Matrix.h"

Matrix matrixInversion(const Matrix& A) {
    Matrix I, B, SuppA(A), R;
    I.fillI();
    B.fillB(A);

    R.fillR(I, A, B);

    Matrix CurrentR(R);
    Matrix InversionMatrix = I;

    InversionMatrix += R;

    for(int i = 1; i < M; ++i) {
        CurrentR =  R * CurrentR;
        InversionMatrix += CurrentR;
    }
    InversionMatrix = InversionMatrix * B;
    return InversionMatrix;
}

int main() {
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime,
                                                                endTime;
    startTime = std::chrono::high_resolution_clock::now();
    Matrix A;
    A.fillA();
    Matrix InversionA = matrixInversion(A);

    endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> duration(endTime - startTime);
    std::cout<< "\nTotal Time: " << duration.count();

    InversionA = InversionA * A;
    InversionA.findNormal();
    std::cout << "\n" << InversionA.getFirstNorm() << " " << InversionA.getEndlessNorm();
    return 0;
}