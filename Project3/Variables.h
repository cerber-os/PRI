/*
 * File: Variables.h
 * Author: Paweł Wieczorek
 * Date: 12 Nov 2018
 * Description: Header of file Variables.c
 */

#ifndef PROJEKT3_VARIABLES_H
#define PROJEKT3_VARIABLES_H

/**********************************
 * Constants definitions
 *********************************/
#define MAX_NUMBER_OF_VARS          100                     // Maximum number of variables that could be stored

#define VAR_SUCCESS                  0                      // No error occurred
#define VAR_ERR_NOT_FOUND           -100                    // Provided variable was not found
#define VAR_ERR_NO_FREE_SPACE       -102                    // There's no free space left for variable

/**********************************
 * Structure definition
 *********************************/
struct Variable {
    char *name;                                             // Pointer to string containing name of variable
    double value;                                           // Value of variable
};
typedef struct Variable Variable;

/**********************************
 * Global variable definition
 *********************************/
/*
 *  calcVariable is global array of "Variable" structures. Initially it will be filled with zeros, because:
 *          -> (C specification) it will be stored in .bss section of ELF file
 *          -> (ELF specification) the system initializes the data in this section with zeros when the program starts
 *  Therefore, it it perfectly safe to check the existence of variable, by checking if pointer to its name
 *  is different than 0 (NULL).
 */
Variable calcVariables[MAX_NUMBER_OF_VARS + 1];

/**********************************
 * Functions declarations
 *********************************/
int findVariableByName( char *name );
int idxFirstFreeVariable( void );
int setVariable( char *name, double value );
int getVariableValue( char *name, double *ret );
void deleteVariables( void );

#endif //PROJEKT3_VARIABLES_H
