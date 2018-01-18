#ifndef UART_H
#define UART_H

#include <stdio.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif

FILE uart_output;
FILE uart_input;

void uart_init();
void uart_set_io_streams();
void uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);

#endif