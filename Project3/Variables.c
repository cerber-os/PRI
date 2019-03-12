/*
 * File: Variables.c
 * Author: Paweł Wieczorek
 * Date: 12 Nov 2018
 * Description: Implementation of variables used in calculator
 */
#include <string.h>
#include <malloc.h>

#include "Variables.h"

/*
 * Function: findVariableByName
 * --------------------
 *      searches for variable with specified name
 *
 *      name: name of variable
 *
 *      returns: VAR_ERR_NOT_FOUND if variable was not found, index of variable otherwise
 *
 */
int findVariableByName( char *name )
{
    for( int i = 0; i < MAX_NUMBER_OF_VARS; i++ )                                       // For each variable in array
        if( calcVariables[i].name != NULL && !strcmp( calcVariables[i].name, name ) )   // Check if its name is set and
            return i;                                                                   // equals the searched one
    return VAR_ERR_NOT_FOUND;
}

/*
 * Function: idxFirstFreeVariable
 * --------------------
 *      returns index of first free Variable structure
 *
 *      returns: VAR_ERR_NO_FREE_SPACE if there's no free space, index of variable otherwise
 *
 */
int idxFirstFreeVariable( void )
{
    for( int i = 0; i < MAX_NUMBER_OF_VARS; i++ )
        if( calcVariables[i].name == NULL )                                 // If it's name is not set
            return i;                                                       // it is free - return it
    return VAR_ERR_NO_FREE_SPACE;
}

/*
 * Function: setVariable
 * --------------------
 *      sets value of variable specified by name
 *
 *      name: name of variable
 *      value: value to be set
 *
 *      returns: 0 on success,
 *               VAR_ERR_NOT_FOUND      if variable was not found,
 *               VAR_ERR_NO_FREE_SPACE  if there's no free space for new variable
 *
 */
int setVariable( char *name, double value )
{
    int idx = findVariableByName( name );                       // Search for variable
    if( idx == VAR_ERR_NOT_FOUND )                              // If it doesn't exist, create it
    {
        idx = idxFirstFreeVariable();                           // Find space for variable
        if( idx == VAR_ERR_NO_FREE_SPACE )                      // There's no free space
            return VAR_ERR_NO_FREE_SPACE;

        calcVariables[idx].name = strdup( name );               // Save copy of name
    }

    calcVariables[idx].value = value;                           // Set the value
    return VAR_SUCCESS;
}

/*
 * Function: getVariable
 * --------------------
 *      gets value of variable specified by name
 *
 *      name: name of variable
 *      ret: pointer to variable where value should be stored
 *
 *      returns: 0 on success,
 *               VAR_ERR_NOT_FOUND      if variable doesn't exist
 *
 */
int getVariableValue( char *name, double *ret )
{
    int idx = findVariableByName( name );                       // Search for variable
    if( idx == VAR_ERR_NOT_FOUND )                              // it doesn't exist
        return VAR_ERR_NOT_FOUND;

    *ret = calcVariables[idx].value;                            // Save value of it
    return VAR_SUCCESS;
}

/*
 * Function: deleteVariable
 * --------------------
 *      frees space used by variables names
 *
 */
void deleteVariables( void )
{
    for( int i = 0; i < MAX_NUMBER_OF_VARS; i++ )
        free( calcVariables[i].name );
}

