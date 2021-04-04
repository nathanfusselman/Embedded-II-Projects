// Wait functions
// Jason Losh

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "wait.h"
#include "main.h"

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

// Approximate busy waiting (in units of microseconds), given a PROCESSOR_FREQUENCY MHz system clock
void waitMicrosecond(uint64_t us)
{
    us = (us * PROCESSOR_FREQUENCY / 5000000);
    __asm("WMS_LOOP:    SUB  R0, #1");         // 1
    __asm("             CBZ  R0, WMS_DONE");   // 1
    __asm("             B    WMS_LOOP");       // 1*2 (speculative, so P=1)
    __asm("WMS_DONE:");                        // ---
                                               // PROCESSOR_FREQUENCY clocks/us + error
}


