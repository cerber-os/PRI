/*
 * File: MatrixGUI.h
 * Author: Paweł Wieczorek
 * Date: 26 Oct 2018
 * Description: Header of file MatrixGUI.c
 */

#ifndef PROJEKT2_MATRIXGUI_H
#define PROJEKT2_MATRIXGUI_H

#include "SquareMatrix.h"

/************************************
 * Target system check
 ************************************/
#ifndef __linux__
#warning "Control sequences and library for changing terminal settings used in this program will work only on Linux system"
#endif

/************************************
 * Macros definitions
 ************************************/
// Control sequences used to manipulate output
// For more control sequences see: "man console_codes"
// If _N_ is used in name, parameter with desired value must be passed to function parsing expression
#define MOVE_CURSOR_UP_N_ROWS   "\033[%dF"
#define CLEAR_CURRENT_LINE      "\033[2K"
#define REVERSE_COLOR           "\033[7m"
#define DEFAULT_DISPLAY         "\033[0m"
#define FONT_RED_COLOR          "\033[31m"

// Characters used to print table border
#define BRACKET_TOP_LEFT        "\u2554"
#define BRACKET_TOP_RIGHT       "\u2557"
#define BRACKET_BOTTOM_LEFT     "\u255A"
#define BRACKET_BOTTOM_RIGHT    "\u255D"
#define BRACKET_MIDDLE          "\u2551"

/************************************
 * Functions definitions
 ************************************/
void printMatrixAsTable( Matrix *matrix );
void reprintMatrixAsTableWithHighlight( Matrix *matrix, int highlightedRow, int highlightedColumn );
void printMatrixAsTableWithHighlight( Matrix *matrix, int highlightedRow, int highlightedColumn );
void clearLastLinePrinted( void );
void switchTerminalToNonBufferingMode( void );
void switchTerminalToDefaultMode( void );
int _safeInput( long *result );
long safeNumPrompt( char *prompt, long minValue, long maxValue );
void editMatrixPrompt( Matrix *matrix );

#endif //PROJEKT2_MATRIXGUI_H
