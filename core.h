#pragma once

#include <stdbool.h>

// Config functiions
void core_calculate_parameters(void);
void core_set_amplitude_1(float ampl);
void core_set_amplitude_2(float ampl);
void core_set_amplitude_symmetric(float ampl);
void core_set_amplitude_single(float ampl);

// State functions
void core_setup(void);
void core_run(void);
void core_stop(void);
void core_finish(void);

// Processing functions
void core_make_x_step(void);
void core_loop(int delay_us);
