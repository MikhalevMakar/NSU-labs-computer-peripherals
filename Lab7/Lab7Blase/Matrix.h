//
// Created by Макар Михалёв on 25.11.2022.
//

#ifndef LAB7BLASE_MATRIX_H
#define LAB7BLASE_MATRIX_H

#include <iostream>
#include <vector>
#include <mkl_cblas.h>

enum { N = 2048, M = 10};

class Matrix {
public:
    Matrix();

    Matrix(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;
    Matrix(Matrix&& other) noexcept;

    void fillI();

    void fillA();

    float findMaxRowAndColum(const Matrix& other);

    void fillZero();

    void fillB(const Matrix& other);

    void fillR(const Matrix& I, const Matrix& B, const Matrix& A);

    void printMatrix();

    Matrix& operator=(const Matrix& first);

    Matrix& operator+=(const Matrix& other);

    Matrix& operator-=(const Matrix& other);

    void findNormal();
    float getFirstNorm();
    float getEndlessNorm();
    friend  Matrix operator*(const Matrix& first, const Matrix& second);
    friend  Matrix operator-(const Matrix& first, const Matrix& second);
private:
    float* matrix;
    float endlessNorm, firstNorm;
};

Matrix operator*(const Matrix& first, const Matrix& second);

Matrix  operator+(const Matrix& first, const Matrix& second);

Matrix operator-(const Matrix& first, const Matrix& second);


#endif //LAB7BLASE_MATRIX_H
