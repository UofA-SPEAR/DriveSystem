#ifndef UART_H
#define UART_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#include <util/setbaud.h>
#include <stdio.h>
#include <avr/io.h>

FILE uart_output;
FILE uart_input;

void uart_init();
void uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);

#endif