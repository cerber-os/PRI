/*
 * File: Stack.c
 * Author: Paweł Wieczorek
 * Date: 21 Nov 2018
 * Description: Stack implementation
 */

#include <malloc.h>
#include "Stack.h"
#include "Structures.h"

/*
 * Function:  createStack
 * --------------------
 *      creates stack of provided size
 *
 *      stack: pointer to variable holding pointer to Stack structure
 *      size:  maximum size of stack
 *
 *      returns: 0 on success, STACK_ERR_OOM on out of memory
 */
int createStack( Stack **stack, int size )
{
    Stack *ptr = malloc( sizeof( Stack ) );                                 // Allocate memory for structure
    if( ptr == NULL )                                                       // We are out of memory
        return STACK_ERR_OOM;

    struct Node *frames = calloc( ( size_t )size, sizeof( struct Node ) );  // Allocate memory for stack frames
    if( frames == NULL )                                                    // We are out of memory
        return STACK_ERR_OOM;

    ptr->frames = frames;
    ptr->top = 0;                                                           // Set current element to 0
    ptr->size = size;
    *stack = ptr;                                                           // Save pointer to stack

    return STACK_SUCCESS;
}

/*
 * Function:  pushToStack
 * --------------------
 *      pushes element to stack
 *
 *      stack: pointer to Stack structure
 *      type:  type of element being pushed
 *      value: value of element
 *
 *      returns: STACK_SUCCESS          on success, 
 *               STACK_ERR_FULL         when stack is already full,
 *               STACK_ERR_UNK_TYPE     when element type is invalid
 */
int pushValueToStack( Stack *stack , enum Node_Types type, void* value)
{
    if( stack->top + 1 == stack->size )                                     // Check if there's free space on stack
        return STACK_ERR_FULL;

    stack->frames[stack->top].type = type;
    switch(type)                                                            // Depending on node, cast to...
    {                                                                       // ...different type
        case NUMBER:
            stack->frames[stack->top].field.number = *( ( double* ) value );
            break;
        case OPERATOR:
            stack->frames[stack->top].field.chr = *( ( char* ) value );
            break;
        case FUNCTION:
            stack->frames[stack->top].field.str = ( ( char* ) value );
            break;
        default:
            return STACK_ERR_UNK_TYPE;
    }

    stack->top++;                                                           // Increase stack top
    return STACK_SUCCESS;
}

/*
 * Function:  pushOperatorToStack
 * --------------------
 *      pushes element of type char to stack. Used to store operators and brackets on stack
 *
 *      stack: pointer to Stack structure
 *      element: element of type char to be pushed
 *
 *      returns: STACK_SUCCESS on success, the same errors as function pushValueToStack
 */
int pushOperatorToStack( Stack *stack, char element )
{
    return pushValueToStack( stack, OPERATOR, ( void *) &element );
}

/*
 * Function:  pushNumberToStack
 * --------------------
 *      pushes element of type int to stack. Used to store numbers on stack
 *
 *      stack: pointer to Stack structure
 *      element: element of type int to be pushed
 *
 *      returns: STACK_SUCCESS on success, the same errors as function pushValueToStack
 */

int pushNumberToStack( Stack *stack, double element )
{
    return pushValueToStack( stack, NUMBER, ( void *) &element );
}

/*
 * Function:  pushFunctionToStack
 * --------------------
 *      pushes element of type char* to stack. Used to store functions names on stack
 *
 *      stack: pointer to Stack structure
 *      element: element of type char* to be pushed
 *
 *      returns: STACK_SUCCESS on success, the same errors as function pushValueToStack
 */

int pushFunctionToStack( Stack *stack, char *element )
{
    return pushValueToStack( stack, FUNCTION, ( void *) element );
}

/*
 * Function:  popValueFromStack
 * --------------------
 *      pops element from stack
 *
 *      stack:  pointer to Stack structure
 *      type:   type of element to be taken from stack
 *      ret:    pointer to variable where value from stack should be stored
 *
 *      returns: STACK_SUCCESS          on success, 
 *               STACK_ERR_EMPTY        when stack is empty,
 *               STACK_ERR_INV_TYPE     when element on top of the stack has different type,
 *               STACK_ERR_UNK_TYPE     when element type is invalid
 */
int popValueFromStack( Stack *stack, enum Node_Types type, void* ret )
{
    if( stack->top == 0 )                                           // Check if there is anything on stack
        return STACK_ERR_EMPTY;

    if( stack->frames[stack->top - 1].type != type )                // Check if type of top element is correct
        return STACK_ERR_INV_TYPE;

    switch(type)                                                    // Cast value depending on top element type
    {
        case NUMBER:
            *( ( double *)ret) = stack->frames[stack->top - 1].field.number;
            break;
        case OPERATOR:
            *( ( char *)ret) = stack->frames[stack->top - 1].field.chr;
            break;
        case FUNCTION:
            *( ( char **)ret ) = stack->frames[stack->top - 1].field.str;
            break;
        default:
            return STACK_ERR_UNK_TYPE;
    }

    stack->top--;                                                   // Decrease stack
    return STACK_SUCCESS;
}

/*
 * Function:  popOperatorFromStack
 * --------------------
 *      pops element of type char from top of the stack. Used to store operators and brackets on stack
 *
 *      stack:      pointer to Stack structure
 *      ret:        pointer to variable in which value from top of the stack should be saved
 *
 *      returns: STACK_SUCCESS on success, the same errors as function popValueFromStack
 */
int popOperatorFromStack( Stack *stack, char* ret )
{
    return popValueFromStack( stack, OPERATOR, ret );               // Get element from stack
}

/*
 * Function:  popNumberFromStack
 * --------------------
 *      pops element of type double from top of the stack. Used to store numbers on stack
 *
 *      stack:      pointer to Stack structure
 *      ret:        pointer to variable in which value from top of the stack should be saved
 *
 *      returns: STACK_SUCCESS on success, the same errors as function popValueFromStack
 */
int popNumberFromStack( Stack *stack, double* ret )
{
    return popValueFromStack( stack, NUMBER, ret );                 // Get element from stack
}

/*
 * Function:  popFunctionFromStack
 * --------------------
 *      pops element of type char* from top of the stack. Used to store functions names on stack
 *
 *      stack:      pointer to Stack structure
 *      ret:        pointer to variable in which value from top of the stack should be saved
 *
 *      returns: STACK_SUCCESS on success, the same errors as function popValueFromStack
 */
int popFunctionFromStack( Stack *stack, char** ret )
{
    return popValueFromStack( stack, FUNCTION, ret );               // Get element from stack
}

/*
 * Function:  popStructFromStack
 * --------------------
 *      return pointer to structure Node being on top of the stack
 *
 *      stack:      pointer to Stack structure
 *      ret:        pointer to variable in which structure from top of the stack should be saved
 *
 *      returns: STACK_SUCCESS          on success,
 *               STACK_ERR_EMPTY        when stack is already empty
 */
int popStructFromStack( Stack *stack, struct Node ** ret)
{
    if( stack->top == 0 )                                           // Check if there's anything on stack
        return STACK_ERR_EMPTY;

    *ret = &( stack->frames[stack->top - 1] );                      // Return pointer to top structure

    stack->top--;                                                   // Decrease stack
    return STACK_SUCCESS;
}

/*
 * Function:  deleteStack
 * --------------------
 *      deletes Stack structure and its elements
 *
 *      stack: pointer to Stack structure
 */
void deleteStack(Stack *stack)
{
    if(stack == NULL)                                               // Check if stack isn't null
        return;

    free(stack->frames);                                            // Free stack elements (free checks if it's not null)
    free(stack);                                                    // Finally free hole structure
}
