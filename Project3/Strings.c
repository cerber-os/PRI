/*
 * File: Strings.c
 * Author: Paweł Wieczorek
 * Date: 09 Nov 2018
 * Description: Functions used to manipulate or analyze strings
 */

#include <string.h>
#include "Strings.h"

/*
 * Function:  isChar
 * --------------------
 *      checks if provided character is a letter; returns 1 if yes, 0 otherwise
 */
int isChar( char c )
{
    return ( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) ) ? 1 : 0;
}

/*
 * Function:  isDigit
 * --------------------
 *      checks if provided character is a digit; returns 1 if yes, 0 otherwise
 */
int isDigit( char c )
{
    return ( c >= '0' && c <= '9' ) ? 1 : 0;
}

/*
 * Function:  isOperator
 * --------------------
 *      checks if provided character is an operation; returns 1 if yes, 0 otherwise
 */
int isOperator( char c )
{
    return ( c == '+' || c == '-' || c == '*' || c == '/' || c == '^' ) ? 1 : 0;
}

/*
 * Function:  isQuation
 * --------------------
 *      checks if provided character is an equation sign; returns 1 if yes, 0 otherwise
 */
int isEquation( char c )
{
    return ( c == '=' ) ? 1 : 0;
}

/*
 * Function:  isRightAssocOperator
 * --------------------
 *      checks if provided operator is right associative; returns 1 if yes, 0 otherwise
 */
int isRightAssocOperator(char c)
{
    if( c == '^' )
        return 1;
    return 0;
}

/*
 * Function:  isLeftAssocOperator
 * --------------------
 *      checks if provided operator is left associative; returns 1 if yes, 0 otherwise
 */
int isLeftAssocOperator(char c)
{
    if( c == '+' || c == '-' || c == '*' || c == '/' )
        return 1;
    return 0;
}

/*
 * Function:  getOpPriority
 * --------------------
 *      returns priority of provided operator
 */
int getOpPriority(char c)
{
    if( c == '+' || c == '-' )
        return 1;
    else if( c == '*' || c == '/' )
        return 2;
    else if( c == '^' )
        return 3;
    return 0;
}

/*
 * Function:  strRemoveWhitespaces
 * --------------------
 *      removes every space in string str
 */
void strRemoveWhitespaces( char *str )
{
    char *slice = str;
    for( ; *str != '\0'; str++ )                            // While pointer doesn't point to null, increment it
    {
        *slice = *str;                                      // Move value pointed by str to position pointed by slice
        if (*slice != ' ')                                  // If it's not space, increment slice
            slice++;
    }
    *slice = '\0';                                          // Add null at the end
}

/*
 * Function:  getText
 * --------------------
 *      creates string being beginning of input text containing only letters. After usage, memory used by created
 *      string should be freed
 *
 *      input: pointer to string
 *      end:   pointer to variable holding pointer of next character after created string (first non-letter)
 *
 *      returns: allocated on heap copy of string
 */
char* getText( char *input, char **end )
{
    int i;
    for( i = 0; input[i] != '\0'; i++ )                     // While we didn't hit null
        if( !isChar( input[i] ) )                           // If current letter is not char...
            break;                                          // ...end out search

    *end = input + i;                                       // Set pointer to the address where we stop
    return strndup( input, ( size_t )i );                   // Creates copy of string input of size i
}

/*
 * Function:  getTextWithNumbers
 * --------------------
 *      creates string being beginning of input text containing letters and numbers
 *      Behavior and usage is the same as in getText function
 */
char* getTextWithNumbers( char *input, char **end )
{
    int i;
    for( i = 0; input[i] != '\0'; i++ )
        if( !isChar( input[i] ) && !isDigit( input[i] ) )   // If current letter is neither char nor number...
            break;                                          // .. end our search

    *end = input + i;
    return strndup( input, ( size_t )i );
}
