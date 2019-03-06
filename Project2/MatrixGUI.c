/*
 * File: MatrixGUI.c
 * Author: Paweł Wieczorek
 * Date: 26 Oct 2018
 * Description: Implementation of matrices GUI
 */

#include "MatrixGUI.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <termio.h>
#include <unistd.h>

/*
 * Function:  printMatrixAsTableWithHighlight
 * --------------------
 *      print matrix on screen with highlighted cell
 *
 *      matrix:             matrix structure which data should be printed
 *      highlightedRow:     row of cell that should be highlighted
 *      highlightedColumn:  column of cell that should be highlighted
 *
 */
void printMatrixAsTableWithHighlight( Matrix *matrix, int highlightedRow, int highlightedColumn )
{
    const int fieldWidth = 12;                              // Width of field containing value of matrix cell
    const int allFieldWidth = fieldWidth * matrix->size;    // Width of all fields

    // Print top part of opening and closing bracket - "*" instructs printf to take value from parameter passed
    printf( BRACKET_TOP_LEFT "%-*s" BRACKET_TOP_RIGHT "\n", allFieldWidth, " " );

    for( int rows = 0; rows < matrix->size; rows++ )
    {
        printf(BRACKET_MIDDLE);
        for( int cols = 0; cols < matrix->size; cols++ )
            if( rows == highlightedRow && cols == highlightedColumn )       // If this is cell we want to highlight
                printf( REVERSE_COLOR "%-*ld" DEFAULT_DISPLAY, fieldWidth, matrix->elements[rows][cols] );
            else
                printf( "%-*ld", fieldWidth, matrix->elements[rows][cols] );
        printf(BRACKET_MIDDLE "\n");
    }

    // Print bottom part of opening and closing bracket
    printf( BRACKET_BOTTOM_LEFT "%-*s" BRACKET_BOTTOM_RIGHT "\n", allFieldWidth, " " );
}

/*
 * Function:  reprintMatrixAsTableWithHighlight
 * --------------------
 *      move cursor up and overwrite already printed matrix on screen
 *
 *      matrix:             matrix structure which data should be printed
 *      highlightedRow:     row of cell that should be highlighted
 *      highlightedColumn:  column of cell that should be highlighted
 *
 */
void reprintMatrixAsTableWithHighlight( Matrix *matrix, int highlightedRow, int highlightedColumn )
{
    // Move cursor to the top of printed matrix
    printf(MOVE_CURSOR_UP_N_ROWS, matrix->size + 2);         // matrix->size + two lines used by matrix decorator

    printMatrixAsTableWithHighlight( matrix, highlightedRow, highlightedColumn );
}

/*
 * Function:  printMatrixAsTable
 * --------------------
 *      prints matrix on screen; it just calls function printMatrixAsTableWithHighlight with arguments -1, -1 to
 *      prevent it form highlighting and cell
 *
 *      matrix: matrix structure which elements should be printed
 *
 */
void printMatrixAsTable( Matrix *matrix )
{
    printMatrixAsTableWithHighlight( matrix, -1, -1 );       // -1 prevents highlighting any cell
}

/*
 * Function:  clearLastLinePrinted
 * --------------------
 *      clears last line using escape sequences
 *
 */
void clearLastLinePrinted( void )
{
    printf(MOVE_CURSOR_UP_N_ROWS CLEAR_CURRENT_LINE, 1);
}

/*
 * Function:  switchTerminalToNonBufferingMode
 * --------------------
 *      by default linux terminal buffers input until enter key was pressed by user. This function instruct terminal to
 *      provide input immediately after user action
 *
 */
void switchTerminalToNonBufferingMode( void )
{
    struct termios settings;

    tcgetattr( STDIN_FILENO, &settings);                    // Get current terminal settings
    settings.c_lflag &= ~(ICANON | ECHO);                   // Clear flag ICANON and ECHO - disable buffering and echoing
    tcsetattr( STDIN_FILENO, TCSANOW, &settings);           // Set new settings
}

/*
 * Function:  switchTerminalToDefaultMode
 * --------------------
 *      sets termianal back to default settings; opposite to switchTerminalToNonBufferMode function
 *
 */
void switchTerminalToDefaultMode( void )
{
    struct termios settings;

    tcgetattr( STDIN_FILENO, &settings);                    // Get current terminal settings
    settings.c_lflag |= (ICANON | ECHO);                    // Set flag ICANON and ECHO - enable buffering and echoing
    tcsetattr( STDIN_FILENO, TCSANOW, &settings);           // Set new settings
}

/*
 * Function <private>:  _safeInput
 * --------------------
 *      takes one line from stdin, converts to signed long integer and in case no error occurred set result pointer
 *      to read value;  this function was created to be use by safeNumPrompt and shouldn't be directly call from outside
 *      this library
 *
 *      result: pointer to variable of type long int, where read value should be stored
 *
 *      returns: 0 on success, -1 on reading error, -2 on out of range, -3 and -4 on strtol converting error
 *
 */
int _safeInput( long *result )
{
    const int MAX_INPUT_BUFFER = 100;                       // Max line length
    char stdin_buffer[MAX_INPUT_BUFFER + 1];                // Buffer where line read from input will be stored
    char *lastChar = NULL;                                  // Pointer set by strtol, used to check if characters were read
    long enteredNumber = 0;
    errno = 0;                                              // Reset error flag

    if( !fgets(stdin_buffer, MAX_INPUT_BUFFER, stdin) )     // Reading error
        return -1;

    enteredNumber = strtol(stdin_buffer, &lastChar, 10);    // Convert read string to long integer
    if( lastChar == stdin_buffer )                          // strtol stopped at the beginning - no characters were read
        return -3;
    if( *lastChar != 0 && *lastChar != '\n')                // Last line is nether NULL nor new line - not all input was processed
        return -4;
    if( errno == ERANGE )                                   // Out of range
        return -2;

    *result = enteredNumber;
    return 0;
}

/*
 * Function:  safeNumPrompt
 * --------------------
 *      prints provided string and reads input until valid integer in range <minValue, maxValue> was provided by user
 *
 *      prompt: string containing message for user
 *      minValue: minimal value accepted by function
 *      maxValue: maximum value accepted by function
 *
 *      returns: number provided by user
 *
 */
long safeNumPrompt( char *prompt, long minValue, long maxValue )
{
    int errorCode = 0;                                      // Variable for storing error codes returned by functions called
    long enteredNumber = 0;

    printf("%s", prompt);

    do {                                                    // While input isn't correct
        errorCode = _safeInput( &enteredNumber );           // Get long int
        if( enteredNumber < minValue)                       // Too small
            errorCode = -5;
        if( enteredNumber > maxValue)                       // Too big
            errorCode = -2;
        switch( errorCode )                                 // Display error for each error code
        {
            case 0:
                break;
            case -2:
                clearLastLinePrinted();                     // Overwrite line with prompt
                printf(FONT_RED_COLOR "%s" DEFAULT_DISPLAY ". Try again:", "Provided number is too big");
                break;
            case -5:
                clearLastLinePrinted();
                printf(FONT_RED_COLOR "%s" DEFAULT_DISPLAY ". Try again:", "Provided number is too small");
                break;
            default:
                clearLastLinePrinted();
                printf(FONT_RED_COLOR "%s" DEFAULT_DISPLAY ". Try again:", "Only numbers are accepted");
                break;

        }
    } while( errorCode != 0 );

    clearLastLinePrinted();                                 // Clear line containing prompt
    return enteredNumber;
}

/*
 * Function:  editMatrixPrompt
 * --------------------
 *      display interactive matrix form, get input from user and insert it in provided matrix structure
 *
 *      matrix:             matrix structure which data should be printed
 *
 */
void editMatrixPrompt( Matrix *matrix )
{
    int selectedRow = 0;                                    // Currently highlighted row in matrix
    int selectedCol = 0;                                    // the same as above, but for column
    int charFromInput = 0;                                  // Character read from input
    long valueRead = 0;                                     // Long integer read from prompt
    const int promptBufferLength = 100;
    char prompt[promptBufferLength];                                       // Buffer used to create text for prompt message

    printMatrixAsTableWithHighlight( matrix, selectedRow, selectedCol );
    puts( "Use arrows to highlight cell. Press enter to change its value. Press q to continue" );
    printf(MOVE_CURSOR_UP_N_ROWS, 1);

    switchTerminalToNonBufferingMode();                     // Disable buffering - we want direct input
    while(charFromInput != 'q')                             // While user haven't pressed (q)uit key
    {
        charFromInput = getchar();                          // Get character
        if( charFromInput == '\033' ) {                     // Check if it is escape sequence starter
            if (getchar() == '[')                           // Take next character and check if its next char of escape sequence
                switch (getchar()) {                        // Finally get last char and check which arrow it is
                    case 'A':                               // Up arrow
                        if (selectedRow == 0)
                            selectedRow = matrix->size - 1; // We are at the beginning of rows - jump to the last
                        else
                            selectedRow--;
                        break;                              // Down arrow
                    case 'B':
                        if (selectedRow == matrix->size - 1)
                            selectedRow = 0;
                        else
                            selectedRow++;
                        break;
                    case 'C':                               // Right arrow
                        if (selectedCol == matrix->size - 1)
                            selectedCol = 0;
                        else
                            selectedCol++;
                        break;
                    case 'D':                               // Left arrow
                        if (selectedCol == 0)
                            selectedCol = matrix->size - 1;
                        else
                            selectedCol--;
                        break;
                    default:
                        break;
                }
        }else if( charFromInput == '\n' )                   // If it is new line (enter)
        {
            switchTerminalToDefaultMode();                  // Switch terminal back to buffering mode - required for scanf used later

            // Prepare prompt message of form: "Value for field [x,y]: "
            snprintf( prompt, promptBufferLength, CLEAR_CURRENT_LINE "Value for field [%d,%d]: ", selectedRow + 1, selectedCol + 1 );
            valueRead = safeNumPrompt( prompt, MIN_MATRIX_FIELD_VALUE, MAX_MATRIX_FIELD_VALUE ); // Read value from user
            switchTerminalToNonBufferingMode();

            matrix->elements[selectedRow][selectedCol] = valueRead; // Save value to matrix

            puts( "Use arrows to highlight cell. Press enter to change its value. Press q to continue" );
            printf( MOVE_CURSOR_UP_N_ROWS, 1 );              // Display instruction again and set cursor on it
        }
        reprintMatrixAsTableWithHighlight( matrix, selectedRow, selectedCol ); // Reprint matrix table with new cell highlighted
    }

    printf( CLEAR_CURRENT_LINE );                            // Clear currently active line
    switchTerminalToDefaultMode();                           // Switch back to default console mode used by the rest of program
}