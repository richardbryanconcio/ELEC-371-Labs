/* for standalone testing of this file by itself using the simulator,
   keep the following line, but for in-lab activity with the Monitor Program
   to have a multi-file project, comment out the following line */

//#define TEST_CHARIO


/* no #include statements should be required, as the character I/O functions
   do not rely on any other code or definitions (the .h file for these
   functions would be included in _other_ .c files) */


/* because all character-I/O code is in this file, the #define statements
   for the JTAG UART pointers can be placed here; they should not be needed
   in any other file */

#define JTAG_UART_DATA     (volatile unsigned int *) 0x10001000
#define JTAG_UART_STATUS   (volatile unsigned int *) 0x10001004



/* place the full function definitions for the character-I/O routines here */
void PrintChar(unsigned int inputChar){
   unsigned int wstatus;
    
	do {
      wstatus = *JTAG_UART_STATUS;
      wstatus = wstatus & 0xFFFF0000;	
   } while (wstatus == 0);
    
	*JTAG_UART_DATA = inputChar; // Write to JTAG UART console
}

void PrintString(char *inputString) {
   char selectChar; // Variable to hold current char selected from inputString

   while(1) {
      selectChar = *inputString; // Read pointer to string
        
		if (selectChar == '\0') { // Empty Char - Break Loop
         break;
      } else {
         PrintChar(selectChar); // Call to PrintChar
         inputString = inputString + 1; // Iterate pointer to next char
      }
   }
}

void PrintHexDigit(unsigned int inputHex) {
   unsigned int charToPrint;
	// Identifiy which value it is, like if it's 0-9 or A-F
	if (inputHex >= 10) {
      charToPrint = inputHex - 10 + 'A';
   } else {
      charToPrint = inputHex + '0';
   }
   PrintChar((int)charToPrint); // Call to PrintChar
}

unsigned int GetChar(void) {
	unsigned int data, isCharRecieved;
	
	do {
	   data = *JTAG_UART_DATA;
		isCharRecieved = data & 0x8000;
	} while (isCharRecieved == 0);
	
	data = data & 0xFF;
	return data;
}




#ifdef TEST_CHARIO

/* this portion is conditionally compiled based on whether or not
   the symbol exists; it is only for standalone testing of the routines
   using the simulator; there is a main() routine in lab4.c, so
   for the in-lab activity, the following code would conflict with it */

int main (void)
{
   /* place calls here to the various character-I/O routines
      to test their behavior, e.g., PrintString("hello\n");  */
   
   PrintChar('I');
   PrintChar('\n');
   PrintString("I love cats!\n");
   for (int i = 0; i < 16; i++) {
      PrintHexDigit(i);
      PrintChar('\n');
   }


   return 0;
} 

#endif /* TEST_CHARIO */