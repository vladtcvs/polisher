#pragma once

#include <stdbool.h>

void hw_mirror_step(void);
void hw_mirror_set_dir(int dir);
void hw_clear_mirror_step(void);

void hw_table_step(void);
void hw_table_set_dir(int dir);
void hw_clear_table_step(void);

void hw_x_step(void);
void hw_x_set_dir(int dir);
void hw_clear_x_step(void);

void hw_enable(bool en);

void hw_setup(void);
