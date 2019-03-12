/*
 * File: Stack.h
 * Author: Paweł Wieczorek
 * Date: 09 Nov 2018
 * Description: Header of file Stack.c
 */

#ifndef PROJEKT3_STACK_H
#define PROJEKT3_STACK_H

#include "Structures.h"

/************************************
 * Constants definitions
 ************************************/
#define STACK_SUCCESS            0                      // No error occurred
#define STACK_ERR_OOM           -201                    // Out of memory error
#define STACK_ERR_FULL          -202                    // Tried to add element to full stack
#define STACK_ERR_EMPTY         -203                    // Tried to take element from empty stack
#define STACK_ERR_UNK_TYPE      -204                    // Invalid type of element was given
#define STACK_ERR_INV_TYPE      -205                    // Element on the top of stack has invalid type

/************************************
 * Structs definitions
 ************************************/
struct Stack {
    struct Node *frames;                                // Pointer to array representing frames of the stack
    int top;                                            // Current number of elements
    int size;                                           // Maximum number of elements
};
typedef struct Stack Stack;

/************************************
 * Functions definitions
 ************************************/
int createStack(Stack **stack, int size);

int pushValueToStack( Stack *stack , enum Node_Types type, void* value);
int pushOperatorToStack( Stack *stack, char element );
int pushNumberToStack( Stack *stack, double element );
int pushFunctionToStack( Stack *stack, char *element );

int popValueFromStack( Stack *stack, enum Node_Types type, void* ret );
int popOperatorFromStack( Stack *stack, char* ret );
int popNumberFromStack( Stack *stack, double* ret );
int popFunctionFromStack( Stack *stack, char** ret );
int popStructFromStack( Stack *stack, struct Node ** ret);

void deleteStack(Stack *stack);

#endif //PROJEKT3_STACK_H
