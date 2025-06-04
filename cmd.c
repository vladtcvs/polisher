
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
        if (!core_is_running()) {
            print_str("Not running\n");
            return;
        }
        core_finish();
    } else if (cmd[0] == 'S') {
        if (core_is_running()) {
            print_str("Already running\n");
            return;
        }
        core_calculate_parameters();
        core_run();
    } else if (cmd[0] == 'A') {
        if (core_is_running()) {
            print_str("Stop first\n");
            return;
        }
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
        if (core_is_running()) {
            print_str("Stop first\n");
            return;
        }
        int ampl = -1;
        sscanf(cmd+1, "%d", &ampl);
        if (ampl < 0) {
            print_str("Bad amplitude: ");
            print_str(cmd);
            print_str("\r\n");
            return;
        }
        core_set_amplitude_single(ampl);
    } else if (cmd[0] == 'X') {
        if (core_is_running()) {
            print_str("Stop first\n");
            return;
        }
        int speed = -1;
        sscanf(cmd+1, "%d", &speed);
        if (speed < 0) {
            print_str("Bad speed: ");
            print_str(cmd);
            print_str("\r\n");
            return;
        }
        core_set_x_speed(speed);
    } else if (cmd[0] == 'P') {
        char buf[16] = {0};
        int ap;
        int am;
        core_get_ampl(&ap, &am);
        snprintf(buf, 15, "A=%i %i\n", ap, am);
        print_str(buf);
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
