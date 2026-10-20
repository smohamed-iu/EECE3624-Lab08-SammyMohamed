
/*****************************************************************************
 *      File: USARTLibrary.c
 *  Lab Name: (Needed for Lab07 and Lab08)
 *    Author: Dr. Greg Nordstrom
 *   Created: 3/3/2019
 *  Modified: Sammy Mohamed
 *  Mod Date: 10/18/2026
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This is intended to be compiled as an AVR Library Project and
 * used by other programs. After compiling this code, copy the
 * libUSART1Library.a and USART1Library.h files to the project directory,
 * then add the library to the target project by R-clicking on the
 * project's Libraries directory and clicking Add Library. Also,
 * the USART1Library.h file must be #included in the project's .c file.
 *
 * NOTE: This library only works with the default USART1 port
 * on the ReadyAVR (so JP2 must have jumpers on PD3 and PD2).
 */

#include <avr/io.h>
#include "UARTLibrary.h" //goesnto main lab08.c

// Use USART1_init to initialize the USART hardware in the AVR processor.
// The function takes the following arguments:
//    - desired baud rate (standard values from 2400 to 115200),
//    - number of databits (5-8),
//    - number of stop bits (1,2),
//    - parity (0=no parity, 1=odd parity, 2=even parity).
// No error checking is performed and the function does not return a value.
//
// For example,
//     usart1_init(9600,8,1,0);
// sets the baudrate to 9600, with 8 data bits, 1 stop bit, and no parity

void uart_init(long unsigned int baudrate,
               unsigned char databits,
               unsigned char stopbits,
               unsigned char parity)
{
    /* your code here */

    // calculates baud rate value
    unsigned int baudValue = (F_OSC / (16UL * baudrate)) - 1;

    // sets baud rate with UBRR0 high and low regs
    // uses USART1 since thats what our board uses
    UBRR1H = (unsigned char)(baudValue >> 8);
    UBRR1L = (unsigned char)baudValue;

    // start with normal asynchronous mode
    UCSR1A = 0;

    // start with the settings cleared
    unsigned char settings = 0;

    // set 7 or 8 databits
    if (databits == 8)
    {
        // 8 data bits
        settings |= (1 << UCSZ11) | (1 << UCSZ10);
    }
    else if (databits == 7)
    {
        // 7 data bits
        settings |= (1 << UCSZ11);
    }

    // set parity
    if (parity == 1)
    {
        // set odd 
        settings |= (1 << UPM11) | (1 << UPM10);
    }
    else if (parity == 2)
    {
        // set even 
        settings |= (1 << UPM11);
    }

    // set stopbits
    if (stopbits == 2)
    {
        // 2 stop bits
        settings |= (1 << USBS1);
    }

    // save all  settings
    UCSR1C = settings;

    //  enable oreceiver and transmitter
    UCSR1B = (1 << RXEN1) | (1 << TXEN1);
}


// This function waits for space in the USART transmit buffer. When space
// is available, the function places an unsigned character value (passed in
// as the function's only argument) in the buffer and returns. No error
// checking is performed and the function does not return a value.

void uart_tx(unsigned char data) //send 1 char to putty
{
    /* your code here */

    // wait until the transmit buffer is ready
    while (!(UCSR1A & (1 << UDRE1)))
    {
    }

    // send character
    UDR1 = data;
}


// This function blocks until a character is received in the USART input
// buffer. Once a character arrives, it is passed back to the caller as
// an unsigned character. No error checking is performed.

unsigned char uart_rx(void)//recieve 1 char from putty
{
    /* your code here */

    // wait until char is arrives
    while (!(UCSR1A & (1 << RXC1)))
    {
    }

    // return the received char
    return UDR1;
}
