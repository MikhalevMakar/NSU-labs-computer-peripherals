//
// Created by Макар Михалёв on 19.11.2022.
//
#include "Matrix.h"

Matrix::Matrix() {
    matrix = new float[N * N];
}

Matrix::Matrix(const Matrix& other) {
    matrix = new float[N * N];
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[N * i + j] = other.matrix[N * i + j];
        }
    }
}

Matrix::Matrix(Matrix&& other) noexcept{
    this->matrix = other.matrix;
    delete[] &other;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    this->matrix = other.matrix;
    return *this;
}

void Matrix::fillI() {
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            matrix[N*i + j] = static_cast<float>(i == j);
        }
    }
}

void Matrix::fillA() {
    for(int i =0;i<N*N;++i){
       matrix[i] = i+1;
    }
}

float Matrix::findMaxRowAndColum(const Matrix& other) {
    float rowMaxSum = __FLT_MIN__, columMaxSum = __FLT_MIN__, curRowSum = 0, curColumSum = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            curRowSum += other.matrix[N * i + j];
            curColumSum += other.matrix[N * j + i];
        }

        rowMaxSum = (rowMaxSum < curRowSum) ? curRowSum : rowMaxSum;
        columMaxSum = (columMaxSum < curColumSum) ? curColumSum : columMaxSum;
        curRowSum = curColumSum = 0;
    }
    return rowMaxSum * columMaxSum;
}

void Matrix::fillZero() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[N*i+j] = 0.0;
        }
    }
}

void Matrix::fillB(const Matrix& other) {
    float denominator = findMaxRowAndColum(other);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[N * i + j] = other.matrix[N * j + i] / denominator;
        }
    }
}

void Matrix::fillR(const Matrix& I, const Matrix& B, const Matrix& A) {
    *this = A * B;
    *this = I - *this;
}

void Matrix::printMatrix() {
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            std::cout <<  matrix[N*i+j] << " ";
        }
        std::cout << "\n";
    }
}
float Matrix::checkMatrix() {
    float sum = 0;
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            sum += matrix[N*i+j];
        }
    }
    return sum;
}

Matrix& Matrix::operator=(const Matrix& first) {
    Matrix second;
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            second.matrix[N*i+j] = first.matrix[N*i+j];
        }
    }
    return second;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            matrix[N*i+j] += other.matrix[N*i+j];
        }
    }
    return *this;
}

Matrix& Matrix::Matrix::operator-=(const Matrix& other) {
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            matrix[N*i+j] -= other.matrix[N*i+j];
        }
    }
    return *this;
}


Matrix operator*(const Matrix& first, const Matrix& second) {
    Matrix result;
    result.fillZero();
    for (int row = 0;  row < N; ++row) {
        for (int  curIndex= 0;  curIndex < N; ++curIndex) {
            for (int colum = 0;  colum < N; ++colum) {
                result.matrix[N*row+colum] += first.matrix[N*row+curIndex] * second.matrix[N*curIndex+colum];
            }
        }
    }
    return result;
}

Matrix operator+(const Matrix& first, const Matrix& second) {
    Matrix result(first);
    result += second;
    return result;
}

Matrix operator-(const Matrix& first, const Matrix& second) {
    Matrix result(first);
    result -= second;
    return result;
}

void Matrix::findNormal() {
   float curRowSum = 0, curColumSum = 0;
   endlessNorm = firstNorm = __FLT_MIN__;
   for (int i = 0; i < N; ++i) {
       for (int j = 0; j < N; ++j) {
           curRowSum += std::abs(matrix[N * i + j]);
           curColumSum += std::abs(matrix[N * j + i]);
       }

       endlessNorm = (endlessNorm < curRowSum) ? curRowSum : endlessNorm;
       firstNorm = (firstNorm < curColumSum) ? curColumSum : firstNorm;
       curRowSum = curColumSum = 0;
   }
}

float Matrix::getFirstNorm() {
    return firstNorm;
}

float Matrix::getEndlessNorm() {
    return endlessNorm;
}
