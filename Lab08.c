
/*****************************************************************************
 *    File: Lab08.c
 *  Lab Name: RS232 Communication Server
 *    Author: Dr. Greg Nordstrom
 *   Created: 12/30/17
 *  Mod Date: 3/28/19, 1/17/21
 *  Modified: Sammy Mohamed
 *   ModDate:10/18/2026
 *
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This program implements a communication server. Normally the server
 * simply echoes what was sent. However, there are two cases where
 * different behavior occurs:
 *    1. Pressing the ESC causes a soft reset (redisplays the title text)
 *    2. Typing the letters 'end' contiguously displays 'Bye' and halts
 *
 *****************************************************************************/

//  CPU speed: 7.3728 MHz 
#define F_CPU   7372800UL

#define CR          0x0D //from datasheet //carrrauge return/enter
#define LF          0x0A //linefeed
#define ESC         0x1B //escape button

#include <avr/io.h>
#include <util/delay.h>
#include "UARTLibrary.h"  // "" local library.

#define BAUDRATE 19200 // standard val
#define DATABITS    8 // 5-8
#define STOPBITS    1 // 1,2
#define PARITY      0 // 0=no 1=odd 2=even

/* Your code here.  */

// messages that show up in putty
const char serverMsg[] = "RS232 Communications Server";
const char statusMsg[] = "Status: Active";
const char nameMsg[]   = "Sammy Mohamed";
const char classMsg[]  = "EECE3624";
const char fallMsg[]   = "Fall 2026";
const char byeMsg[]    = "Bye";

// functions for printing the messages
void printMessage(const char *cp);
void displayStatus(void);


int main(void)
{
    /* Your code here.  */

    // holds character typed in putty
    unsigned char c;

    // keeps track of how far into "end" we are
    unsigned char count = 0;

    // make LED0 an output
    DDRA |= (1 << DDA0);

    // start LED0 off
    PORTA |= (1 << PA0);


    /* See the UARTLibrary.h for available calls.
     *      // initialize the UART
     *      uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY);
     *
     *      // transmit a value (0x41) to the comm port
     *      c = 0x41;
     *      uart_tx(c);
     *
     *      // receive a byte from the comm port
     *      c = uart_rx();
     */

    // start the uart
    uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY);

    // show the startup screen
    displayStatus();


    while(1)
    {
        /* your loop code here */

        // wait for a character from putty
        c = uart_rx(); //echoes recieved char 

        // turn LED0 on when character comes in
        PORTA &= ~(1 << PA0);

        // leave on for 75 ms
        _delay_ms(75);

        // turn LED0 back off
        PORTA |= (1 << PA0);


        // check if escape was pressed
        if (c == ESC)
        {
            // reset the end counter
            count = 0;

            // show the startup screen again
            displayStatus();

            // go back to waiting for another character
            continue;
        }


        // send the character back into putty 
        uart_tx(c);


        // if enter is pressed send linefeed to move cursor down one line
        if (c == CR) //c echoes CR
        {
            uart_tx(LF); //linefed
        }


        // check for e
        if (c == 'e')
        {
            count = 1;
        }

        // check for en
        else if ((count == 1) && (c == 'n'))
        {
            count = 2;
        }

        // check for end
        else if ((count == 2) && (c == 'd'))
        {
            count = 3;
        }

        // wrong character so start over
        else
        {
            count = 0;
        }


        // if end was actually typed
        if (count == 3)
        {
            // two blank line endings before bye
            uart_tx(CR);
            uart_tx(LF);// one line
            uart_tx(CR);
            uart_tx(LF); //two

            // print bye
            printMessage(byeMsg);

            // stay here until board gets reset
            while(1)
            {
            }
        }
    }

    return 0;
}


/*************************************************************************
 * printMessage
 *
 * prints one message one character at a time
 *************************************************************************/
void printMessage(const char *cp)
{
    // keeps track of character we are on
    unsigned char i = 0;

    // keep printing until end of the message
    while (cp[i] != '\0')
    {
        uart_tx(cp[i]);
        i++;
    }
}


/*************************************************************************
 * displayStatus
 *
 * prints the startup screen
 *************************************************************************/
void displayStatus(void)
{
    // blank line before message
    uart_tx(CR);
    uart_tx(LF);
    uart_tx(CR);
    uart_tx(LF);

    // print the server name
    printMessage(serverMsg);
    uart_tx(CR);
    uart_tx(LF);

    // print status
    printMessage(statusMsg);
    uart_tx(CR);
    uart_tx(LF);

    // print my name
    printMessage(nameMsg);
    uart_tx(CR);
    uart_tx(LF);

    // print class
    printMessage(classMsg);
    uart_tx(CR);
    uart_tx(LF);

    // print semester
    printMessage(fallMsg);

    // blank line after message
    uart_tx(CR);
    uart_tx(LF);
    uart_tx(CR);
    uart_tx(LF);
}