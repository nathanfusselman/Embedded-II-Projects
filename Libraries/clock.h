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

#ifndef CLOCK_H_
#define CLOCK_H_

// Enum values for clock speeds
typedef enum _OSCSRC
{
    MOSC = 0x0,
    PIOSC = 0x1,
    PIOSC_4 = 0x2,
    LFIOSC = 0x3
} OSCSRC;

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

// Enum values for clock speeds
typedef enum _SYSDIV
{
    _66_67MHZ = 0x2,
    _50MHZ = 0x3,
    _40MHZ = 0x4,
    _33_33MHZ = 0x5,
    _28_57MHZ = 0x6,
    _25MHZ = 0x7,
    _22_22MHZ = 0x8,
    _20MHZ= 0x9,
    _18_18MHZ = 0xA,
    _16_67MHZ = 0xB,
    _15_38MHZ = 0xC,
    _14_29MHZ = 0xD,
    _13_33MHZ = 0xE,
    _12_5MHZ = 0xF
} SYSDIV;

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

void initSystemClockTo40Mhz(void);
void initSystemClockTo80Mhz(void);
void initSystemClock(SYSDIV sysdiv, XTAL xtal, OSCSRC oscsrc);

#endif
