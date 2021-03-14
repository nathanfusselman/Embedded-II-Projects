// Timers Library
// Nathan Fusselman

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

// Hardware configuration:
// TIMERS 0-5 and WTIMERS 0-5

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "timers.h"

// Bit offset of the registers relative to CFG_R
#define OFS_CFG_CFG        0x000
#define OFS_CFG_TAMR       0x004
#define OFS_CFG_TBMR       0x008
#define OFS_CFG_CTL        0x00C
#define OFS_CFG_SYNC       0x010
#define OFS_CFG_IMR        0x018
#define OFS_CFG_RIS        0x01C
#define OFS_CFG_MIS        0x020
#define OFS_CFG_ICR        0x024
#define OFS_CFG_TAILR      0x028
#define OFS_CFG_TBILR      0x02C
#define OFS_CFG_TAMATCHR   0x030
#define OFS_CFG_TBMATCHR   0x034
#define OFS_CFG_TAPR       0x038
#define OFS_CFG_TBPR       0x03C
#define OFS_CFG_TAPMR      0x040
#define OFS_CFG_TBPRM      0x044
#define OFS_CFG_TAR        0x048
#define OFS_CFG_TBR        0x04C
#define OFS_CFG_TAV        0x050
#define OFS_CFG_TBV        0x054
#define OFS_CFG_RTCPD      0x058
#define OFS_CFG_TAPS       0x05C
#define OFS_CFG_TBPS       0x060
#define OFS_CFG_TAPV       0x064
#define OFS_CFG_TBPV       0x068
#define OFS_CFG_PP         0xFC0


//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

void initTimer(TIMER timer)
{
    uint32_t* p;

    if (((timer & 0x0000F000) >> 12) < 6)
        SYSCTL_RCGCTIMER_R |= (1 << (((timer & 0x0000F000) >> 12) % 6));
    else
        SYSCTL_RCGCWTIMER_R |= (1 << (((timer & 0x0000F000) >> 12) % 6));

    _delay_cycles(3);

    p = (uint32_t*)(timer + OFS_CFG_CTL);
    *p &= ~TIMER_CTL_TAEN;
    p = (uint32_t*)(timer + OFS_CFG_CFG);
    *p = 0x0;
    p = (uint32_t*)(timer + OFS_CFG_TAMR);
    *p = TIMER_TAMR_TACMR | TIMER_TAMR_TAMR_CAP | TIMER_TAMR_TACDIR;
    p = (uint32_t*)(timer + OFS_CFG_CTL);
    *p = TIMER_CTL_TAEVENT_POS;

    resetTimer(timer);
}

void resetTimer(TIMER timer)
{
    uint32_t* p;

    p = (uint32_t*)(timer + OFS_CFG_TAV);
    *p = 0;
    p = (uint32_t*)(timer + OFS_CFG_TBV);
    *p = 0;
}

void enableTimer(TIMER timer)
{
    uint32_t* p;

    p = (uint32_t*)(timer + OFS_CFG_CTL);
    *p |= TIMER_CTL_TAEN;
}

void disableTimer(TIMER timer)
{
    uint32_t* p;

    p = (uint32_t*)(timer + OFS_CFG_CTL);
    *p &= ~TIMER_CTL_TAEN;
}

void enableTimerInterrupt(TIMER timer)
{
    uint32_t* p;

    p = (uint32_t*)(timer + OFS_CFG_IMR);
    *p = TIMER_IMR_TATOIM;

    if (((timer & 0x0000F000) >> 12) < 6)
    {
        NVIC_EN0_R |= 1 << (INT_TIMER0A - 16 + (((timer & 0x0000F000) >> 12) % 6)*2);
    }
    else
    {
        NVIC_EN0_R |= 1 << (INT_WTIMER0A - 16 + (((timer & 0x0000F000) >> 12) % 6)*2);
    }
}

void disableTimerInterrupt(TIMER timer)
{
    uint32_t* p;

    p = (uint32_t*)(timer + OFS_CFG_IMR);
    *p &= ~TIMER_IMR_TATOIM;

    if (((timer & 0x0000F000) >> 12) < 6)
    {
        NVIC_EN0_R &= ~(1 << (INT_TIMER0A - 16 + (((timer & 0x0000F000) >> 12) % 6)*2));
    }
    else
    {
        NVIC_EN0_R &= ~(1 << (INT_WTIMER0A - 16 + (((timer & 0x0000F000) >> 12) % 6)*2));
    }
}

void clearTimerInterrupt(TIMER timer)
{
    uint32_t* p;
    p = (uint32_t*)(timer + OFS_CFG_ICR);
    *p |= 0x1;
}

uint64_t getTimerValue(TIMER timer)
{
    uint32_t* p;
    uint64_t value = 0;

    if (((timer & 0x0000F000) >> 12) < 6)
    {
        p = (uint32_t*)(timer + OFS_CFG_TAV);
        value |= *p;
    }
    else
    {
        p = (uint32_t*)(timer + OFS_CFG_TBV);
        value |= *p;
        value = value << 32;
        p = (uint32_t*)(timer + OFS_CFG_TAV);
        value |= *p;
    }
    return value;
}

