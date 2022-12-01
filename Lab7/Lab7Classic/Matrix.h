//
// Created by Макар Михалёв on 19.11.2022.
//

#ifndef LAB7_MATRIX_H
#define LAB7_MATRIX_H

#include <iostream>
#include <vector>

class Matrix {
public:
    Matrix() = default;
    Matrix(const int sizeMatrix_);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    void fillIdentityMatrix();
    void fillZero();
    void printMatrix();
    void printMatrix() const;
    void findNormal();
    float getFirstNorm();
    float getEndlessNorm();
    void setMatrix();
    int getSizeMatrix();
    int getSizeMatrix() const;
    Matrix transformMatrix(const Matrix& other);

    Matrix& operator=(const Matrix& first);
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator/=(float denominator);

    friend  Matrix operator-(const Matrix& first, const Matrix& second);
    friend Matrix operator*(const Matrix& first, const Matrix& second);
    ~Matrix();
private:
    int sizeMatrix;
    float* matrix;
    float endlessNorm, firstNorm;
};

Matrix operator*(const Matrix& first, const Matrix& second);
Matrix  operator+(const Matrix& first, const Matrix& second);
Matrix operator-(const Matrix& first, const Matrix& second);

Matrix matrixInversion(const int N, const int M);
Matrix createMatrixOfRange(const int sizeMatrixOfRange, const Matrix& I, const Matrix& A, const Matrix& B);
Matrix createMatrixBuiltFromNorms(const int sizeMatrix, Matrix other);
#endif //LAB7_MATRIX_H

