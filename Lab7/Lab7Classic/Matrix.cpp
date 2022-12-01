//
// Created by Макар Михалёв on 19.11.2022.
//
#include "Matrix.h"

Matrix::Matrix(const int sizeMatrix_) : sizeMatrix(sizeMatrix_) {
    matrix = new float[sizeMatrix * sizeMatrix];
}

Matrix::Matrix(const Matrix& other) {
    matrix = new float[other.sizeMatrix * other.sizeMatrix];
    sizeMatrix = other.sizeMatrix;
    for (int i = 0; i < sizeMatrix; ++i) {
        for (int j = 0; j < sizeMatrix; ++j) {
            matrix[sizeMatrix * i + j] = other.matrix[sizeMatrix * i + j];
        }
    }
}

Matrix::Matrix(Matrix&& other) noexcept {
    this->matrix = other.matrix;
    delete[] &other;
}

void Matrix::fillIdentityMatrix() {
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix; ++j) {
            matrix[sizeMatrix*i + j] = static_cast<float>(i == j);
        }
    }
}

void Matrix::fillZero() {
    for (int i = 0; i < sizeMatrix; ++i) {
        for (int j = 0; j < sizeMatrix; ++j) {
            matrix[sizeMatrix*i+j] = 0.0;
        }
    }
}

void Matrix::printMatrix() {
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix; ++j) {
            std::cout <<  matrix[sizeMatrix*i+j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Matrix::printMatrix() const {
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix; ++j) {
            std::cout <<  matrix[sizeMatrix*i+j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

Matrix& Matrix::operator=(const Matrix& first) {
    sizeMatrix = first.sizeMatrix;
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix; ++j) {
           matrix[sizeMatrix*i+j] = first.matrix[sizeMatrix*i+j];
        }
    }
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix; ++j) {
            matrix[sizeMatrix*i+j] += other.matrix[sizeMatrix*i+j];
        }
    }
    return *this;
}

Matrix& Matrix::Matrix::operator-=(const Matrix& other) {
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix; ++j) {
            this->matrix[sizeMatrix*i+j] -= other.matrix[sizeMatrix*i+j];
        }
    }

    return *this;
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
    for (int i = 0; i < sizeMatrix; ++i) {
        for (int j = 0; j < sizeMatrix; ++j) {
            curRowSum += std::abs(matrix[sizeMatrix * i + j]);
            curColumSum += std::abs(matrix[sizeMatrix * j + i]);
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

Matrix& Matrix::operator/=(float denominator) {
    for (int i = 0; i < sizeMatrix; ++i) {
        for (int j = 0; j < sizeMatrix; ++j) {
            matrix[i * sizeMatrix + j] /= denominator;
        }
    }
    return *this;
}
int Matrix::getSizeMatrix() {
    return sizeMatrix;
}

int Matrix::getSizeMatrix() const {
    return sizeMatrix;
}

Matrix operator*(const Matrix& first, const Matrix& second) {
    Matrix result(first.getSizeMatrix());
    result.fillZero();
    for (int row = 0;  row < result.getSizeMatrix(); ++row) {
        for (int  curIndex= 0;  curIndex < result.getSizeMatrix(); ++curIndex) {
            for (int colum = 0;  colum < result.getSizeMatrix(); ++colum) {
                result.matrix[result.getSizeMatrix()*row+colum] += first.matrix[result.getSizeMatrix()*row+curIndex] * second.matrix[result.getSizeMatrix()*curIndex+colum];
            }
        }
    }
    return result;
}

Matrix Matrix::transformMatrix(const Matrix& other) {
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix; ++j) {
            matrix[sizeMatrix*j+i] = other.matrix[sizeMatrix*i+j];
        }
    }
    return *this;
}

Matrix createMatrixBuiltFromNorms(const int sizeMatrix,  Matrix other) {
    Matrix transformMatrix(sizeMatrix);
    transformMatrix.transformMatrix(other);

    other.findNormal();

    transformMatrix /= (other.getFirstNorm() * other.getEndlessNorm());

    return transformMatrix;
}

Matrix createMatrixOfRange(const int sizeMatrixOfRange, const Matrix& I, const Matrix& A, const Matrix& B) {
    Matrix matrixOfRange(sizeMatrixOfRange);
    matrixOfRange = B * A;
    matrixOfRange = I - matrixOfRange;
    return matrixOfRange;
}

void Matrix::setMatrix() {
    for(int i = 0; i < sizeMatrix*sizeMatrix; ++i) {
        matrix[i] = static_cast<float>(i % 7);
    }
}

Matrix::~Matrix() {
    if(matrix != NULL) {
        delete[] matrix;
        matrix = NULL;
    }
}

Matrix matrixInversion(const int sizeMatrix, const int M) {
    Matrix A(sizeMatrix), I(sizeMatrix);
    A.setMatrix();
    I.fillIdentityMatrix();
    Matrix B = createMatrixBuiltFromNorms(sizeMatrix, A);
    Matrix Range = createMatrixOfRange(sizeMatrix, I, A, B);

    Matrix CurrentRange(Range);
    Matrix InversionMatrix(I);

    for(int i = 0; i < M; ++i) {
        InversionMatrix += CurrentRange;
        CurrentRange = CurrentRange * Range;
    }

    InversionMatrix = InversionMatrix * B;

    return InversionMatrix;
}

