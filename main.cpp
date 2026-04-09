#include "Image.h"
#include <iostream>
#include <vector>

using namespace std;

typedef vector<double> Point;
typedef vector<vector<double>> Matrix;

Matrix invertMatrix(Matrix inputMatrix);
Point multiplyMatrixAndPoint(Matrix inputMatrix, Point point);
Matrix multiplyMatrixAndMatrix(Matrix matrix1, Matrix matrix2);
Point subtractPoints(Point point1, Point point2);
Point addPoints(Point point1, Point point2);

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
    if (argc != 7)
    {
        cerr << "Expected 6 inputs: Scale X, Scale Y, Translation X, Translation Y, Rotation angle, Shear factor" << endl;
        return 1;
    }

    // Load the input image and create an output image of the same size
    Image inputImage("test1.gif");
    Image outputImage(inputImage.getRows(), inputImage.getCols());

    // Calculate the center of the image
    double centerX = inputImage.getCols() / 2.0;
    double centerY = inputImage.getRows() / 2.0;

    // Create the transformation matrices based on the input parameters
    double s_x = stod(argv[1]);
    double s_y = stod(argv[2]);
    double t_x = stod(argv[3]);
    double t_y = stod(argv[4]);
    double theta_degrees = stod(argv[5]);
    double k = stod(argv[6]);

    double pi = std::acos(-1.0);
    double radians = theta_degrees * (pi / 180.0);

    // Define the transformation matrices
    Matrix scaleMatrix = {{s_x, 0}, {0, s_y}};
    Point translationMatrix = {t_x, t_y};
    Matrix rotation = {{cos(radians), -sin(radians)}, {sin(radians), cos(radians)}};
    Matrix shearMatrix = {{1, k}, {0, 1}};
    Point center = {centerX, centerY};

    // Create the combined inverse transformation matrix
    Matrix invertedScaleMatrix = invertMatrix(scaleMatrix);
    Matrix invertedRotation = invertMatrix(rotation);
    Matrix invertedShearMatrix = invertMatrix(shearMatrix);

    // R^1 * K^-1 * S^-1
    Matrix k_inv_s_inv = multiplyMatrixAndMatrix(invertedShearMatrix, invertedScaleMatrix);
    Matrix transformationMatrix = multiplyMatrixAndMatrix(invertedRotation, k_inv_s_inv);

    // Fill all pixels in the blank image
    for (int i = 0; i < outputImage.getRows(); i++)
    {
        for (int j = 0; j < outputImage.getCols(); j++)
        {
            Point currentPixel = {(double)j, (double)i};

            // (q - t - c)
            Point shiftedByTranslation = subtractPoints(currentPixel, translationMatrix);
            Point shiftedToOrigin = subtractPoints(shiftedByTranslation, center);

            // M^-1 * (q - t - c)
            Point transformedPoint = multiplyMatrixAndPoint(transformationMatrix, shiftedToOrigin);

            // M^-1 * (q - t - c) + c
            transformedPoint = addPoints(transformedPoint, center);

            double sourcePixelX = transformedPoint[0];
            double sourcePixelY = transformedPoint[1];

            // Check for out of bounds of input image
            if (sourcePixelX < 0 || sourcePixelX >= inputImage.getCols() || sourcePixelY < 0 || sourcePixelY >= inputImage.getRows())
            {
                outputImage.setPixel(i, j, 0, 0, 0);
            }
            else
            {
                // Perform bilinear interpolation
                double c = floor(sourcePixelX);
                double r = floor(sourcePixelY);

                Point top_left = {c, r};
                Point top_right = {c + 1, r};
                Point bottom_left = {c, r + 1};
                Point bottom_right = {c + 1, r + 1};

                double alpha = sourcePixelX - c;
                double beta = sourcePixelY - r;

                pixel topLeft = inputImage.getPixel(r, c);
                pixel topRight = inputImage.getPixel(r, c + 1);
                pixel bottomLeft = inputImage.getPixel(r + 1, c);   
                pixel bottomRight = inputImage.getPixel(r + 1, c + 1);

                double finalRed = (1 - alpha) * (1 - beta) * topLeft.red 
                                + alpha * (1 - beta) * topRight.red 
                                + (1 - alpha) * beta * bottomLeft.red 
                                + alpha * beta * bottomRight.red;

                double finalGreen = (1 - alpha) * (1 - beta) * topLeft.green 
                                + alpha * (1 - beta) * topRight.green 
                                + (1 - alpha) * beta * bottomLeft.green 
                                + alpha * beta * bottomRight.green;

                double finalBlue = (1 - alpha) * (1 - beta) * topLeft.blue 
                                + alpha * (1 - beta) * topRight.blue 
                                + (1 - alpha) * beta * bottomLeft.blue 
                                + alpha * beta * bottomRight.blue;

                byte finalR = static_cast<byte>(finalRed);
                byte finalG = static_cast<byte>(finalGreen);
                byte finalB = static_cast<byte>(finalBlue);

                pixel blendedPixel = {finalR, finalG, finalB}; 
                outputImage.setPixel(i, j, blendedPixel);
            }
        }
    }

    return 0;
}

/**
 * @param inputMatrix a 2x2 matrix
 * @returns inverted input matrix
 *
 * Precondition: inputMatrix is a 2x2 matrix with non-zero determinant
 * Postcondition: returns the inverse of the input matrix
 */
Matrix invertMatrix(Matrix inputMatrix)
{
    double a = inputMatrix[0][0];
    double b = inputMatrix[0][1];
    double c = inputMatrix[1][0];
    double d = inputMatrix[1][1];

    double determinant = a * d - b * c;

    Matrix outputMatrix(2, vector<double>(2));
    outputMatrix[0][0] = d / determinant;
    outputMatrix[0][1] = -b / determinant;
    outputMatrix[1][0] = -c / determinant;
    outputMatrix[1][1] = a / determinant;

    return outputMatrix;
}

/**
 * @param inputMatrix a 2x2 matrix
 * @param point a 2D point represented as a vector of doubles
 * @returns the result of multiplying the matrix by the point
 *
 * Precondition: inputMatrix is a 2x2 matrix and point is a 2D point
 * Postcondition: returns the result of multiplying the matrix by the point
 */
Point multiplyMatrixAndPoint(Matrix inputMatrix, Point point)
{
    double a = inputMatrix[0][0];
    double b = inputMatrix[0][1];
    double c = inputMatrix[1][0];
    double d = inputMatrix[1][1];
    double x = point[0];
    double y = point[1];

    Point outputPoint(2);
    outputPoint[0] = a * x + b * y;
    outputPoint[1] = c * x + d * y;

    return outputPoint;
}

// Write a method to multiply two 2D matrices to get a new 2D matrix
/**
 * @param matrix1, matrix2 two 2x2 matrices
 * @returns the result of multiplying the two matrices
 *
 * Precondition: inputMatrix1 and inputMatrix2 are 2x2 matrices
 * Postcondition: returns the result of multiplying the two matrices
 */
Matrix multiplyMatrixAndMatrix(Matrix matrix1, Matrix matrix2)
{
    double a = matrix1[0][0];
    double b = matrix1[0][1];
    double c = matrix1[1][0];
    double d = matrix1[1][1];

    double e = matrix2[0][0];
    double f = matrix2[0][1];
    double g = matrix2[1][0];
    double h = matrix2[1][1];

    Matrix outputMatrix(2, vector<double>(2));
    outputMatrix[0][0] = a * e + b * g;
    outputMatrix[0][1] = a * f + b * h;
    outputMatrix[1][0] = c * e + d * g;
    outputMatrix[1][1] = c * f + d * h;

    return outputMatrix;
}

/**
 * @param point1 The first point as a vector of doubles.
 * @param point2 The second point as a vector of doubles.
 * @return A new point representing subtracting point2 from point1.
 */
Point subtractPoints(Point point1, Point point2)
{
    return {point1[0] - point2[0], point1[1] - point2[1]};
}

/**
 * @param point1, point2 two 2D points represented as vectors of doubles
 * @returns the result of adding the two points
 */
Point addPoints(Point point1, Point point2)
{
    return {point1[0] + point2[0], point1[1] + point2[1]};
}