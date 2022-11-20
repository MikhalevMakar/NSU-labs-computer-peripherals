//
// Created by Макар Михалёв on 19.11.2022.
//

#ifndef LAB7_MATRIX_H
#define LAB7_MATRIX_H

#include <iostream>
#include <vector>
enum { N = 2, M = 10};

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



    friend  Matrix operator*(const Matrix& first, const Matrix& second);
    friend  Matrix operator-(const Matrix& first, const Matrix& second);
private:
    float* matrix;
};

Matrix operator*(const Matrix& first, const Matrix& second);

Matrix  operator+(const Matrix& first, const Matrix& second);

Matrix operator-(const Matrix& first, const Matrix& second);

#endif //LAB7_MATRIX_H

//
//#include <cfloat>       // FLT_MIN
//#include <cmath>        // fabs()
//#include <ctime>
//#include <iostream>
//
//#define N 2
//#define M 10
//
//void Inverse(const float * matrix,
//             float * result);
//float GetMaxSum(const float * matrix); // |A|_1 * |A|_infinity
//void FillB(const float * matrix, float * B);
//void FillI(float* I);
//void Multiplication(const float * multiplier1,
//                    const float * multiplier2,
//                    float * result);
//void Addition(const float * addend1,
//              const float * addend2,
//              float * result);
//void Subtraction(const float * minuend,
//                 const float * subtrahend,
//                 float * result);
//void Copy(float * dest,
//          const float * src);
// //float AbsSum(const float * matrix);
//
//void print(float* matrix) {
//    for(int i = 0; i < N; i++) {
//        for (int j = 0; j < N; j++) {
//            std::cout << matrix[N * i + j];
//        }
//    }
//}

//int main()
//{
//    srandom(time(nullptr));
//    auto * matrix = new float [N * N];
//    auto * result = new float [N * N];
//    auto * check = new float [N * N];
//    timespec start = {
//            0,
//            0
//    };
//    timespec end = {
//            0,
//            0
//    };
//
//    for (int i = 0; i < N * N; ++i)
//    {
//        matrix[i] = 1;
//    }
//
//    clock_gettime(CLOCK_MONOTONIC_RAW,
//                  &start);
//    Inverse(matrix,
//            result);
//    clock_gettime(CLOCK_MONOTONIC_RAW,
//                  &end);
//

//    std::cout << "Time without vectorization: "
//              << (double)end.tv_sec - (double)start.tv_sec + 1e-9 * ((double)end.tv_nsec - (double)start.tv_nsec)
//              << " sec." << std::endl;
//
//    delete []matrix;
//    delete []result;
//    delete []check;
//
//    return EXIT_SUCCESS;
//}
//
//void Inverse(const float * matrix,
//             float * result)
//{
//    auto * B = new float[N * N];
//    auto * I = new float[N * N];
//    auto * tmp = new float[N * N];
//    auto * R = new float[N * N];
//    bool flag = true;
//
//    FillB(matrix,
//          B);
//    FillI(I);
//    Multiplication(B,
//                   matrix,
//                   tmp);
//    Subtraction(I,
//                tmp,
//                R);
//    Addition(I,
//             R,
//             tmp);
//    Copy(result,
//         R);
//
//    print(B);
//
//    for (int i = 2; i < M; ++i)
//    {
//        Multiplication(flag ? result : I,
//                       R,
//                       flag ? I : result);
//        Addition(tmp,
//                 flag ? I : result,
//                 tmp);
//        flag = !flag;
//    }
//
//    Multiplication(tmp,
//                   B,
//                   result);
//
//    delete[] I;
//    delete[] B;
//    delete[] tmp;
//    delete[] R;
//}
//
//float GetMaxSum(const float * matrix)
//{
//    float max_sum_row = FLT_MIN;
//    float max_sum_column = FLT_MIN;
//    float sum_row = 0;
//    float sum_column = 0;
//
//    for (int i = 0; i < N; i++) // rows
//    {
//        sum_row = 0;
//        sum_column = 0;
//
//        for (int j = 0; j < N; j++) // columns
//        {
//            sum_row += std::fabs(matrix[N * i + j]);
//            sum_column += std::fabs(matrix[j * N + i]);
//        }
//
//        if (sum_row > max_sum_row)          max_sum_row = sum_row;
//        if (sum_column > max_sum_column)    max_sum_column = sum_column;
//    }
//
//    return max_sum_row * max_sum_column;
//}
//
//void FillB(const float * matrix,
//           float * B)
//{
//    float max = GetMaxSum(matrix);
//
//    for (int i = 0; i < N; ++i)
//        for (int j = 0; j < N; ++j)
//            B[N * i + j] = matrix[j * N + i] / max;
//}
//
//void FillI(float * I)
//{
//    for (int i = 0; i < N; ++i)
//        for (int j = 0; j < N; ++j)
//            I[N * i + j] = (float)(i == j);
//}
//
//void Multiplication(const float * multiplier1,
//                    const float * multiplier2,
//                    float * result)
//{
//    for (int i = 0; i < N * N; ++i)
//        result[i] = 0;
//
//    for (int i = 0; i < N; ++i)
//        for (int j = 0; j < N; ++j)
//            for (int k = 0; k < N; ++k)
//                result[N * i + k] += multiplier1[N * i + j] * multiplier2[N * j + k];
//}
//
//void Addition(const float * addend1,
//              const float * addend2,
//              float * result)
//{
//    for (int i = 0; i < N * N; ++i)
//        result[i] = addend1[i] + addend2[i];
//}
//
//void Subtraction(const float * minuend,
//                 const float * subtrahend,
//                 float * result)
//{
//    for (int i = 0; i < N * N; i++)
//        result[i] = minuend[i] - subtrahend[i];
//}
//
//void Copy(float * dest,
//          const float * src)
//{
//    for (int i = 0; i < N * N; i++)
//        dest[i] = src[i];
