// #include "core.h"
#include "mathF.h"

vector<int> mathF::range(int limit) {
    vector<int> output;
    for (int i=0; i < limit; i++)
        output.push_back(i);
    return output;
}

vector<int> mathF::rangeI(int start, int end, int increment) {
    vector<int> output;
    for (int i=start; i < end; i += increment)
        output.push_back(i);
    return output;
}

float mathF::interpoler(float x1, float y1, float x2, float y2, float x) {
    if (x1 == x2) {return (y1 + y2)/2.0;}
    return (y1 + (y2 - y1)/(x2 - x1)*(x - x1));
};

float mathF::getMatrixDeterminant(matrix Matrix){
    //this function is written in c++ to calculate the determinant of matrix
    // it's a recursive function that can handle matrix of any dimension
    float det = 0.0; // the determinant value will be stored here
    if (Matrix.size() == 1)
    {
        return Matrix[0][0]; // no calculation needed
    }
    else if (Matrix.size() == 2)
    {
        //in this case we calculate the determinant of a 2-dimensional matrix in a 
        //default procedure
        det = (Matrix[0][0] * Matrix[1][1] - Matrix[0][1] * Matrix[1][0]);
        return det;
    }
    else
    {
        //in this case we calculate the determinant of a squared matrix that have 
        // for example 3x3 order greater than 2
        for (int p = 0; p < Matrix[0].size(); p++)
        {
            //this loop iterate on each elements of the first row in the matrix.
            //at each element we cancel the row and column it exist in
            //and form a matrix from the rest of the elements in the matrix
            matrix TempMatrix; // to hold the shaped matrix;
            for (float i = 1; i < Matrix.size(); i++)
            {
                // iteration will start from row one cancelling the first row values
                vector<float> TempRow;
                for (float j = 0; j < Matrix[i].size(); j++)
                {
                    // iteration will pass all cells of the i row excluding the j 
                    //value that match p column
                    if (j != p)
                    {
                       TempRow.push_back(Matrix[i][j]);//add current cell to TempRow 
                    }
                }
                if (TempRow.size() > 0)
                    TempMatrix.push_back(TempRow);
                //after adding each row of the new matrix to the vector tempx
                //we add it to the vector temp which is the vector where the new 
                //matrix will be formed
            }
            det = det + Matrix[0][p] * pow(-1, p) * getMatrixDeterminant(TempMatrix);
            //then we calculate the value of determinant by using a recursive way
            //where we re-call the function by passing to it the new formed matrix
            //we keep doing this until we get our determinant
        }
        return det;
    }
};

mathF::matrix mathF::getTransposeMatrix(mathF::matrix Matrix) {
    mathF::matrix transposedMatrix;
    for (int j=0; j < Matrix[0].size(); j++) {
        vector<float> line;
        for (int i=0; i < Matrix.size(); i++) {
            line.push_back(Matrix[i][j]);
        }
        transposedMatrix.push_back(line);
    }

    return transposedMatrix;
};

/*
Cofactor of a Matrix
The cofactor of a matrix A is matrix C that the value of element Cij equals the determinant of a matrix created by removing row i and column j from matrix A. Here is the method that calculates the cofactor matrix:
*/

mathF::matrix mathF::getSubMatrix(mathF::matrix Matrix, int ii, int jj) {
    matrix subMatrix;
    for (int i=0; i < Matrix.size(); i++) {
        if (i != ii) {
            vector<float> line;
            for (int j=0; j < Matrix[0].size(); j++) {
                if (j != jj) {
                    line.push_back(Matrix[i][j]);
                }
            }
            // for (int j=0; j < line.size(); j++) {
            //     cout << line[j] << " ";
            // }
            // cout << endl;
            subMatrix.push_back(line);
        }
    }
    return subMatrix;
}

mathF::matrix mathF::getCofactorMatrix(mathF::matrix Matrix) {
    // cout << "getSubMatrix: " << endl;
    // showMatrix(getSubMatrix(Matrix, 0, 0));
    // cout << pow(-1.0, 0) << endl;
    // cout << getMatrixDeterminant(getSubMatrix(Matrix, 0, 0)) << endl;

    matrix cofactorMatrix;
    for (int i=0; i < Matrix.size(); i++) {
        vector<float> line;
        for (int j=0; j < Matrix[0].size(); j++) {
            line.push_back(pow(-1.0, i+j) * getMatrixDeterminant(getSubMatrix(Matrix, i, j)));
        }
        cofactorMatrix.push_back(line);
    }
    return cofactorMatrix;
};

void mathF::matrixMultiplyConstant(matrix& Matrix, float constant) {
    for (int i=0; i < Matrix.size(); i++) {
        for (int j=0; j < Matrix[0].size(); j++) {
            Matrix[i][j] *= constant;
        }
    }
};

mathF::matrix mathF::getInverseMatrix(matrix Matrix, float determinant) {
    matrix inverseMatrix;
    // cout << "Matrix: " << endl;
    // showMatrix(Matrix);
    // cout << "getCofactorMatrix: " << endl;
    // showMatrix(getCofactorMatrix(Matrix));
    // cout << "getTransposeMatrix: " << endl;
    // showMatrix(getTransposeMatrix(getCofactorMatrix(Matrix)));
    inverseMatrix = getTransposeMatrix(getCofactorMatrix(Matrix));
    matrixMultiplyConstant(inverseMatrix, 1.0/determinant);
    return inverseMatrix;
};

void mathF::showMatrix(matrix Matrix){
    cout << "[ ";
    for (int i=0; i < Matrix.size(); i++) {
        for (int j=0; j < Matrix[0].size(); j++) {
            cout << Matrix[i][j] << ", ";
        }
        cout << endl;
    }
    cout << "]" << endl;
};

/*
Inverse of a Matrix
Inverse of a square matrix A is the matrix A-1 where AA-1=I. I is the identity matrix (see this link for more details).
*/

mathF::matrix mathF::multi2matrix(mathF::matrix v1, mathF::matrix v2) {
    mathF::matrix output;
    for (int i=0; i < v1.size(); i++) {
        vector<float> line;
        for (int j=0; j < v1[i].size(); j++) {
            float e = 0.0;
            for (int k=0; k < v1[i].size(); k++)
                e += v1[i][k] * v2[k][j];
            line.push_back(e);
        }
        output.push_back(line);
    }
    return output;
}