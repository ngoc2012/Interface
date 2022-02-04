#ifndef MATHF_H // include guard
#define MATHF_H

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

namespace mathF  // namespace declaration
{
    extern vector<int> range(int);
    extern vector<int> rangeI(int, int, int);

    extern float interpoler(float, float, float, float, float);
    
    // template<class T>
    // T core::roundNumber(T number, int demical) {
    //     number = number * (10 ^ demical);
    //     // number = number % i;
    //     number = number / (10 ^ demical);
    //     return number;
    // }

    typedef vector<vector<float>> matrix;

    extern float getMatrixDeterminant(matrix);
    extern matrix getTransposeMatrix(matrix);
    extern matrix getCofactorMatrix(matrix);
    extern matrix getSubMatrix(matrix, int, int);
    extern void matrixMultiplyConstant(matrix&, float);
    extern matrix getInverseMatrix(matrix, float);
    extern void showMatrix(matrix);

    template <typename T> int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }

    template <typename T> T sumprod2(vector<T> v1, vector<T> v2) {
        T output=0.0;
        for (int i=0; i < v1.size(); i++)
            output += v1[i]*v2[i];
        return output;
    }

    template <typename T> T sumprod3(vector<T> v1, vector<T> v2, vector<T> v3) {
        T output=0.0;
        for (int i=0; i < v1.size(); i++)
            output += v1[i]*v2[i]*v3[i];
        return output;
    }

    template <typename T> T sumprod4(vector<T> v1, vector<T> v2, vector<T> v3, vector<T> v4) {
        T output=0.0;
        for (int i=0; i < v1.size(); i++)
            output += v1[i]*v2[i]*v3[i]*v4[i];
        return output;
    }

    template <typename T> void sum2matrix(vector<vector<T>> v1, vector<vector<T>> v2, vector<vector<T>>& output) {
        for (int i=0; i < v1.size(); i++)
            for (int j=0; j < v1[i].size(); j++)
                output[i][j] = v1[i][j] + v2[i][j];
    }

    template <typename T> void sum3matrix(vector<vector<T>> v1, vector<vector<T>> v2, vector<vector<T>> v3, vector<vector<T>>& output) {
        for (int i=0; i < v1.size(); i++)
            for (int j=0; j < v1[i].size(); j++)
                output[i][j] = v1[i][j] + v2[i][j] + v3[i][j];
    }

    template <typename T> void sum2vecteurs(vector<T> v1, vector<T> v2, vector<T>& output) {
        for (int i=0; i < v1.size(); i++)
            output[i] = v1[i] + v2[i];
    }

    template <typename T> void sum3vecteurs(vector<T> v1, vector<T> v2, vector<T> v3, vector<T>& output) {
        for (int i=0; i < v1.size(); i++)
            output[i] = v1[i] + v2[i] + v3[i];
    }

    template <typename T> void dotProductMatrixColumn(vector<vector<T>> m, vector<T> v, vector<T>& output) {
        for (int i=0; i < m.size(); i++) {
            output[i] = 0.0;
            for (int j=0; j < v.size(); j++)
                output[i] += m[i][j]*v[j];
        }   
    }

    matrix multi2matrix(matrix, matrix);

    template <typename T> float length2DVector(T a) {
        return sqrt(a.x*a.x + a.y*a.y);
    }

    template <typename T> float distance2DVector(T a, T b) {
        return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
    }

    template <typename T> float dot2DVector(T a, T b) {
        return (a.x*b.x + a.y*b.y);
    }
        // return (a['x']*b['x'] + a['y']*b['y'])

    template <typename T> float angle2DVector(T a, T b) {
        return acos(dot2DVector(a, b) / length2DVector(a) / length2DVector(b));
    }
        // return math.acos(round(, 8))

}

#endif