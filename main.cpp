#include "Image.h"
#include <iostream>

using namespace std;

typedef vector<double> Point;
typedef vector<vector<double>> Matrix;

/**
 * @param takes a 2d matrix
 * @returns inverted input matrix
 *
 * Precondition: inputMatrix is a 2x2 matrix with non-zero determinant
 * Postcondition: returns the inverse of the input matrix
 */
Matrix invertMatrix(Matrix inputMatrix)
{
    int a = inputMatrix[0][0];
    int b = inputMatrix[0][1];
    int c = inputMatrix[1][0];
    int d = inputMatrix[1][1];

    int determinant = a * d - b * c;

    Matrix outputMatrix;
    outputMatrix[0][0] = d / determinant;
    outputMatrix[0][1] = -b / determinant;
    outputMatrix[1][0] = -c / determinant;
    outputMatrix[1][1] = a / determinant;

    return outputMatrix;
}

/**
 * @param takes a 2x2 matrix and a 2D point
 * @returns the result of multiplying the matrix by the point
 *
 * Precondition: inputMatrix is a 2x2 matrix and point is a 2D point
 * Postcondition: returns the result of multiplying the matrix by the point
 */
Point multiplyMatrixAndPoint(Matrix inputMatrix, Point point)
{
    int a = inputMatrix[0][0];
    int b = inputMatrix[0][1];
    int c = inputMatrix[1][0];
    int d = inputMatrix[1][1];
    int x = point[0];
    int y = point[1];

    Point outputPoint;
    outputPoint[0] = a * x + b * y;
    outputPoint[1] = c * x + d * y;

    return outputPoint;
}

// Write a method to multiply two 2D matrices to get a new 2D matrix
/**
 * @param takes two 2x2 matrices
 * @returns the result of multiplying the two matrices
 *
 * Precondition: inputMatrix1 and inputMatrix2 are 2x2 matrices
 * Postcondition: returns the result of multiplying the two matrices
 */
Matrix multiplyMatrixAndMatrix(Matrix matrix1, Matrix matrix2)
{
    
}

int main(int argc, char *argv[])
{

    /**
     * Expecting:
     *     - Scale X (s_x)
     *     - Scale Y (s_y)
     *     - Translation X (t_x)
     *     - Translation Y (t_y)
     *     - Rotation angle in degrees (theta)
     *     - Shear factor (k)
     */
    if (argc != 6)
    {
        cerr << "Expected 6 inputs: Scale X, Scale Y, Translation X, Translation Y, Rotation angle, Shear factor" << endl;
        return 1;
    }

    double s_x = stod(argv[0]);
    double s_y = stod(argv[1]);
    double t_x = stod(argv[2]);
    double t_y = stod(argv[3]);
    double theta = stod(argv[4]);
    double k = stod(argv[5]);

    Image inputImage("test1.gif");
    Image outputImage(inputImage.getRows(), inputImage.getCols());

    return 0;
}