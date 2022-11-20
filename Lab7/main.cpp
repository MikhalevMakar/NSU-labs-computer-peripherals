//  main.cpp
//  ClassicalImplement
//
//  Created by Макар Михалёв on 19.11.2022.
//

#include "Matrix.h"

Matrix matrixInversion(const Matrix& A) {
    Matrix I, B, SuppA, R;
    I.fillI();
    SuppA = A;
    B.fillB(A);
    R.fillR(I, B, A);

    std::cout<< "\nR\n";
    R.printMatrix();

    std::cout<< "\nB\n";
    B.printMatrix();

    std::cout<< "\nI\n";
    I.printMatrix();

    Matrix CurrentR(R);
    Matrix InversionMatrix = I;

    InversionMatrix += R;
    B.printMatrix();

    for(int i = 2; i < M; ++i) {
        CurrentR = CurrentR * R;
        InversionMatrix += CurrentR;
    }

    InversionMatrix = InversionMatrix * B;
    std::cout<<"InversionMatrix\n";
    InversionMatrix.printMatrix();
    return InversionMatrix;
}

int main() {
    Matrix A;
    A.fillA();

    std::cout << "matrix:\n";
    A.printMatrix();

    Matrix inversionA = matrixInversion(A);
    std::cout << "matrix inversion:\n";
    inversionA.printMatrix();
    A.printMatrix();

    Matrix E = A * inversionA;
    E.printMatrix();

    return 0;
}
