// Clock Library
// Jason Losh

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL
// Target uC:       TM4C123GH6PM
// System Clock:    -

// Hardware configuration:
// 16 MHz external crystal oscillator

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdint.h>
#include "clock.h"
#include "tm4c123gh6pm.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

// Initialize system clock to 40 MHz using PLL and 16 MHz crystal oscillator
void initSystemClockTo40Mhz(void)
{
    // Configure HW to work with 16 MHz XTAL, PLL enabled, sysdivider of 5, creating system clock of 40 MHz
    SYSCTL_RCC_R = SYSCTL_RCC_XTAL_16MHZ | SYSCTL_RCC_OSCSRC_MAIN | SYSCTL_RCC_USESYSDIV | (4 << SYSCTL_RCC_SYSDIV_S);
}

void initSystemClockTo80Mhz(void)
{
    initSystemClock(_40MHZ, _16_0MHZ, MOSC);
    SYSCTL_RCC2_R = (1 << 31) | (1 << 30) | (0x02 << 23) | (0 << 22) | (0x0 << 4);
}

void initSystemClock(SYSDIV sysdiv, XTAL xtal, OSCSRC oscsrc)
{
    SYSCTL_RCC_R = (sysdiv << 23) | (1 << 22) | (xtal << 6) | (oscsrc << 4) | 0x0;
}
