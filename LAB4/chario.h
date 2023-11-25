#ifndef _CHARIO_H_
#define _CHARIO_H_

/* place character-I/O function prototypes here */

/**
 * Prints a single character to the JTAG UART console.
 * @param inputChar The character to print.
 */
void PrintChar(unsigned int inputChar);

/**
 * Prints a null-terminated string to the JTAG UART console.
 * @param inputString Pointer to the string to be printed.
 */
void PrintString(char *inputString);

/**
 * Prints a single hexadecimal digit.
 * @param inputHex The hexadecimal digit (0-15) to print.
 */
void PrintHexDigit(unsigned int inputHex);

/**
 * Reads a character from the JTAG UART.
 * @return The character read from the JTAG UART.
 */
unsigned int GetChar(void);

#endif /* _CHARIO_H_ */
