/*
 * File: Structures.h
 * Author: Paweł Wieczorek
 * Date: 20 Nov 2018
 * Description: Definition of types used by calculator
 */
#ifndef PROJEKT3_STRUCTURES_H
#define PROJEKT3_STRUCTURES_H

// Enum used to distinguish type of node represented by structure
enum Node_Types {EMPTY, NUMBER, OPERATOR, BRACKET_LEFT, BRACKET_RIGHT, FUNCTION, VARIABLE, EQUALS, DELETED};

// Union used to store one of those variables in structure, depending on node type
union value {
    double number;                          // Store number in double type
    char chr;                               // One char
    char* str;                              // Pointer to string
};

// Structure and typedefs of basic node
struct Node {
    union value field;                      // Value of node
    enum Node_Types type;                   // Type of node
};
typedef struct Node RPN_Node;
typedef struct Node Infix_Node;

#endif //PROJEKT3_STRUCTURES_H
