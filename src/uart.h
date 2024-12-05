#ifndef ATMEGA_SNAKE_UART_H_
#define ATMEGA_SNAKE_UART_H_

// #define BAUD 19200
#include <avr/io.h>
// #include <util/setbaud.h>

void uart_init(void);
void uart_transmit(char data);
void uart_send_str(const char* str);
// void uart_print(const char* str);
// void uart_print_binary(uint32_t num);

#endif // ATMEGA_SNAKE_UART_H_
