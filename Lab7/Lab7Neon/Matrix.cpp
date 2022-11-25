#include "Matrix.h"

Matrix::Matrix() {
    matrix = new float[N * N];
}

Matrix::Matrix(const Matrix& other) {
    matrix = new float[N * N];
    matrix_neon = (float32x4_t*)matrix;
    float32x4_t* other_neon = (float32x4_t*)other.matrix;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N/4; ++j) {
            matrix_neon[N*i/4+j] = other_neon[N*i/4+j];
        }
    }
}

Matrix::Matrix(Matrix&& other) noexcept{
    this->matrix = other.matrix;
    delete[] &other;
    other.matrix = NULL;
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

float* Matrix::TransformMatrix(const Matrix& other) {
    float* transformMatrix = new float[N*N];

    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            transformMatrix[N*i+j] = other.matrix[N*j+i];
        }
    }
    return transformMatrix;
}

float Matrix::findMaxRowAndColum(const Matrix& other) {
    float minValue = __FLT_MIN__;
    float* transformMatrix = TransformMatrix(other);

    float32x4_t rowMaxSum__neon = vdupq_n_f32(minValue),
                columMaxSum__neon = vdupq_n_f32(minValue),
                curRowSum__neon = vdupq_n_f32(zeroValue),
                curColumSum__neon = vdupq_n_f32(zeroValue);

    float32x4_t* other_neon = (float32x4_t*)other.matrix;
    float32x4_t* transformMatrix_neon = (float32x4_t*)transformMatrix;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N/4; ++j) {
            curRowSum__neon = vaddq_f32(curRowSum__neon, other_neon[N*i/4+j]);
            curColumSum__neon = vaddq_f32(curColumSum__neon, transformMatrix_neon[N*i/4+j]);
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
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N/4; ++j) {
            matrix_neon[N*i/4+j] = zeroValue_neon;
        }
    }
}

void Matrix::fillB(const Matrix& other) {
    float denominator = findMaxRowAndColum(other);
    float* transformMatrix = TransformMatrix(other);
    float32x4_t denominator_neon = vdupq_n_f32(denominator);
    float32x4_t* other_neon = (float32x4_t*)transformMatrix;
    matrix_neon = (float32x4_t*)matrix;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N/4; ++j) {
            matrix_neon[N*i/4+j] = other_neon[N*i/4+j] / denominator_neon;
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
            std::cout << matrix[N*i+j] << " ";
        }
        std::cout << "\n";
    }
}

void Matrix::printMatrix() const{
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            std::cout << matrix[N*i+j] << " ";
        }
        std::cout << "\n";
    }
}

Matrix& Matrix::operator=(const Matrix& first) {
   float32x4_t* first_neon = (float32x4_t*)first.matrix;
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N/4; ++j) {
            matrix_neon[N*i/4+j] = first_neon[N*i/4+j];
        }
    }
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    float32x4_t* other_neon = (float32x4_t*)other.matrix;
    matrix_neon = (float32x4_t*)matrix;
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N/4; ++j) {
            matrix_neon[N*i/4+j] = vaddq_f32(matrix_neon[N*i/4+j], other_neon[N*i/4+j]);
        }
    }
    return *this;
}

Matrix& Matrix::Matrix::operator-=(const Matrix& other) {
    const float32x4_t* other_neon = (const float32x4_t*)other.matrix;
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N/4; ++j) {
            matrix_neon[N*i/4+j] = vsubq_f32(matrix_neon[N*i/4+j], other_neon[N*i/4+j]);
        }
    }
    return *this;
}

Matrix operator*(const Matrix& first, const Matrix& second) {
    Matrix result;
    result.fillZero();
    float32x4_t* result_neon = (float32x4_t*)result.matrix;
    float32x4_t* second_neon = (float32x4_t*)second.matrix;
    float32x4_t currentResult, currentMul;

    for (int row = 0;  row < N; ++row) {
        for(int  curIndex= 0;  curIndex < N; ++curIndex) {
            currentResult = vdupq_n_f32(first.matrix[N*row + curIndex]);
            for (int colum = 0;  colum < N/4; ++colum) {
                currentMul = vmulxq_f32(currentResult, second_neon[N*curIndex/4+colum]);
                result_neon[N*row/4+colum] += currentMul;
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

Matrix::~Matrix() {
//    delete[] matrix;
//    matrix = NULL;
}
