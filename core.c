#include <stdbool.h>

#include "config.h"
#include "hw.h"
#include "print.h"
#include "core.h"

#define max(a,b) ((a) > (b) ? (a) : (b))

// 3 prime numbers to prevent astigmatism
static int X_T      = 11; // X carriage period proportional
static int MIRROR_T = 37; // Mirror rotation period proportional
static int TABLE_T  = 47; // Table rotation period proportional

// Parameters
static float X_SPEED = 30.0;   // X speed in mm/s
static float X_AMPL_1 = 0.0;   // Amplitude of X movements in mm
static float X_AMPL_2 = 0.0;   // Amplitude of X movements in mm

// Calculated parameters
static uint32_t table_delay_us;
static uint32_t mirror_delay_us;
static int32_t x_steps_1;
static int32_t x_steps_2;
static uint32_t x_delay_us;

// State
int32_t x_steps; // Current position
int x_dir;   // Current direction of move

uint32_t mirror_timer; // Timer for mirror stepper
uint32_t table_timer;  // Timer for rotary table stepper
uint32_t x_timer;      // Timer for x carriage

enum core_state_e
{
    CORE_STATE_INIT = 0,
    CORE_STATE_IDLE,
    CORE_STATE_RUN,
    CORE_STATE_FINISHING,
    CORE_STATE_STOP,
} state;

// Config functiions
void core_calculate_parameters(void)
{
    // full x path
    float X_path = 2 * (X_AMPL_1 + X_AMPL_2);
    float X_period = X_path / X_SPEED;
    
    // coefficient of proportion
    float k = X_period / X_T;

    // Calculate X carriage delay between steps
    float x_steps_per_mm = (float)X_STEPPER_STEPS * X_STEPPER_MICRO / (X_GEAR_NTEETH * X_GEAR_TOOTH);
    x_steps_1 = max(X_AMPL_1 * x_steps_per_mm, 1);
    x_steps_2 = max(X_AMPL_2 * x_steps_per_mm, 1);
    x_delay_us = 1e6 / (X_SPEED * x_steps_per_mm);

    // Calculate rotary table delay between steps
    float table_period = TABLE_T * k;
    float table_steps_per_rot = (float)TABLE_STEPPER_STEPS * TABLE_STEPPER_MICRO * TABLE_REDUCER_BIG / TABLE_REDUCER_SMALL;
    table_delay_us = max(1e6 * table_period / table_steps_per_rot, 1);
    table_timer = 0;
    hw_table_set_dir(TABLE_DIR);

    // Calculate mirror rotator delay between steps
    float mirror_period = MIRROR_T * k;
    float mirror_steps_per_rot = (float)MIRROR_STEPPER_STEPS * MIRROR_STEPPER_MICRO * MIRROR_REDUCER_BIG / MIRROR_REDUCER_SMALL;
    mirror_delay_us = max(1e6 * mirror_period / mirror_steps_per_rot, 1);
    mirror_timer = 0;
    hw_mirror_set_dir(MIRROR_DIR);
}

void core_set_amplitude_1(float ampl)
{
    X_AMPL_1 = ampl;
}

void core_set_amplitude_2(float ampl)
{
    X_AMPL_2 = ampl;
}

void core_set_amplitude_symmetric(float ampl)
{
    core_set_amplitude_1(ampl);
    core_set_amplitude_2(ampl);
}

void core_set_amplitude_single(float ampl)
{
    core_set_amplitude_1(ampl);
    core_set_amplitude_2(0);
}

void core_set_x_speed(float speed)
{
    X_SPEED = speed;
}

// State functions
void core_setup(void)
{
    x_steps = 0;
    X_AMPL_1 = 0;
    X_AMPL_2 = 0;
    core_calculate_parameters();
    hw_x_set_dir(1);
    x_dir = 1;
    x_timer = 0;
    state = CORE_STATE_INIT;
}

void core_run(void)
{
    if (state == CORE_STATE_RUN || state == CORE_STATE_FINISHING)
        return;
    print_str("run\r\n");
    hw_enable(true);
    state = CORE_STATE_RUN;
    x_dir = 1;
    hw_x_set_dir(1);
}

void core_stop(void)
{
    state = CORE_STATE_STOP;
    hw_enable(false);
}

void core_finish(void)
{
    if (state != CORE_STATE_RUN)
        return;
    if (x_steps != 0) {
        print_str("finishing\r\n");
        state = CORE_STATE_FINISHING;
    } else {
        print_str("finished\r\n");
        state = CORE_STATE_IDLE;
        hw_enable(false);
    }
}

// Processing functions
void core_make_x_step(void)
{
    if (state == CORE_STATE_IDLE || state == CORE_STATE_INIT || state == CORE_STATE_STOP)
        return;

    if (state == CORE_STATE_FINISHING && x_steps == 0) {
        print_str("finished\r\n");
        state = CORE_STATE_IDLE;
        hw_enable(false);
        return;
    }

    if (x_steps > 0 || x_steps == 0 && x_dir > 0)
    {
        // Positive side
        if (x_dir == 1)
        {
            if (x_steps >= x_steps_1)
            {
                x_dir = -1;
                hw_x_set_dir(-1);
            }
        }
    }
    else
    {
        // Negative side
        if (x_dir == -1)
        {
            if (x_steps <= -x_steps_2)
            {
                x_dir = 1;
                hw_x_set_dir(1);
            }
        }
    }

    hw_x_step();
    x_steps += x_dir;
}

void core_loop(int delay_us)
{
    if (state == CORE_STATE_IDLE || state == CORE_STATE_INIT || state == CORE_STATE_STOP)
        return;

    x_timer += delay_us;
    table_timer += delay_us;
    mirror_timer += delay_us;

    if (x_timer >= x_delay_us)
    {
        core_make_x_step();
        x_timer = 0;
    }
    else
    {
        hw_clear_x_step();
    }

    if (mirror_timer >= mirror_delay_us)
    {
        hw_mirror_step();
        mirror_timer = 0;
    }
    else
    {
        hw_clear_mirror_step();
    }

    if (table_timer >= table_delay_us)
    {
        hw_table_step();
        table_timer = 0;
    }
    else
    {
        hw_clear_table_step();
    }
}

bool core_is_running(void)
{
    return (state == CORE_STATE_RUN) || (CORE_STATE_FINISHING);
}

void core_get_ampl(int *ap, int *am)
{
    *ap = X_AMPL_1;
    *am = X_AMPL_2;    
}
