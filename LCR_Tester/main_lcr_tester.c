// LCR Tester
// Nathan Fusselman

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

// Hardware configuration:
// LCR Module, PCB Model
// Display Module, PCB Model

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "main_lcr_tester.h"
#include "clock.h"
#include "gpio.h"
#include "lcr.h"
#include "display.h"
#include "buttons.h"

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

// Initialize Hardware
void initHw()
{
    // Initialize system clock to 40 MHz
    initSystemClockTo40Mhz();
}

void handleButtonPressed(uint8_t buttonNum)
{
    if (buttonNum != 255 && buttonNum != 7)
    {
        switch (buttonNum)
        {
        case 0:
            writeDisplay("   LCR TESTER   \nDUT1, DUT2: XXXX");
            break;
        case 1:
            writeDisplay("   LCR TESTER   \nDUT1: 3.10-3.20V");
            break;
        case 2:
            writeDisplay("   LCR TESTER   \nDUT1: 0.10-0.20V");
            break;
        case 3:
            writeDisplay("   LCR TESTER   \nDUT2: 3.00-3.15V");
            break;
        case 4:
            writeDisplay("   LCR TESTER   \nDUT2: 0.10-0.20V");
            break;
        case 5:
            writeDisplay("   LCR TESTER   \nDUT2: 0.15-0.25V");
            break;
        case 6:
            writeDisplay("   LCR TESTER   \nDUT2: 3.00-3.15V");
            break;
        }
        runTest(buttonNum);
    }
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(void)
{
    // Initialize hardware
    initHw();
    initLCR();
    initDisp();
    initButtons();

    enableAllListen();

    handleButtonPressed(0);

    // Endless loop
    while(true);
}
