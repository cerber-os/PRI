/*
 * File: MatrixCalculator.c
 * Author: Paweł Wieczorek
 * Date: 27 Oct 2018
 * Description: Basic matrix calculator
 */

#include <stdio.h>
#include <stdlib.h>
#include "SquareMatrix.h"
#include "MatrixGUI.h"

/************************************
 * Macros definitions
 ************************************/
// Maximum number of matrices program is able to store
#define MAX_NUMBER_OF_MATRICES 30

// Codes used by function operationOnMatrix to distinguish operations
#define MATRICES_ADD        1
#define MATRICES_SUB        2
#define MATRICES_MULT       3

/************************************
 * Enums definitions
 ************************************/
// Enum containing "programmer friendly" names of menus
enum AvailableOptions {
    QUIT, CREATE_MATRIX, EDIT_MATRIX, PRINT_MATRIX, DELETE_MATRIX, ADD_MATRICES, SUB_MATRICES, MULTIPLY_MATRICES,
    DETERMINANT, HELP
};

/************************************
 * Program functions
 ************************************/

/*
 * Function:  printExistingMatrices
 * --------------------
 *      prints string "Existing matrices: " followed by ids and sizes of every created matrix
 *
 *      matricesMemory: pointer to list of saved matrices
 *
 */
void printExistingMatrices( Matrix** matricesMemory )
{
    printf( "Existing matrices:" );
    for( int i = 0; i < MAX_NUMBER_OF_MATRICES; i++ )
        if( matricesMemory[i] != NULL )                                                  // If matrix exists
            printf( " %d[%dx%d]", i, matricesMemory[i]->size, matricesMemory[i]->size ); // Print its id and size
    printf( "\n" );                                                                      // Remember to put \n at the end!
}

/*
 * Function:  findFirstFreeMatrixIndex
 * --------------------
 *      finds first free id for matrix
 *
 *      matricesMemory: pointer to list of saved matrices
 *
 *      returns: -1 if no free ids left; number in range (0, MAX_NUMBER_OF_MATRICES) being free matrix id
 *
 */
int findFirstFreeMatrixIndex( Matrix** matricesMemory )
{
   for( int i = 0; i < MAX_NUMBER_OF_MATRICES; i++ )
       if( matricesMemory[i] == NULL )                                                   // Is it free?
           return i;                                                                     // Yep, return it
   return -1;                                                                            // No free ids found
}

/*
 * Function:  menuCreateMatrix
 * --------------------
 *      displays and handles menu for creating new matrix
 *
 *      matricesMemory: pointer to list of saved matrices
 *
 */
void menuCreateMatrix( Matrix** matricesMemory )
{
    int matrixIndex, matrixSize;

    printExistingMatrices( matricesMemory );

    if( findFirstFreeMatrixIndex( matricesMemory ) == -1)                                // No free index left
        puts(FONT_RED_COLOR "There is no free space for new matrix. " DEFAULT_DISPLAY);

    matrixIndex = ( int )safeNumPrompt( "Index of new matrix: ", 0, MAX_NUMBER_OF_MATRICES );
    while( matricesMemory[matrixIndex] != 0 )                                            // While user doesn't provide free id, ask again
        matrixIndex = ( int )safeNumPrompt( FONT_RED_COLOR "This index is already taken. Try again: " DEFAULT_DISPLAY, 0, MAX_NUMBER_OF_MATRICES );

    matrixSize = ( int )safeNumPrompt( "Number of cols (=rows): ", 0, MAX_NUMBER_OF_ROWS );

    createSquareMatrix( matrixSize, &matricesMemory[matrixIndex] );
    editMatrixPrompt( matricesMemory[matrixIndex] );
}

/*
 * Function:  menuCreateMatrix
 * --------------------
 *      displays and handles menu for creating editing existing matrix
 *
 *      matricesMemory: pointer to list of saved matrices
 *
 */
void menuEditMatrix( Matrix** matricesMemory )
{
    printExistingMatrices( matricesMemory );

    int matrixIndex = ( int )safeNumPrompt( "Index of matrix to be edited: ", 0, MAX_NUMBER_OF_MATRICES );
    if( matricesMemory[matrixIndex] == NULL )                                            // Check if matrix exists
    {
        puts( FONT_RED_COLOR "There's no matrix with such index!" DEFAULT_DISPLAY );
        return;
    }
    editMatrixPrompt( matricesMemory[matrixIndex] );
}

/*
 * Function:  menuPrintMatrix
 * --------------------
 *      displays and handles menu for printing existing matrix
 *
 *      matricesMemory: pointer to list of saved matrices
 *
 */
void menuPrintMatrix( Matrix** matricesMemory )
{
    printExistingMatrices( matricesMemory );

    int matrixIndex = ( int )safeNumPrompt( "Index of matrix to be printed: ", 0, MAX_NUMBER_OF_MATRICES );
    if( matricesMemory[matrixIndex] == NULL )                                            // Check if matrix exists
    {
        puts( FONT_RED_COLOR "There's no matrix with such index!" DEFAULT_DISPLAY );
        return;
    }
    printMatrixAsTable( matricesMemory[matrixIndex] );
}

/*
 * Function:  menuDeleteMatrix
 * --------------------
 *      displays and handles menu for deleting existing matrix
 *
 *      matricesMemory: pointer to list of saved matrices
 *
 */
void menuDeleteMatrix( Matrix** matricesMemory )
{
    printExistingMatrices( matricesMemory );

    int matrixIndex = ( int )safeNumPrompt( "Index of matrix to be deleted: ", 0, MAX_NUMBER_OF_MATRICES );
    if( matricesMemory[matrixIndex] == NULL )                                            // Check if matrix exists
    {
        puts( FONT_RED_COLOR "There's no matrix with such index!" DEFAULT_DISPLAY );
        return;
    }

    deleteSquareMatrix( matricesMemory[matrixIndex] );                                   // Delete matrix structure
    matricesMemory[matrixIndex] = NULL;                                                  // Set pointer to NULL
}

/*
 * Function:  operationOnMatrices
 * --------------------
 *      displays and handles menu for operation on matrix
 *
 *      matricesMemory: pointer to list of saved matrices
 *      operation: one of the following values: (MATRICES_ADD | MATRICES_DIFF | MATRICES_MULT)
 *
 */
void operationOnMatrices(  Matrix** matricesMemory, int operation )
{
    Matrix* result;                                 // Result of operation
    int firstMatrix, secondMatrix;                  // Indexes of first and second matrix
    int resultMatrixIndex = 0;                      // Index of matrix containing result
    int errorCode = 0;                              // Error code returned by functions called

    printExistingMatrices( matricesMemory );

    firstMatrix = ( int )safeNumPrompt( "Index of first matrix: ", 0, MAX_NUMBER_OF_MATRICES );
    if( matricesMemory[firstMatrix] == NULL )       // Matrix does not exist
    {
        puts( FONT_RED_COLOR "There's no matrix with such index!" DEFAULT_DISPLAY );
        return;
    }

    secondMatrix = ( int )safeNumPrompt( "Index of second matrix: ", 0, MAX_NUMBER_OF_MATRICES );
    if( matricesMemory[secondMatrix] == NULL )      // Matrix does not exist
    {
        puts( FONT_RED_COLOR "There's no matrix with such index!" DEFAULT_DISPLAY );
        return;
    }

    printf( "Matrix #%d:\n",firstMatrix  );
    printMatrixAsTable( matricesMemory[firstMatrix] );
    printf( "Matrix #%d:\n",secondMatrix  );
    printMatrixAsTable( matricesMemory[secondMatrix] );

    if( operation == MATRICES_ADD )                 // Add matrices
    {
        errorCode = sumSquareMatrix(matricesMemory[firstMatrix], matricesMemory[secondMatrix], &result);
        printf("Sum of matrix #%d and #%d is: \n", firstMatrix, secondMatrix);
    } else if( operation == MATRICES_SUB )         // Subtract matrices
    {
        errorCode = subSquareMatrix(matricesMemory[firstMatrix], matricesMemory[secondMatrix], &result);
        printf("Differential of matrix #%d and #%d is: \n", firstMatrix, secondMatrix);
    } else                                          // Multiply matrices
    {
        errorCode = multiplySquareMatrix(matricesMemory[firstMatrix], matricesMemory[secondMatrix], &result);
        printf("Multiplication of matrix #%d and #%d is: \n", firstMatrix, secondMatrix);
    }

    if( errorCode == -1 )                           // Function returned out-of-memory error
    {
        puts( FONT_RED_COLOR "Out of memory occurred!" DEFAULT_DISPLAY );
        return;
    }else if( errorCode == -2 )                     // Function returned, because matrices does not have the same size
    {
        puts( FONT_RED_COLOR "Matrices must have the same size!" DEFAULT_DISPLAY );
        return;
    }

    printMatrixAsTable( result );

    resultMatrixIndex = findFirstFreeMatrixIndex( matricesMemory ); // Find first free index for matrix storing our result
    if( resultMatrixIndex == -1 )                   // If there is not free index, show error
    {
        puts( FONT_RED_COLOR "There's no space to save result" DEFAULT_DISPLAY );
        deleteSquareMatrix( result );
        return;
    }
    printf( "Result was saved at index #%d.\n", resultMatrixIndex);
    matricesMemory[resultMatrixIndex] = result;     // Save matrix with result
}

/*
 * Functions:  menu(Add | Sub | Multiply)Matrices
 * --------------------
 *      does operation specified in function name by calling operationOnMatrices function with appropriate parameter
 *
 *      matricesMemory: pointer to list of saved matrices
 *
 */
void menuAddMatrices( Matrix** matricesMemory )
{
    operationOnMatrices( matricesMemory, MATRICES_ADD );
}

void menuSubMatrices( Matrix** matricesMemory )
{
    operationOnMatrices( matricesMemory, MATRICES_SUB );
}

void menuMultiplyMatrices( Matrix** matricesMemory )
{
    operationOnMatrices( matricesMemory, MATRICES_MULT );
}

/*
 * Function:  menuDeterminant
 * --------------------
 *      displays and handles menu for calculating matrix determinant
 *
 *      matricesMemory: pointer to list of saved matrices
 *
 */
void menuDeterminant( Matrix** matricesMemory )
{
    long result = 0;
    int matrixIndex;
    int errorCode = 0;

    printExistingMatrices( matricesMemory );

    matrixIndex = ( int )safeNumPrompt( "Index of matrix: ", 0, MAX_NUMBER_OF_MATRICES);
    if( matricesMemory[matrixIndex] == NULL)                    // If matrix does not exist, print error
    {
        puts( FONT_RED_COLOR "There's no matrix with such index!" DEFAULT_DISPLAY );
        return;
    }

    errorCode = detSquareMatrix( matricesMemory[matrixIndex], &result );

    if( errorCode == -1 ) // If detSquareMatrix returned -1, we are out-of-memory
    {
        puts( FONT_RED_COLOR "Out of memory!" DEFAULT_DISPLAY );
        return;
    } else if( errorCode == -2 ) // If detSquareMatrix returned -2, integer overflow occurred
    {
        puts( FONT_RED_COLOR "Result is bigger than MAX_LONG!" DEFAULT_DISPLAY );
        return;
    }

    printMatrixAsTable( matricesMemory[matrixIndex] );
    printf( "Determinant of matrix #%d = %ld\n", matrixIndex, result );
}

/*
 * Function:  printHelp
 * --------------------
 *      displays program usage
 *
 */
void printHelp( void )
{
    puts( "Basic matrices calculator" );
    puts( "Available commands:" );
    puts( "1.\tCreate matrix" );
    puts( "2.\tEdit matrix" );
    puts( "3.\tPrint matrix" );
    puts( "4.\tDelete matrix" );
    puts( "5.\tAdd matrices" );
    puts( "6.\tSubtract matrices" );
    puts( "7.\tMultiply matrices" );
    puts( "8.\tDeterminant" );
    puts( "9.\tHelp" );
    puts( "0.\tQuit" );
    puts( "Type a number of a command to execute it" );
}

/*
 * Function:  main
 * --------------------
 *      main program logic
 *
 *      returns: 0 on success, other on error
 *
 */
int main() {
    int quitRequested = 0;                                                  // Flag set if user wants to exit program
    enum AvailableOptions command_selected = 0;
    Matrix** matricesMemory;

    matricesMemory = calloc( MAX_NUMBER_OF_MATRICES, sizeof( Matrix* ) );   // Allocate memory for matrices array
    if( matricesMemory == NULL )                                            // If we are unable to do this, exit with error
    {
        puts( FONT_RED_COLOR "Can't allocate memory." DEFAULT_DISPLAY );
        return 1;
    }

    printHelp();                                                            // Show help
    while( !quitRequested )
    {
        command_selected =  ( int )safeNumPrompt( "> ", 0,  9 );            // Get option from user
        printf( "> %d\n", command_selected );                               // Print selected option

        switch( command_selected )
        {
            case CREATE_MATRIX:
                menuCreateMatrix( matricesMemory );
                break;
            case EDIT_MATRIX:
                menuEditMatrix( matricesMemory );
                break;
            case PRINT_MATRIX:
                menuPrintMatrix( matricesMemory );
                break;
            case DELETE_MATRIX:
                menuDeleteMatrix( matricesMemory );
                break;
            case ADD_MATRICES:
                menuAddMatrices( matricesMemory );
                break;
            case SUB_MATRICES:
                menuSubMatrices( matricesMemory );
                break;
            case MULTIPLY_MATRICES:
                menuMultiplyMatrices( matricesMemory );
                break;
            case DETERMINANT:
                menuDeterminant( matricesMemory );
                break;
            case HELP:
                printHelp();
                break;
            case QUIT:
                quitRequested = 1;                                           // Set flag to end program
                break;
            default:
                puts( FONT_RED_COLOR "Wrong option!" DEFAULT_DISPLAY );
                break;
        }
    }

    // Free memory occupied by not deleted matrices
    for( int i = 0; i < MAX_NUMBER_OF_MATRICES; i++ )
        if( matricesMemory[i] != NULL )
            deleteSquareMatrix( matricesMemory[i] );

    free(matricesMemory);                                                   // Free memory used by matrices array

    return 0;
}