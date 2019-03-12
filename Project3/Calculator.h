/*
 * File: Calculator.h
 * Author: Paweł Wieczorek
 * Date: 12 Nov 2018
 * Description: Header of file Calculator.c
 */


#ifndef PROJEKT3_CALCULATOR_H
#define PROJEKT3_CALCULATOR_H

#include "Structures.h"

/**********************************
 * Constants definitions
 *********************************/
#define MAX_NUMBER_OF_NODES         10000               // Maximum number of elements in array of structures
#define MAX_STACK_SIZE              1000                // Maximum number of elements on stack

// Error codes
enum {  CALC_SUCCESS, CALC_OOM,

        CALC_ERR_S2I_UNDEF_CHAR,
        CALC_ERR_S2I_TOO_BIG,

        CALC_ERR_CAV_INV_EQ,
        CALC_ERR_CAV_INV_BRT_OPEN,
        CALC_ERR_CAV_INV_BRT_CLD,

        CALC_ERR_I2R_STACK_OOM,
        CALC_ERR_I2R_VAR_UNDEF,
        CALC_ERR_I2R_NODE_UNDEF,
        CALC_ERR_I2R_STACK_FULL,
        CALC_ERR_I2R_MISS_BRACKET,

        CALC_ERR_RPN_FUNC_DOMAIN,
        CALC_ERR_RPN_FUNC_RANGE,
        CALC_ERR_RPN_FUNC_UNDEF,
        CALC_ERR_RPN_FUNC_OOM,
        CALC_ERR_RPN_STACK_OOM,
        CALC_ERR_RPN_ARGS_LACK,
        CALC_ERR_RPN_DIV_ZERO,
        CALC_ERR_RPN_POW_DOMAIN,
        CALC_ERR_RPN_PDW_RANGE,
        CALC_ERR_RPN_OPER_UNDEF,
        CALC_ERR_RPN_NODE_UNDEF
};

/**********************************
 * Functions declarations
 *********************************/
int freeMemoryAndPassErr( int err, struct Node* n1, struct Node* n2 );
int checkBracketsCnt( Infix_Node *nodes );
int checkEqualsSigns( Infix_Node *nodes );
void convertSymbolsInInfix( Infix_Node *nodes );
int calculateFunction( char* str, double parameter, double *result );

int strToInfixNodes( char *input, Infix_Node *nodes );
int convertAndValidateInfixNodes( Infix_Node *nodes );
int infixNodesToRPNNodes(Infix_Node *input, RPN_Node *output);
int calculateRPNNodes(RPN_Node *nodes, double *result);

int calculateExpression(char* input);

#endif //PROJEKT3_CALCULATOR_H
