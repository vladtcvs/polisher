#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define BUFLEN 32
#define NUMBUF 4

static char sendbuf[NUMBUF][BUFLEN];
static uint8_t length[NUMBUF];
static uint8_t buf_send;
static uint8_t buf_write;
static uint8_t buf_used;

static uint8_t pos_send;

static bool run;

static void send_char(char c)
{
    UDR0 = c;
}

static int get_next_char(void)
{
    while (buf_used > 0)
    {
        if (pos_send == length[buf_send]) {
            buf_send = (buf_send + 1) % NUMBUF;
            buf_used--;
            pos_send = 0;
            continue;
        }

        int c = sendbuf[buf_send][pos_send++];
        if (pos_send == length[buf_send]) {
            buf_send = (buf_send + 1) % NUMBUF;
            buf_used--;
            pos_send = 0;
        }
        return c;
    }
    return -1;
}

void print_str(const char *s)
{
    if (*s == 0)
        return;

    if (buf_used == NUMBUF)
        return;

    size_t len = strlen(s);
    if (len > BUFLEN)
        len = BUFLEN;
    memcpy(sendbuf[buf_write], s, len);
    length[buf_write] = len;
    buf_used++;
    buf_write = (buf_write + 1) % NUMBUF;

    if (!run) {
        int c = get_next_char();
        if (c != -1) {
            run = true;
            send_char(c);
        }
    }
}

static void print_onsend(void)
{
    int c = get_next_char();
    if (c != -1) {
        send_char(c);
    } else {
        run = false;
    }
}

void print_setup(void)
{
    pos_send = 0;
    buf_send = 0;
    buf_write = 0;
    buf_used = 0;
    run = false;
}

ISR(USART_TX_vect)
{
    print_onsend();
}
