
#include "cmd.h"
#include "print.h"

#include <avr/io.h>

#include <math.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

static char buf[16];
static uint8_t len;

void cmd_setup(void)
{
    buf[sizeof(buf)-1] = 0;
    len = 0;
}

void cmd_process(const char *cmd)
{
    if (cmd[0] == 'F') {
        core_finish();
    } else if (cmd[0] == 'S') {
        core_calculate_parameters();
        core_run();
    } else if (cmd[0] == 'A') {
        int ampl = -1;
        sscanf(cmd+1, "%d", &ampl);
        if (ampl < 0) {
            print_str("Bad amplitude: ");
            print_str(cmd);
            print_str("\r\n");
            return;
        }
        core_set_amplitude_symmetric(ampl);
    } else if (cmd[0] == 'B') {
        int ampl = -1;
        sscanf(cmd+1, "%d", &ampl);
        if (ampl < 0) {
            print_str("Bad amplitude: ");
            print_str(cmd);
            print_str("\r\n");
            return;
        }
        core_set_amplitude_single(ampl);
    }
}

void cmd_handle(char c)
{
    if (c == '\n' || c == '\r') {
        if (len > 0) {
            cmd_process(buf);
            memset(buf, 0, sizeof(buf));
            len = 0;
        }
    } else {
        if (len < sizeof(buf)-1)
            buf[len++] = c;
    }
}
