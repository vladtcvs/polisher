#include <stdbool.h>
#include "hw.h"
#include "config.h"

#define setbit(port, bit)   \
do {                        \
    port |= (1<<(bit));     \
} while (0)

#define clearbit(port, bit) \
do {                        \
    port &= ~(1<<(bit));    \
} while (0)

void hw_mirror_step(void)
{
    setbit(MIRROR_PORT, MIRROR_STEP_PIN);
}

void hw_mirror_set_dir(int dir)
{
  if (dir == 1)
    setbit(MIRROR_PORT, MIRROR_DIR_PIN);
  else
    clearbit(MIRROR_PORT, MIRROR_DIR_PIN);
}

void hw_clear_mirror_step(void)
{
    clearbit(MIRROR_PORT, MIRROR_STEP_PIN);
}

void hw_table_step(void)
{
    setbit(TABLE_PORT, TABLE_STEP_PIN);
}

void hw_table_set_dir(int dir)
{
    if (dir == 1)
        setbit(TABLE_PORT, TABLE_DIR_PIN);
    else
        clearbit(TABLE_PORT, TABLE_DIR_PIN);
}

void hw_clear_table_step(void)
{
    clearbit(TABLE_PORT, TABLE_STEP_PIN);
}

void hw_x_step(void)
{
    setbit(X_PORT, X_STEP_PIN);
}

void hw_x_set_dir(int dir)
{
    if (dir == 1)
        setbit(X_PORT, X_DIR_PIN);
    else
        clearbit(X_PORT, X_DIR_PIN);
}

void hw_clear_x_step(void)
{
    clearbit(TABLE_PORT, TABLE_STEP_PIN);
}

void hw_enable(bool en)
{
    if (en)
        clearbit(EN_PORT, EN_PIN);
    else
        setbit(EN_PORT, EN_PIN);
}

void hw_setup(void)
{
    setbit(X_DDR, X_STEP_PIN);
    setbit(X_DDR, X_DIR_PIN);

    setbit(MIRROR_DDR, MIRROR_STEP_PIN);
    setbit(MIRROR_DDR, MIRROR_DIR_PIN);

    setbit(TABLE_DDR, TABLE_STEP_PIN);
    setbit(TABLE_DDR, TABLE_DIR_PIN);

    setbit(EN_DDR, EN_PIN);
}
