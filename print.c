#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>
#include <stdbool.h>

static char sendbuf[24];
static uint8_t pos_send;
static uint8_t pos_write;
static uint8_t send_len;
static bool run;

static void send_char(char c)
{
    UDR0 = c;
}

void print_str(const char *s)
{
    if (*s == 0)
        return;

    while (*s != 0 && send_len < sizeof(sendbuf)) {
        sendbuf[pos_write] = *(s++);
        pos_write = (pos_write + 1) % sizeof(sendbuf);
        send_len++;
    }
    if (!run) {
        send_char(sendbuf[pos_send]);
        pos_send = (pos_send + 1) % sizeof(sendbuf);
        send_len--;
        run = true;
    }
}

static void print_onsend()
{
    if (send_len > 0) {
        send_char(sendbuf[pos_send]);
        pos_send = (pos_send + 1) % sizeof(sendbuf);
        send_len--;
    } else {
        run = false;
    }
}

void print_setup(void)
{
    send_len = 0;
    pos_send = 0;
    pos_write = 0;
    run = false;
}

ISR(USART_TX_vect)
{
	print_onsend();
}
