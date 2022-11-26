//
// Created by Макар Михалёв on 24.11.2022.
//

#ifndef LAB7NEON_MATRIX_H
#define LAB7NEON_MATRIX_H



#include <iostream>
#include <vector>
#include <arm_neon.h>
enum { N = 2048, M = 10, zeroValue = 0};

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

    float* TransformMatrix(const Matrix& other);

    void printMatrix();

    void findNormal();

    float getFirstNorm();

    float getEndlessNorm();

    Matrix& operator=(const Matrix& first);

    Matrix& operator+=(const Matrix& other);
    void printMatrix() const;
    Matrix& operator-=(const Matrix& other);
    ~Matrix();
    float findMaxSumColumLow(float32x4_t rowMaxSum__neon, float32x4_t columMaxSum__neon);

    friend  Matrix operator*(const Matrix& first, const Matrix& second);
    friend  Matrix operator-(const Matrix& first, const Matrix& second);
private:
    float* matrix;
    float32x4_t* matrix_neon;
    //float* transformMatrix;
    float endlessNorm, firstNorm;
};

Matrix operator*(const Matrix& first, const Matrix& second);

Matrix  operator+(const Matrix& first, const Matrix& second);

Matrix operator-(const Matrix& first, const Matrix& second);


#endif //LAB7NEON_MATRIX_H
