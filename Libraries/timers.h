// Timers Library
// Nathan Fusselman

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL
// Target uC:       TM4C123GH6PM
// System Clock:    -

// Hardware configuration:
// GPIO APB ports A-F

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#ifndef TIMERS_H_
#define TIMERS_H_

#include <stdint.h>
#include <stdbool.h>

// Enum values set to bitband address of bit 0 of the XTIMERX_CFG_R register
typedef enum _TIMER
{
    TIMER_0 = 0x40030000,
    TIMER_1 = 0x40031000,
    TIMER_2 = 0x40032000,
    TIMER_3 = 0x40033000,
    TIMER_4 = 0x40034000,
    TIMER_5 = 0x40035000,
    WTIMER_0 = 0x40036000,
    WTIMER_1 = 0x40037000,
    WTIMER_2 = 0x40038000,
    WTIMER_3 = 0x40039000,
    WTIMER_4 = 0x4003A000,
    WTIMER_5 = 0x4003B000
} TIMER;

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

void initTimer(TIMER timer);
void resetTimer(TIMER timer);
void enableTimer(TIMER timer);
void disableTimer(TIMER timer);
void enableTimerInterrupt(TIMER timer);
void disableTimerInterrupt(TIMER timer);
void clearTimerInterrupt(TIMER timer);
uint64_t getTimerValue(TIMER timer);

#endif
