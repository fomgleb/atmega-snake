#include "avr-utils/common/real_inline.h"
#include "avr/io.h"

#define BAUD_rate 19200

REAL_INLINE void
uart_init(void) {
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
    UBRR0 = F_CPU / BAUD_rate / 16 - 1;
}

REAL_INLINE void
uart_transmit(char data) {
    while (!((UCSR0A >> UDRE0) & 1)) {}
    UDR0 = data;
}

REAL_INLINE void
uart_transmit_str(const char* str) {
    while (*str != '\0') {
        uart_transmit(*str);
        str++;
    }
}
