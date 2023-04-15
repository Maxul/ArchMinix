/*
 * spinlock.c
 *
 */
#include <rtems.h>
#include <stdint.h>
#include <ti/csl/csl_semAux.h>

void spinlock_lock(uint8_t num)
{
    ISR_Level level;

    _ISR_Disable(level);
    while (CSL_semAcquireDirect(num) == 0)
        ;
    _ISR_Enable(level);
}

void spinlock_unlock(uint8_t num)
{
    ISR_Level level;

    _ISR_Disable(level);
    CSL_semReleaseSemaphore(num);
    _ISR_Enable(level);
}
