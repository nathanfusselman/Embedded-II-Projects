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

// Enum values for clock speeds
typedef enum _XTAL
{
    _4MHZ = 0x06,
    _4_096MHZ = 0x07,
    _4_9152MHZ = 0x08,
    _5MHZ = 0x09,
    _5_12MHZ = 0x0A,
    _6MHZ = 0x0B,
    _6_144MHZ = 0x0C,
    _7_3728MHZ= 0x0D,
    _8MHZ = 0x0E,
    _8_192MHZ = 0x0F,
    _10_0MHZ = 0x10,
    _12_0MHZ = 0x11,
    _12_288MHZ = 0x12,
    _13_56MHZ = 0x13,
    _14_31818MHZ = 0x14,
    _16_0MHZ = 0x15,
    _16_384MHZ = 0x16,
    _18_0MHZ = 0x17,
    _20_0MHZ = 0x18,
    _24_0MHZ = 0x19,
    _25_0MHZ = 0x1A
} XTAL;

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

void initSystemClock(XTAL xtal)
{
    SYSCTL_RCC_R = (xtal << 6) | 0x0;
}
