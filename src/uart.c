#include "uart.h"

void uart_init(void) {
    //UART_PORT.DIRSET = UART_TX_bm;
    //UART_PORT.DIRCLR = UART_RX_bm;
    PORTB.DIRSET = PIN2_bm;

    USART0.BAUD  = 1389;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;

    while (!(USART0.STATUS & USART_DREIF_bm)) {}
    USART0.TXDATAL = '>';
    while (!(USART0.STATUS & USART_DREIF_bm)) {}
    USART0.TXDATAL = '\n';
}

uint8_t  uart_available(void) {
    return (USART0.STATUS & USART_RXCIF_bm) != 0;
}

uint8_t uart_read(void) {
    while (!uart_available()) ;
    return USART0.RXDATAL;
}

void uart_write(uint8_t b) {
    while (!(USART0.STATUS & USART_DREIF_bm)) ;
    USART0.TXDATAL = b;
}

void uart_write_str(const char s) {
    //while (s) uart_write((uint8_t)*s++);
}