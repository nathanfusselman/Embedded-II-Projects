// Buttons Module
// Nathan Fusselman

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

// Hardware configuration:
// Buttons Module, PCB Model

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "clock.h"
#include "gpio.h"
#include "buttons.h"

#define LEDR PORTF,1
#define LEDB PORTF,2
#define LEDG PORTF,3

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

// Initialize Hardware
void initHw()
{
    // Initialize system clock to 40 MHz
    initSystemClockTo40Mhz();

    // Enable clocks
    enablePort(PORTF);

    // Configure LED pins
    selectPinPushPullOutput(LEDR);
    selectPinPushPullOutput(LEDB);
    selectPinPushPullOutput(LEDG);

    // Set LEDs to 0
    setPinValue(LEDR, 0);
    setPinValue(LEDB, 0);
    setPinValue(LEDG, 0);
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(void)
{
    // Initialize hardware
    initHw();
    initButtons();

    // Endless loop
    while(true)
    {
        if (getButtonPressed() != 255)
            setPinValue(LEDR, !getPinValue(LEDR));
    }
}

