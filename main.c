#include "hw.h"
#include "core.h"
#include "cmd.h"
#include "print.h"
#include "config.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void UART_init(uint32_t baud)
{
    uint16_t ubrr = F_CPU / (baud * 16) - 1;
 
    UBRR0 = ubrr;
    UCSR0A = 0;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (1 << TXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void setup() {
    UART_init(BAUD);
    hw_setup();
    print_setup();
    core_setup();
}

ISR(USART_RX_vect)
{
    char c = UDR0;
    cmd_handle(c);
}

int main(void)
{
    setup();
    sei();

    print_str("init\n\r> ");
    while (true) {
        core_loop(10);
        _delay_us(10);
    }
    return 0;
}
