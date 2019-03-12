/*
 * File: Strings.h
 * Author: Paweł Wieczorek
 * Date: 09 Nov 2018
 * Description: Header of file Strings.c
 */

#ifndef PROJEKT3_STRINGS_H
#define PROJEKT3_STRINGS_H

/************************************
 * Functions definitions
 ************************************/
int isChar( char c );
int isDigit( char c );
int isOperator( char c );
int isEquation( char c );

int isRightAssocOperator(char c);
int isLeftAssocOperator(char c);
int getOpPriority(char c);

void strRemoveWhitespaces(char *str);
char* getText( char *input, char **end );
char* getTextWithNumbers( char *input, char **end);

#endif //PROJEKT3_STRINGS_H
