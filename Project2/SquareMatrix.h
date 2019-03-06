/*
 * File: SquareMatrix.h
 * Author: Paweł Wieczorek
 * Date: 25 Oct 2018
 * Description: Header of file SquareMatrix.c
 */

#ifndef PROJEKT2_SQUAREMATRIX_H
#define PROJEKT2_SQUAREMATRIX_H

/************************************
 * Macros definitions
 ************************************/
#define MAX_MATRIX_FIELD_VALUE  65536
#define MIN_MATRIX_FIELD_VALUE  -65536
#define MAX_NUMBER_OF_ROWS 6

/************************************
 * Structure declarations
 ************************************/
struct Matrix {
    int size;               // Number of rows|cols (rows == cols for square matrix)
    long **elements;        // Pointer to 2D array of long ints containing elements of matrix
};
typedef struct Matrix Matrix;


/************************************
 * Function declarations
 ************************************/
int createSquareMatrix( int size, Matrix **output );
void deleteSquareMatrix( Matrix *matrix );
int sumSquareMatrix( Matrix *m1, Matrix *m2, Matrix **output );
int subSquareMatrix(Matrix *m1, Matrix *m2, Matrix **output);
int multiplySquareMatrix( Matrix *m1, Matrix *m2, Matrix **output );
void copyMinorFromMatrix( Matrix *input, Matrix *output, int omitRow, int omitCol );
int detSquareMatrix( Matrix *matrix, long *result );

#endif //PROJEKT2_SQUAREMATRIX_H
