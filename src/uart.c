#include "uart.h"

// void
// uart_init() {
//     UBRR0H = UBRRH_VALUE;
//     UBRR0L = UBRRL_VALUE;
//     UCSR0B = (1 << TXEN0);                  // Enable TX
//     UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
// }

// void
// uart_transmit(char data) {
//     while (!(UCSR0A & (1 << UDRE0)))
//         ; // Wait for empty transmit buffer
//     UDR0 = data;
// }

// void
// uart_print(const char* str) {
//     while (*str) {
//         uart_transmit(*str++);
//     }
// }

// // Function to print a binary number via UART
// void
// uart_print_binary(uint32_t num) {
//     for (int i = 31; i >= 0; i--) { // Iterate through all 32 bits
//         if (num & (1UL << i)) {     // Check if the bit is set
//             uart_transmit('1');
//         } else {
//             uart_transmit('0');
//         }
//         if (i % 8 == 0 && i != 0) { // Add a space every 8 bits for readability
//             uart_transmit(' ');
//         }
//     }
//     uart_transmit('\n'); // Add newline at the end
// }

// void
// uart_init(void) {
//     // UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
//     // UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
//     // UBRR0 = F_CPU / BAUD / 16 - 1;

//     UBRR0H = UBRRH_VALUE; // Set baud rate
//     UBRR0L = UBRRL_VALUE;
//     UCSR0B = (1 << TXEN0); // Enable transmitter
//     UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
// }

// void
// uart_transmit(char data) {
//     while (!((UCSR0A >> UDRE0) & 1)) {}
//     UDR0 = data;
// }

#define BAUD_rate 9600
#define UBRR0_val F_CPU / BAUD_rate / 16 - 1

void
uart_init(void) {
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
    UBRR0 = UBRR0_val;
}

void
uart_transmit(char data) {
    while (!((UCSR0A >> UDRE0) & 1)) {}
    UDR0 = data;
}

void
uart_send_str(const char* str) {
    while (*str != '\0') {
        uart_transmit(*str);
        str++;
    }
    uart_transmit('\n');
}
