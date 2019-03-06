/*
 * File: DecToBinConverter.c
 * Author: Paweł Wieczorek
 * Date: 11 Oct 2018
 * Description: Simple program converting provided number to string in binary numeral system
 */

#include <stdio.h>
#include <limits.h>

#define MAX_OUTPUT_SIZE 100
#define BITS_IN_BYTE    8

// Special strings used to change color of text in UNIX-compatible terminal
#define COLOR_DEFAULT   "\x1B[0m"
#define COLOR_RED       "\x1B[31m"

/*
 * Function:  welcomeMessage
 * --------------------
 *      prints initial message and program's usage instructions on output
 */
void welcomeMessage( void )
{
    puts( COLOR_DEFAULT "Decimal to Binary integers converter" );   // Set default color of text - in case other program changed it
    puts( "Please, provide positive number in decimal numeral system" );
}


/*
 * Function:  decToBinStr
 * --------------------
 *      converts provided number to string in binary numeral system
 *
 *      number: number in decimal numeral system to convert
 *      output: pointer to char array in which output will be stored
 *
 */
void decToBinStr( unsigned long number, char* output )
{
    unsigned long numberSizeInBits = sizeof(number) * BITS_IN_BYTE;
    unsigned long mask = 1ul << (numberSizeInBits - 1);              // Mask used to check if first bit is set
    unsigned long firstBit;
    char zerosAreImportant = 0;                                      // Flag used to prevent printing unnecessary zeros

    for( unsigned long i = 0; i < numberSizeInBits; i++ )
    {
        firstBit = number << i;                                      // Make i-th bit first
        if( firstBit & mask )                                        // Check if bit is set
        {
            *output = '1';
            ++output;
            zerosAreImportant = 1;                                   // Start inserting zeros
        } else if( zerosAreImportant || i == numberSizeInBits - 1 )  // Check if flag is set or it's last digit
        {
            *output = '0';
            ++output;
        }
    }
    *output = '\0';                                                  // Append null at the end of string
}


/*
 * Function:  main
 * --------------------
 *      main program logic
 *
 *      returns: return one if error occurred
 *
 */
int main( void ) {
    int argumentsFilled;                                     // Value returned by scanf, used to check if input was valid
    unsigned long input;
    char output[MAX_OUTPUT_SIZE];

    welcomeMessage();
    while( 1 )
    {
        printf( "> " );
        argumentsFilled = scanf( "%lu", &input );

        // Input error handling
        if( argumentsFilled != 1 )                           // Scanf was unable to read one number
        {
            puts( COLOR_RED "An error occurred while reading input!" );
            puts( "Please provide positive integer." COLOR_DEFAULT );
            return 1;
        }else if( input == ULONG_MAX )                       // Provided input is larger than maximum acceptable value
        {
            puts( COLOR_RED "Provided number is too big" );
            printf( "Maximum value is %lu\n" COLOR_DEFAULT, ULONG_MAX - 1 );
            return 1;
        }

        decToBinStr( input, output );
        puts( output );
    }
}
