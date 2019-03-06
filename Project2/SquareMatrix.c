/*
 * File: SquareMatrix.c
 * Author: Paweł Wieczorek
 * Date: 25 Oct 2018
 * Description: Implementation of matrices and basic operations on them
 */
#include "SquareMatrix.h"
#include <stdlib.h>
#include <memory.h>

/*
 * Function:  createSquareMatrix
 * --------------------
 *      allocates memory on heap for structure and array holding matrix elements
 *
 *      size:    number of cols|rows (cols == rows for square matrix)
 *      output:  pointer to memory where pointer to structure should be stored
 *               ex. Matrix* matrix; createSquareMatrix( 3, &matrix ); //Now „matrix” contains pointer to created matrix
 *
 *      returns: 0 on success, -1 on out of memory
 *
 */
int createSquareMatrix( int size, Matrix **output )
{
    long** elements = calloc( ( size_t )size, sizeof( long* ) );                  // Allocate rows of 2D array
    if( elements == NULL )                                                        // We are out of memory
        return -1;

    *output = ( Matrix * ) malloc( sizeof( Matrix ) );                            // Allocate matrix structure
    if( *output == NULL )                                                         // We are out of memory
    {
        free( elements );
        return -1;
    }

    for( int column = 0; column < size; column++ )
    {
        elements[column] = calloc( ( size_t )size, sizeof(long) );
        if( elements[column] == NULL )                                               // We are out of memory
        {
            for( int createdColumns = 0; createdColumns < column; createdColumns++ ) // For each column we already created
                free( elements[createdColumns] );                                    // free it.
            free( elements );                                                        // Don't forget about cols...
            free( *output );                                                         // ...and output!
            return -1;
        }
    }

    ( *output )->size = size;
    ( *output )->elements = elements;
    return 0;
}

/*
 * Function:  deleteSquareMatrix
 * --------------------
 *      frees memory occupied by matrix structure and elements array
 *
 *      input:  initiated Matrix structure
 *
 */
void deleteSquareMatrix( Matrix *matrix )
{
    if( matrix == NULL )
        return;

    if( matrix->elements != NULL )
    {
        for( int row = 0; row < matrix->size; row++ )
            free( matrix->elements[row] );
    }
    free( matrix->elements );                                   // Free can deal with NULL
    free( matrix );
}

/*
 * Function:  sumSquareMatrix
 * --------------------
 *      creates matrix and sets its elements to sum of matrices being arguments
 *
 *      m1:      pointer to first Matrix structure
 *      m2:      pointer to second Matrix structure
 *      output:  pointer to memory where pointer to structure should be stored
 *               output->elements will store sum of matrices m1 and m2
 *
 *      returns: 0 on success, -2 if input matrices don't have the same size
 *
 */
int sumSquareMatrix( Matrix *m1, Matrix *m2, Matrix **output )
{
    if( m1-> size != m2->size )                                    // Can only sum matrices of the same size
        return -2;

    createSquareMatrix( m1->size, output );

    for( int row = 0; row < m1->size; row++ )
        for( int col = 0; col < m1->size; col++ )
        ( *output )->elements[row][col] = m1->elements[row][col] + m2->elements[row][col];

    return 0;
}

/*
 * Function:  subSquareMatrix
 * --------------------
 *      creates matrix and sets its elements to differential of matrices being arguments
 *      (for details look at comment block above function sumSquareMatrix)
 *
 */
int subSquareMatrix(Matrix *m1, Matrix *m2, Matrix **output)
{
    if( m1-> size != m2->size )                                    // Can only sub. matrices of the same size
        return -2;

    createSquareMatrix( m1->size, output );

    for( int row = 0; row < m1->size; row++ )
        for( int col = 0; col < m1->size; col++ )
            ( *output )->elements[row][col] = m1->elements[row][col] - m2->elements[row][col];

    return 0;
}

/*
 * Function:  multiplySquareMatrix
 * --------------------
 *      creates matrix and sets its elements to multiplication of matrices being arguments
 *
 *      m1:      pointer to first Matrix structure
 *      m2:      pointer to second Matrix structure
 *      output:  pointer to memory where pointer to structure should be stored
 *               output->elements will store multiplication of matrices m1 and m2
 *
 *      returns: 0 on success, -2 if input matrices don't have the same size
 *
 */
int multiplySquareMatrix( Matrix *m1, Matrix *m2, Matrix **output )
{
    if( m1-> size != m2->size )
        return -2;

    createSquareMatrix( m1->size, output );

    for( int rowInM1 = 0; rowInM1 < m1->size; rowInM1++ )              // For each row in matrix m1
        for( int colInM2 = 0; colInM2 < m2->size; colInM2++ )          // For each col in matrix m2
        {
            (*output)->elements[rowInM1][colInM2] = 0;                 // Set value in this cell to zero
            for (int r = 0; r < m2->size; r++)
                // From Cauchy's definition: multiply r-th value in row of matrix m1 with r-th value in col of m2
                (*output)->elements[rowInM1][colInM2] += m1->elements[rowInM1][r] * m2->elements[r][colInM2];
        }
    return 0;
}

/*
 * Function:  copyMinorFromMatrix
 * --------------------
 *      copies minor from "input" to "output" omitting specified one row and one col
 *
 *      input:  pointer to Matrix structure from which minor should be taken
 *      output: pointer to Matrix structure in which minor should be stored
 *      omitRow: number of row (counted from 0) which should be omitted
 *      omitCol: number of col (counted from 0) which should be omitted
 *
 */
void copyMinorFromMatrix( Matrix *input, Matrix *output, int omitRow, int omitCol )
{
    int offsetRow = 0;
    int offsetCol = 0;
    for( int row = 0; row < input->size; row++ )
    {
        if ( row == omitRow ) {
            offsetRow = 1;
            continue;
        }
        for( int col = 0; col < input->size; col++ )
        {
            if( col == omitCol ) {
                offsetCol = 1;
                continue;
            }
            output->elements[row - offsetRow][col - offsetCol] = input->elements[row][col];
        }
        offsetCol = 0;
    }
}
/*
 * Function:  detSquareMatrix
 * --------------------
 *      calculates determinant of matrix using Laplace expansion
 *
 *      matrix:  pointer to Matrix structure
 *      result:  pointer to long integer where determinant should be stored
 *
 *      returns: 0 on success, -1 on out of memory, -2 on long integer overflow
 *
 */
int detSquareMatrix( Matrix *matrix, long *result )
{
    if( matrix->size == 1 )
    {                                                                  // For matrix having only one element, determinant
        *result = matrix->elements[0][0];                              // is this element.
        return 0;                                                      // Nothing goes wrong - return 0
    }

    const int selectedRow = 0;                                         // Row along which we will calculate Laplace expansion
    long partialDeterminant = 0;
    long sumOfPartialDeterminant = 0;
    for( int col = 0; col < matrix->size; col++ )
    {
        Matrix *minor;
        if( createSquareMatrix( matrix->size - 1, &minor) != 0 )       // Can't create new matrix - out of memory
            return -1;
        copyMinorFromMatrix( matrix, minor, selectedRow, col );        // Copy minor to newly created matrix
        if( detSquareMatrix( minor, &partialDeterminant ) != 0)        // Child function returned non-zero code
            return -1;
        deleteSquareMatrix(minor);                                     // Free used memory

        if( ( col + selectedRow ) % 2 == 1 )                           // If sum of current coordinates is odd
            partialDeterminant *= -1;                                  // multiply it by -1 (from Laplace expansions formula)

        /*
         * Function __builtin_[a](operation)[b]_overflow(x, y, *res), where:
         *          a = s for signed ints, u for unsigned ints; operation = add, sub or mul; b = none for int, l for long and ll for long long
         * performs operation specified by its name, stores result in variable pointed by res and checks status of
         * processor flag OF (overflow). If flag is set (which means that result of arithmetic operation was bigger than
         * size of register in which it is stored) function return true (1). In such case function detSquareMatrix
         * returns -2, which means that matrix partial determinant is bigger than could be fit in long integer.
         *
         * This function is provided by GCC 5+ and Clang 3.4+. It's implementation in assembly varies depending on
         * target architecture.
         *
         * Thanks to this function, detSquareMatrix will return -2, rather than invalid result due to integer overflow
         */
        // Equivalent to: partialDeterminant = partialDeterminant * matrix->elements[selectedRow][col];
        if( __builtin_smull_overflow( partialDeterminant, matrix->elements[selectedRow][col] , &partialDeterminant ) )
            // Overflow in multiplication operation occurred
            return -2;

        // Equivalent to: sumOfPartialDeterminant = sumOfPartialDeterminant + partialDeterminant;
        if( __builtin_saddl_overflow( partialDeterminant, sumOfPartialDeterminant , &sumOfPartialDeterminant ) ) // Add result of function
            // Overflow occurred
            return -2;
    }
    *result = sumOfPartialDeterminant;
    return 0;
}