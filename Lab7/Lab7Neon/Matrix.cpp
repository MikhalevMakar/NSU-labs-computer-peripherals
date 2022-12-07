#include "Matrix.h"

Matrix::Matrix(const int sizeMatrix_) : sizeMatrix(sizeMatrix_) {
    matrix = new float[sizeMatrix * sizeMatrix];
}

Matrix::Matrix(const Matrix& other) {
    matrix = new float[other.sizeMatrix * other.sizeMatrix];
    matrix_neon = (float32x4_t*)matrix;
    this->sizeMatrix = other.sizeMatrix;
    float32x4_t* other_neon = (float32x4_t*)other.matrix;
    for (int i = 0; i < sizeMatrix; ++i) {
        for (int j = 0; j < sizeMatrix/4; ++j) {
            matrix_neon[(sizeMatrix/4)*i+j] = other_neon[(sizeMatrix/4)*i+j];
        }
    }
}

Matrix::Matrix(Matrix&& other) noexcept{
    this->matrix = other.matrix;
    delete[] &other;
    other.matrix = NULL;
}

void Matrix::fillIdentityMatrix() {
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix; ++j) {
            matrix[sizeMatrix*i + j] = static_cast<float>(i == j);
        }
    }
}

float Matrix::findMaxSumColumLow(float32x4_t rowMaxSum__neon, float32x4_t columMaxSum__neon) {
    float* resultMax = new float[4];
    float32x2_t rowLow = vget_low_f32(rowMaxSum__neon);
    float32x2_t rowHigh = vget_high_f32(rowMaxSum__neon);

    float32x2_t columLow = vget_low_f32(columMaxSum__neon);
    float32x2_t ColumHigh = vget_high_f32(columMaxSum__neon);
    rowLow = vadd_f32(rowLow, rowHigh);
    columLow = vadd_f32(columLow, ColumHigh);
    float32x4_t  result = vcombine_f32(rowLow, columLow);
    vst1q_f32(resultMax, result);

    return (resultMax[0] + resultMax[1]) * (resultMax[2] + resultMax[3]);
}

float Matrix::findMaxRowAndColum(const Matrix& other) {
    float minValue = __FLT_MIN__;
    Matrix transformMatrix(other.sizeMatrix);
    transformMatrix.transformMatrix(other);

    float32x4_t rowMaxSum__neon = vdupq_n_f32(minValue),
                columMaxSum__neon = vdupq_n_f32(minValue),
                curRowSum__neon = vdupq_n_f32(zeroValue),
                curColumSum__neon = vdupq_n_f32(zeroValue);

    float32x4_t* other_neon = (float32x4_t*)other.matrix;
    float32x4_t* transformMatrix_neon = (float32x4_t*)transformMatrix.matrix;

    for (int i = 0; i < sizeMatrix; ++i) {
        for (int j = 0; j < sizeMatrix/4; ++j) {
            curRowSum__neon = vaddq_f32(curRowSum__neon, other_neon[(sizeMatrix/4)*i+j]);
            curColumSum__neon = vaddq_f32(curColumSum__neon, transformMatrix_neon[(sizeMatrix/4)*i+j]);
        }

        rowMaxSum__neon = (rowMaxSum__neon < curRowSum__neon) ? curRowSum__neon : rowMaxSum__neon;
        columMaxSum__neon = (columMaxSum__neon < curColumSum__neon) ? curColumSum__neon : columMaxSum__neon;
        curRowSum__neon = curColumSum__neon = vdupq_n_f32(zeroValue);
    }

    return findMaxSumColumLow(rowMaxSum__neon, columMaxSum__neon);
}

void Matrix::fillZero() {
    float32x4_t zeroValue_neon = vdupq_n_f32(zeroValue);
    matrix_neon = (float32x4_t*)matrix;
    for (int i = 0; i < sizeMatrix; ++i) {
        for (int j = 0; j < sizeMatrix/4; ++j) {
            matrix_neon[(sizeMatrix/4)*i+j] = zeroValue_neon;
        }
    }
}

Matrix createMatrixOfRange(const int sizeMatrixOfRange, const Matrix& I, const Matrix& A, const Matrix& B) {
    Matrix matrixOfRange(sizeMatrixOfRange);
    matrixOfRange = B*A;
    matrixOfRange = I - matrixOfRange;
    return matrixOfRange;
}

void Matrix::printMatrix() {
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix; ++j) {
            std::cout << matrix[sizeMatrix*i+j] << " ";
        }
        std::cout << "\n";
    }
}

void Matrix::printMatrix() const {
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix; ++j) {
            std::cout << matrix[sizeMatrix*i+j] << " ";
        }
        std::cout << "\n";
    }
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

Matrix& Matrix::operator=(const Matrix& first) {
   float32x4_t* first_neon = (float32x4_t*)first.matrix;
   sizeMatrix = first.sizeMatrix;
   matrix_neon = (float32x4_t*)matrix;
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix/4; ++j) {
            matrix_neon[(sizeMatrix/4)*i+j] = first_neon[(sizeMatrix/4)*i+j];
        }
    }
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    float32x4_t* other_neon = (float32x4_t*)other.matrix;
    matrix_neon = (float32x4_t*)matrix;
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix/4; ++j) {
            matrix_neon[(sizeMatrix/4)*i+j] = vaddq_f32(matrix_neon[(sizeMatrix/4)*i+j], other_neon[(sizeMatrix/4)*i+j]);
        }
    }
    return *this;
}

Matrix& Matrix::Matrix::operator-=(const Matrix& other) {
    const float32x4_t* other_neon = (const float32x4_t*)other.matrix;
    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix/4; ++j) {
            matrix_neon[(sizeMatrix/4)*i+j] = vsubq_f32(matrix_neon[(sizeMatrix/4)*i+j], other_neon[(sizeMatrix/4)*i+j]);
        }
    }
    return *this;
}

Matrix operator*(const Matrix& first, const Matrix& second) {
    Matrix result(first.sizeMatrix);
    result.fillZero();
    float32x4_t* result_neon = (float32x4_t*)result.matrix;
    float32x4_t* second_neon = (float32x4_t*)second.matrix;
    float32x4_t currentResult, currentMul;

    for (int row = 0; row < first.sizeMatrix; ++row) {
        for(int curIndex= 0; curIndex < first.sizeMatrix; ++curIndex) {
            currentResult = vdupq_n_f32(first.matrix[first.sizeMatrix*row + curIndex]);
            for (int colum = 0;  colum < first.sizeMatrix/4; ++colum) {
                currentMul = vmulxq_f32(currentResult, second_neon[first.sizeMatrix*curIndex/4+colum]);
                result_neon[first.sizeMatrix*row/4+colum] += currentMul;
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

Matrix& Matrix::operator/=(float denominator) {
    float32x4_t denominator_neon = vdupq_n_f32(denominator);
    matrix_neon = (float32x4_t*)matrix;
    for (int i = 0; i < sizeMatrix; ++i) {
        for (int j = 0; j < sizeMatrix/4; ++j) {
            matrix_neon[(sizeMatrix/4)*i+j] /=  denominator_neon;
        }
    }
    return *this;
}

void Matrix::transformMatrix(const Matrix& other) {
    float* transformMatrix = new float[sizeMatrix*sizeMatrix];

    for(int i = 0; i < sizeMatrix; ++i) {
        for(int j = 0; j < sizeMatrix; ++j) {
            transformMatrix[sizeMatrix*i+j] = other.matrix[sizeMatrix*j+i];
        }
    }
    this->matrix = transformMatrix;
}

Matrix createMatrixBuiltFromNorms(const int sizeMatrix,  Matrix other) {
    Matrix transformMatrix(sizeMatrix);
    transformMatrix.transformMatrix(other);
    other.findNormal();
    float denominator = other.getFirstNorm() * other.getEndlessNorm();
    transformMatrix /= denominator;
    return transformMatrix;
}

Matrix::~Matrix() {
    if(matrix != NULL) {
        delete[] matrix;
        matrix = NULL;
    }
}

void Matrix::setMatrix() {
    for(int i = 0; i < sizeMatrix*sizeMatrix; ++i) {
        matrix[i] = static_cast<float>(i % 7);
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
