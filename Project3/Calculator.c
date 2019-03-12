/*
 * File: Calculator.c
 * Author: Paweł Wieczorek
 * Date: 09 Nov 2018
 * Description: Calculator of string expression
 */
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

#include "Calculator.h"
#include "Strings.h"
#include "Stack.h"
#include "Variables.h"
#include "Structures.h"

/*
* Function: freeMemoryAndPassErr
* --------------------
*      frees memory occupied by strings and node structures. Also returns value passed in parameter err, which allows
*      to use it with return statement
*
*      err:             value to be returned by function
*      n1/n2:           list of nodes structures to be fried
*
*      returns: integer given in parameter err
*
*/
int freeMemoryAndPassErr( int err, struct Node* n1, struct Node* n2 )
{
    struct Node* origN1 = n1;                                   // Store original pointers to lists
    struct Node* origN2 = n2;

    for( ; n1 != NULL && n1->type != EMPTY; n1++ )              // While there's a node in our list
        if( n1->type == FUNCTION || n1->type == VARIABLE )      // If it stores string (i.e. is function or variable)
        {
            free( n1->field.str );                              // Free memory occupied by it
            n1->field.str = NULL;                               // and set to NULL to avoid further usages
        }

    for( ; n2 != NULL && n2->type != EMPTY; n2++ )              // The same as for n2 list
        if( n2->type == FUNCTION || n2->type == VARIABLE )
        {
            free( n2->field.str );
            n2->field.str = NULL;
        }

    free( origN1 );                                             // Finally, free lists
    free( origN2 );
    return err;                                                 // Return error code given to function
}

/*
* Function: checkBracketsCnt
* --------------------
*      checks if expression contains the same number of opening and closing brackets
*
*      nodes:           pointer to list of Infix_node structures
*
*      returns: CALC_SUCCESS                 if expression is valid
*               CALC_ERR_CAV_INV_BRT_OPEN    if there's too many opening brackets
*               CALC_ERR_CAV_INV_BRT_CLS     if there's too many closing brackets
*
*/
int checkBracketsCnt( Infix_Node *nodes )
{
    int numberOfOpenedBrackets = 0;
    for( int i = 0; nodes[i].type != EMPTY; i++ )               // For each node in list
    {
        if( nodes[i].type == BRACKET_LEFT )                     // It's opening bracket
            numberOfOpenedBrackets++;
        if( nodes[i].type == BRACKET_RIGHT )                    // It's closing bracket
            numberOfOpenedBrackets--;
        if( numberOfOpenedBrackets < 0 )                        // There's more closing than opening brackets
            return CALC_ERR_CAV_INV_BRT_CLD;
    }
    if( numberOfOpenedBrackets != 0 )                           // Some opened brackets left
        return CALC_ERR_CAV_INV_BRT_OPEN;

    return CALC_SUCCESS;
}

/*
* Function: checkEqualsSigns
* --------------------
*      checks if expression correctly uses equals signs
*
*      nodes:           pointer to list of Infix_node structures
*
*      returns: CALC_SUCCESS                if expression is valid
*               CALC_ERR_CAV_INV_EQ         if there's invalidly used equals sign
*
*/
int checkEqualsSigns( Infix_Node *nodes )
{
    if(nodes[0].type == EQUALS)                                 // If equals sign is at the beginning of expression
        return CALC_ERR_CAV_INV_EQ;

    if( nodes[1].type == EQUALS && nodes[0].type != VARIABLE )  // If before equals sign, there's no variable
        return CALC_ERR_CAV_INV_EQ;

    for(int i = 2; nodes[i].type != EMPTY; i++)                 // Check if there's no more equals signs in expression
        if( nodes[i].type == EQUALS )
            return CALC_ERR_CAV_INV_EQ;
    return CALC_SUCCESS;
}

/*
* Function: convertSymbolsInInfix
* --------------------
*      converts: "**" to "^", variables with brackets to functions and "-" before number to negative number
*
*      nodes:           pointer to list of Infix_node structures
*
*/
void convertSymbolsInInfix( Infix_Node *nodes )
{
    // "-" at the beginning of expression
    if(nodes[0].type == OPERATOR && nodes[0].field.chr == '-' && nodes[1].type == NUMBER)
    {
        nodes[1].field.number *= -1;
        nodes[0].type = DELETED;
    }
    for(int i = 1; nodes[i].type != EMPTY; i++)
        if(nodes[i].type == OPERATOR && nodes[i].field.chr == '-' && nodes[i+1].type == NUMBER && nodes[i-1].type != NUMBER)
        {                                                                           // "-" not between two number
            nodes[i+1].field.number *= -1;
            nodes[i].type = DELETED;
        } else if( nodes[i].type == OPERATOR && nodes[i].field.chr == '*' && \
                   nodes[i - 1].type == OPERATOR && nodes[i - 1].field.chr == '*')
        {                                                                           // "**" to "^"
            nodes[i].field.chr = '^';
            nodes[i-1].type = DELETED;
        }

    for(int i = 0 ; nodes[i].type != EMPTY; i++)
        if( nodes[i].type == VARIABLE && nodes[i + 1].type == BRACKET_LEFT )        // Variable with bracket to function
            nodes[i].type = FUNCTION;
}

/*
* Function: calculateFunction
* --------------------
*      calculates the value of function given by name
*
*      str:             name of function
*      parameter:       parameter of function
*      result:          pointer to variable where result should be stored
*
*      returns: CALC_SUCCESS               on success,
*               CALC_ERR_RPN_FUNC_UNDEF    if given function was not found
*               CALC_ERR_RPN_FUNC_DOMAIN   if provided parameter is incorrect for this function
*               CALC_ERR_RPN_FUNC_RANGE    if the result of function is too big
*               CALC_ERR_RPN_FUNC_OOM      if out of memory
*
*/
int calculateFunction( char* str, double parameter, double *result )
{
    char *endPtr;                                   // Pointer to the char on which function name ends
    char *name = getText( str, &endPtr );           // Get function name
    double opt;
    int err = CALC_SUCCESS;
    errno = 0;                                      // Clear libc error flag

    if( name == NULL )                              // We are out of memory
        return CALC_ERR_RPN_FUNC_OOM;
    if( *endPtr == '\0' )                           // If there's no optional parameter after func name, set it to 2
        opt = 2;
    else
        opt = strtod( endPtr, 0 );                  // Otherwise, get it from string

    if( !strcmp( name, "sqrt" ) )                   // Strcmp return 0 when strings are similar
        *result = sqrt( parameter );                // Get square root
    else if( !strcmp( name, "log" ) )               // libc provide only logarithm with base "10" so we have...
        *result = log( parameter ) / log( opt );    // ...to change it ourselves
    else if( !strcmp( name, "exp" ) )
        *result = pow( opt, parameter );            // Get power
    else
        err =  CALC_ERR_RPN_FUNC_UNDEF;             // Undefined function name was provided

    if( errno == EDOM )                             // Check if domain was invalid
        err = CALC_ERR_RPN_FUNC_DOMAIN;
    else if( errno == ERANGE )                      // Check if range was invalid
        err = CALC_ERR_RPN_FUNC_RANGE;

    free( name );                                   // Free memory occupied by name string
    return err;
}

/*
* Function: strToInfixNodes
* --------------------
*      converts given string to list of structures representing its infix notation
*
*      input:           string to be converted
*      nodes:           pointer to list of structures to be filled
*
*      returns: CALC_SUCCESS                on success,
*               CALC_ERR_S2I_UNDEF_CHAR     if unknown character was found,
*               CALC_ERR_S2I_TOO_BIG        if number in string is too big for double type
*
*/
int strToInfixNodes( char *input, Infix_Node *nodes )
{
    int currentNode = 0;
    char *endPosition = NULL;                                       // Pointer to the char on which strtod stops
    errno = 0;                                                      // Clear libc error flag

    strRemoveWhitespaces( input );                                  // Remove all whitespaces from string

    for( ; ( *input != '\0' ) && ( *input != '\n' ); input++ )      // Iterate over string until NULL or NL found
    {
        if( isDigit( *input ) )                                     // Symbol is digit
        {
            nodes[currentNode].type = NUMBER;
            nodes[currentNode].field.number = strtod( input, &endPosition ); // Convert it to double
            input = endPosition - 1;                                // Continue where strtod stopped
        } else if ( isOperator( *input ) )                          // Symbol is operator
        {
            nodes[currentNode].type = OPERATOR;
            nodes[currentNode].field.chr = *input;
        } else if ( isEquation( *input ) )                          // Symbol is "=" sign
        {
            nodes[currentNode].type = EQUALS;
            nodes[currentNode].field.chr = *input;
        } else if ( isChar( *input ) )                              // Symbol is character
        {
            nodes[currentNode].type = VARIABLE;
            nodes[currentNode].field.str = getTextWithNumbers( input, &endPosition ); // Get text with numbers
            input = endPosition - 1;                                // Continue where above func stopped
        } else if ( *input == '(' )                                 // Symbol is left bracket
        {
            nodes[currentNode].type = BRACKET_LEFT;
            nodes[currentNode].field.chr = *input;
        } else if ( *input == ')' )                                 // Symbol is right bracket
        {
            nodes[currentNode].type = BRACKET_RIGHT;
            nodes[currentNode].field.chr = *input;
        } else
            return CALC_ERR_S2I_UNDEF_CHAR;                         // Unknown character was found

        currentNode++;                                              // Move to next node

        if( errno == ERANGE )                                       // Check if strtod returns overflow error
            return CALC_ERR_S2I_TOO_BIG;
    }

    return CALC_SUCCESS;
}

/*
* Function: convertAndValidateInfixNodes
* --------------------
*      does basic conversion and validation of infix notation
*
*      nodes:           pointer to list of structures to be filled
*
*      returns: CALC_SUCCESS                on success,
*               CALC_ERR_CAV_INV_EQ         if invalid usage of equals signs occurred
*               CALC_ERR_CAV_INV_BRT_OPEN   if there's too many opening brackets
*               CALC_ERR_CAV_INV_BRT_CLD    if there's too many closing brackets
*
*/
int convertAndValidateInfixNodes( Infix_Node *nodes )
{
    int err = checkEqualsSigns( nodes );                        // Check correctness of equals signs
    if( err )
        return err;

    err = checkBracketsCnt( nodes );                            // Check correctness of brackets
    if( err )
        return err;
    convertSymbolsInInfix( nodes );                             // Convert expression to more "programmer friendly"

    return CALC_SUCCESS;
}

/*
* Function: infixNodesToRPNNodes
* --------------------
*      converts expression in infix to Reverse Polish notation
*
*      input:               pointer to list of structures containing infix notation
*      output:              pointer to list of structures in which rpn notation should be saved
*
*      returns: CALC_SUCCESS                on success,
*               CALC_ERR_I2R_STACK_OOM      if out of memory
*               CALC_ERR_I2R_VAR_UNDEF      if expression contains undefined variable
*               CALC_ERR_I2R_STACK_FULL     if stack is full
 *              CALC_ERR_I2R_MISS_BRACKET   if there's missing bracker
 *              CALC_ERR_I2R_NODE_UNDEF     if unknown node was found in input
*
*/
int infixNodesToRPNNodes(Infix_Node *input, RPN_Node *output) {
    int currentRPNNode = 0;
    int err = CALC_SUCCESS;
    double result = 0;
    char curOp;

    char stackValueChr = 0;
    struct Node *stackStruct = NULL;
    char *stackValueStr = NULL;
    Stack *stack;

    if( createStack(&stack, MAX_STACK_SIZE) == STACK_ERR_OOM )                          // Create stack
        return CALC_ERR_I2R_STACK_OOM;

    for ( int i = 0; err == CALC_SUCCESS && input[i].type != EMPTY; i++ )               // Until error occurred or input ends
        switch ( input[i].type ) {
            case NUMBER:                                                                // Output number
                output[currentRPNNode].type = NUMBER;
                output[currentRPNNode].field.number = input[i].field.number;
                currentRPNNode++;
                break;
            case VARIABLE:
                if( getVariableValue( input[i].field.str, &result ) == VAR_ERR_NOT_FOUND ) // Get variable value and output as number
                    err = CALC_ERR_I2R_VAR_UNDEF;
                output[currentRPNNode].type = NUMBER;
                output[currentRPNNode].field.number = result;
                currentRPNNode++;
                break;
            case FUNCTION:
                if( pushFunctionToStack( stack, strdup( input[i].field.str ) ) != STACK_SUCCESS) // Push function to stack
                    err = CALC_ERR_I2R_STACK_FULL;
                break;
            case OPERATOR:
                while( popOperatorFromStack(stack, &stackValueChr) == STACK_SUCCESS ) {         // While there're operators on stack
                    curOp = input[i].field.chr;                                                 // Take it
                    if( ( isRightAssocOperator( curOp ) && getOpPriority( stackValueChr ) > getOpPriority( curOp ) ) ||
                        ( isLeftAssocOperator( curOp ) && getOpPriority( stackValueChr ) >= getOpPriority( curOp ) ) )
                    {
                        output[currentRPNNode].type = OPERATOR;                                 // It should go first - output it
                        output[currentRPNNode].field.chr = stackValueChr;
                        currentRPNNode++;
                    } else {
                        // It's operator that should be there - push it back and run away
                        if( pushOperatorToStack( stack, stackValueChr ) != STACK_SUCCESS )
                            err = CALC_ERR_I2R_STACK_FULL;
                        break;
                    }
                }

                if( pushOperatorToStack( stack, input[i].field.chr ) != STACK_SUCCESS )     // Push current operator to stack
                    err = CALC_ERR_I2R_STACK_FULL;
                break;
            case BRACKET_LEFT:
                if( pushOperatorToStack( stack, input[i].field.chr ) != STACK_SUCCESS )     // Push bracket to stack
                    err = CALC_ERR_I2R_STACK_FULL;
                break;
            case BRACKET_RIGHT:
                if( popOperatorFromStack( stack, &stackValueChr ) != STACK_SUCCESS)         // Take operator from stack
                {
                    err = CALC_ERR_I2R_MISS_BRACKET;
                    break;
                }

                while ( stackValueChr != '(' ) {                                            // Until we took opening bracket
                    output[currentRPNNode].type = OPERATOR;                                 // Output operator
                    output[currentRPNNode].field.chr = stackValueChr;
                    currentRPNNode++;

                    if( popOperatorFromStack( stack, &stackValueChr ) != STACK_SUCCESS )    // Take next operator from stack
                    {
                        err = CALC_ERR_I2R_MISS_BRACKET;
                        break;
                    }
                }

                if( popFunctionFromStack( stack, &stackValueStr ) == STACK_SUCCESS )        // If there's function on top of stack
                {
                    output[currentRPNNode].type = FUNCTION;                                 // Output it
                    output[currentRPNNode].field.str = stackValueStr;
                    currentRPNNode++;
                }
                break;
            case DELETED:                                                       // Ignore deleted nodes
                break;
            default:
                printf( "[DEBUG] UNDEF NODE IN I2R: %d %p", input[i].type, (void *)&input[i] );
                err = CALC_ERR_I2R_NODE_UNDEF;                                  // Oops! Something goes terribly wrong
        }

    while ( !err && popStructFromStack( stack, &stackStruct ) != STACK_ERR_EMPTY ) // Copy rest of the symbols left on stack
    {
        output[currentRPNNode].type = stackStruct->type;
        if( stackStruct->type == OPERATOR )
            output[currentRPNNode].field.chr = stackStruct->field.chr;
        else
            output[currentRPNNode].field.str = stackStruct->field.str;
        currentRPNNode++;
    }

    deleteStack( stack );                                                       // Free memory occupied by stack
    return err;
}

/*
* Function: calculateRPNNodes
* --------------------
*      calculates value of expression given in RPN notification
*
*      nodes:           list of RPN_Node structures to be calculated
*      result:          result of expression
*
*      returns: CALC_SUCCESS                on success,
*               CALC_ERR_RPN_STACK_OOM      if out of memory
*               CALC_ERR_RPN_ARGS_LACK      if there's not enough arguments for operand
*               CALC_ERR_RPN_DIV_ZERO       if attempted to divide by zero
*               CALC_ERR_RPN_POW_DOMAIN     if domain of power was invalid
*               CALC_ERR_RPN_PDW_RANGE      if out of range occurred
*               CALC_ERR_RPN_OPER_UNDEF     if operation is undefined
*               CALC_ERR_RPN_NODE_UNDEF     if node is undefined
 *              STACK_ERR_****              if stack error occurred
*
*/
int calculateRPNNodes( RPN_Node *nodes, double *result )
{
    double valA = 0;
    double valB = 0;
    int err = CALC_SUCCESS;                                                     // Stores return code

    Stack *stack;                                                               // Pointer to stack
    if( createStack( &stack, MAX_STACK_SIZE )  == STACK_ERR_OOM )               // We are out of memory
        return CALC_ERR_RPN_STACK_OOM;

    for( int i = 0; err == CALC_SUCCESS && nodes[i].type != EMPTY; i++ )        // For every node
        switch ( nodes[i].type ) {
            case NUMBER:
                err = pushNumberToStack( stack, nodes[i].field.number );        // Push number on stack
                break;
            case FUNCTION:
                if( popNumberFromStack( stack, &valA ) )                        // Get argument from stack
                {
                    err = CALC_ERR_RPN_ARGS_LACK;                               // There's no number on top
                    break;
                }
                err = calculateFunction( nodes[i].field.str, valA, &valB );     // Calculate result of function
                pushNumberToStack( stack, valB );                               // Push result back to stack
                break;
            case OPERATOR:
                if( popNumberFromStack( stack, &valA ) || popNumberFromStack( stack, &valB ) )
                    err = CALC_ERR_RPN_ARGS_LACK;
                else switch( nodes[i].field.chr )
                {
                    case '+':
                        err = pushNumberToStack( stack, valB + valA );          // Push back sum of args
                        break;
                    case '-':
                        err = pushNumberToStack( stack, valB - valA );          // Push back sub of args
                        break;
                    case '*':
                        err = pushNumberToStack( stack, valB * valA );          // Push back mul of args
                        break;
                    case '/':
                        if( valA == 0 )                                         // Check if 2. argument is zero
                            err = CALC_ERR_RPN_DIV_ZERO;
                        else
                            err = pushNumberToStack( stack, valB / valA );      // Push back div of args
                        break;
                    case '^':
                        errno = 0;                                              // Clear libc error flag
                        err = pushNumberToStack( stack, pow( valB, valA ) );    // Push back power of args
                        if( !err && errno == EDOM )                             // Check for invalid domain
                            err = CALC_ERR_RPN_POW_DOMAIN;
                        else if( errno == ERANGE )                              // Check for invalid range
                            err = CALC_ERR_RPN_PDW_RANGE;
                        break;
                    default:
                        err = CALC_ERR_RPN_OPER_UNDEF;                          // We found undefined operator!
                }
                break;
            default:
                err = CALC_ERR_RPN_NODE_UNDEF;                                  // Even worse: undefined node!
        }

    if( !err )
        err = popNumberFromStack( stack, result );                              // Get result from stack
    deleteStack( stack );                                                       // Delete stack
    return err;
}

/*
* Function: calculateExpression
* --------------------
*      calculates expression given in string
*
*      input:   expression to be calculated
*
*      returns: CALC_SUCCESS on success, number of error otherwise
*
*/

int calculateExpression( char* input )
{
    Infix_Node *node;                                                   // Expression in infix notification 
    RPN_Node* rpn_node;                                                 // in Reverse-Polish notification
    double result = 0;                                                  // Result of expression
    int err;                                                            // Variable storing error code of function

    node = calloc( MAX_NUMBER_OF_NODES, sizeof( Infix_Node ) );         // Allocate memory for arrays of structs
    rpn_node = calloc( MAX_NUMBER_OF_NODES, sizeof( RPN_Node ) );

    if( node == NULL || rpn_node == NULL )                              // We are out of memory
        return CALC_OOM;

    err = strToInfixNodes( input, node );                               // Convert input string to infix notif.
    if( err != CALC_SUCCESS )                                           // Error occurred - free memory and return error
        return freeMemoryAndPassErr(err, node, rpn_node);
    err = convertAndValidateInfixNodes( node );                         // Clean up and verify infix notif.
    if( err != CALC_SUCCESS )
        return freeMemoryAndPassErr(err, node, rpn_node);

    if( node[0].type == VARIABLE && node[1].type == EMPTY )             // If it's just variable name, print its
    {                                                                   // value
        err = getVariableValue( node[0].field.str, &result );
        if( err != CALC_SUCCESS )
            return freeMemoryAndPassErr(err, node, rpn_node);

        printf( "%g\n", result );
        return freeMemoryAndPassErr(CALC_SUCCESS, node, rpn_node);;
    }

    if( node[1].type == EQUALS )                                        // If expression ***saves*** result in var
        err = infixNodesToRPNNodes( node + 2, rpn_node );               // omit it while converting to RPN notif.
    else
        err = infixNodesToRPNNodes( node, rpn_node );

    if( err != CALC_SUCCESS )
        return freeMemoryAndPassErr(err, node, rpn_node);

    err = calculateRPNNodes( rpn_node, &result );                       // Calculate result of expression
    if( err != CALC_SUCCESS )
        return freeMemoryAndPassErr(err, node, rpn_node);

    if( node[1].type == EQUALS )                                        // If user want to save it in variable
        setVariable( node[0].field.str, result );                       // .. do his wish
    else
        printf( "%g\n", result );                                       // Otherwise, print result on screen

    return freeMemoryAndPassErr(CALC_SUCCESS, node, rpn_node);
}
