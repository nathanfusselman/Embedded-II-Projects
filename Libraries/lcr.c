// LCR Library
// Nathan Fusselman

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL
// Target uC:       TM4C123GH6PM
// System Clock:    -

// Hardware configuration:
// LCR Module

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "wait.h"
#include "lcr.h"
#include "tm4c123gh6pm.h"

#define MEAS_LR PORTA,2
#define MEAS_C PORTA,3
#define HIGHSIDE_R PORTA,4
#define LOWSIDE_R PORTA,5
#define INTEGRATE PORTA,6
#define SENSE PORTD,2

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

// Initialize Display
void initLCR()
{
    // Enable clocks
    enablePort(PORTA);
    enablePort(PORTD);

    // Configure Control pins
    selectPinPushPullOutput(MEAS_LR);
    selectPinPushPullOutput(MEAS_C);
    selectPinPushPullOutput(HIGHSIDE_R);
    selectPinPushPullOutput(LOWSIDE_R);
    selectPinPushPullOutput(INTEGRATE);

    // Configure Input pins
    selectPinAnalogInput(SENSE);

    // Setting Pins to 0
    setOff();
}

void runTest(uint8_t test)
{
    setOff();
    switch (test)
    {
    case 0:
        break;
    case 1:
        setPinValue(MEAS_LR, 1);
        break;
    case 2:
        setPinValue(MEAS_C, 1);
        break;
    case 3:
        setPinValue(HIGHSIDE_R, 1);
        break;
    case 4:
        setPinValue(LOWSIDE_R, 1);
        break;
    case 5:
        setPinValue(LOWSIDE_R, 1);
        setPinValue(INTEGRATE, 1);
        break;
    case 6:
        setPinValue(HIGHSIDE_R, 1);
        setPinValue(INTEGRATE, 1);
        break;
    }
}

void setOff()
{
    // Setting Pins to 0
    setPinValue(MEAS_LR, 0);
    setPinValue(MEAS_C, 0);
    setPinValue(HIGHSIDE_R, 0);
    setPinValue(LOWSIDE_R, 0);
    setPinValue(INTEGRATE, 0);
}
