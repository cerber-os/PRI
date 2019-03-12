/*
 * File: main.c
 * Author: Paweł Wieczorek
 * Date: 09 Nov 2018
 * Description: Basic calculator
 */

// Motto tego projektu: „Teraz prędko, zanim dotrze do nas, że to bez sensu.”

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "Calculator.h"
#include "Stack.h"
#include "Variables.h"

/************************************
 * Macros definitions
 ************************************/
#define MAX_INPUT_SIZE 10000

// String displayed before each user input
#define PROMPT_STR     "$ "

// Escape-sequences used to color output of program
#define FORMAT_ERROR    "\033[31;1;4m"
#define FORMAT_DFT      "\033[0m"

/************************************
 * Program functions
 ************************************/

/*
 * Function:  handleError
 * --------------------
 *      prints error message
 *
 *      err:            error code
 *
 *      returns:        0 if err == 0, 1 otherwise
 *
 */
int handleErrors( int err )
{
    printf( FORMAT_ERROR );
    switch(err)
    {
        case CALC_SUCCESS:
            printf( FORMAT_DFT );
            return 0;
        case CALC_ERR_S2I_UNDEF_CHAR:
            printf( "Improper character found in input" );
            break;
        case CALC_ERR_S2I_TOO_BIG:
            printf( "Number in input is too big" );
            break;
        case CALC_ERR_CAV_INV_EQ:
            printf( "Invalid use of equals sign" );
            break;
        case CALC_ERR_CAV_INV_BRT_OPEN:
            printf( "Too many opening brackets" );
            break;
        case CALC_ERR_CAV_INV_BRT_CLD:
            printf( "Too many closing brackets" );
            break;
        case CALC_ERR_I2R_STACK_OOM:
        case CALC_ERR_RPN_STACK_OOM:
        case CALC_ERR_RPN_FUNC_OOM:
        case STACK_ERR_OOM:
        case CALC_OOM:
            printf( "Out of memory!" );
            break;
        case CALC_ERR_I2R_VAR_UNDEF:
            printf( "Undefined variable found in input" );
            break;
        case CALC_ERR_RPN_NODE_UNDEF:
        case CALC_ERR_I2R_NODE_UNDEF:
        case STACK_ERR_INV_TYPE:
            printf( "Invalid expression" );
            break;
        case CALC_ERR_I2R_STACK_FULL:
        case STACK_ERR_FULL:
            printf( "Stack overflow" );
            break;
        case CALC_ERR_I2R_MISS_BRACKET:
            printf( "Missing brackets in expression" );
            break;
        case CALC_ERR_RPN_POW_DOMAIN:
        case CALC_ERR_RPN_FUNC_DOMAIN:
            printf( "Computation of function failed: incorrect domain" );
            break;
        case CALC_ERR_RPN_PDW_RANGE:
        case CALC_ERR_RPN_FUNC_RANGE:
            printf( "Computation of function failed: out of range" );
            break;
        case CALC_ERR_RPN_FUNC_UNDEF:
            printf( "Undefined function found in input" );
            break;
        case CALC_ERR_RPN_ARGS_LACK:
        case STACK_ERR_EMPTY:
            printf( "More operators than arguments in expression" );
            break;
        case CALC_ERR_RPN_DIV_ZERO:
            printf( "Attempted to divide by zero!" );
            break;
        case CALC_ERR_RPN_OPER_UNDEF:
            printf( "Undefined operation found in input" );
            break;
        case VAR_ERR_NOT_FOUND:
            printf( "Undefined variable used in input" );
            break;
        case VAR_ERR_NO_FREE_SPACE:
            printf( "Maximum number of variables exceeded" );
            break;
        default:
            printf( "Unknown error occurred! Please try again" );
            break;
    }
    printf( " (Err=%d)\n" FORMAT_DFT, err );
    return 1;
}

/*
 * Function:  printHelp
 * --------------------
 *      displays program usage
 *
 */
void printHelp( void )
{
    puts( "               Simple calculator                 " );
    puts( "<-- Type expression below to get its result   -->" );
    puts( "<-- Available commands : sqrt, expN, logN     -->" );
    puts( "<--    where: N - base parameter (default: 2) -->" );
}

/*
 * Function:  main
 * --------------------
 *      main program logic
 *
 *      returns: 0 on success, other on error
 *
 */
int main( void )
{
    int err = 0;
    char *fgetsResult;
    char *input = malloc( MAX_INPUT_SIZE * sizeof(char) );              // Allocate memory for user input
    if( input == NULL )                                                 // We are out of memory!
    {
        puts( "Unable to allocate memory!" );
        return 1;
    }

    printHelp();
    while( 1 )
    {
        printf( PROMPT_STR );                                           // Display prompt
        fgetsResult = fgets( input, MAX_INPUT_SIZE, stdin );            // Get line entered by user
        if ( fgetsResult == NULL || !strcmp( input, "quit\n" ) )        // If he typed "quit" or sent EOF, do his wish
            break;
        err = calculateExpression( input );                             // Calculate this expression
        handleErrors( err );                                            // Check for errors
    }
    // Clean-up memory
    deleteVariables();                                                  // Free memory used by variables
    free( input );

    return 0;
}
