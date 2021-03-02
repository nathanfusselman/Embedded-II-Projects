// Term Project
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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "main.h"
#include "clock.h"
#include "gpio.h"
#include "lcr.h"
#include "display.h"
#include "buttons.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

MODE currentMode;

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
    if (buttonNum != 255)
    {
        if (currentMode == Norm) {
            switch (buttonNum)
            {
            case 0:
                toHomePage();
                break;
            case 1:
                printResult(runMeasure(Voltage));
                break;
            case 2:
                printResult(runMeasure(Resistance));
                break;
            case 3:
                printResult(runMeasure(Capacitance));
                break;
            case 4:
                printResult(runMeasure(Inductance));
                break;
            case 5:
                printResult(runMeasure(ESR));
                break;
            case 6:
                printResult(runMeasure(AUTO));
                break;
            case 7:
                toTestPage();
                break;
            }
        }
        if (currentMode == Test) {
            runTest(buttonNum);
            switch (buttonNum)
            {
            case 0:
                toHomePage();
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
            case 7:
                toTestPage();
                break;
            }
        }
    }
}

void toHomePage()
{
    currentMode = Norm;
    writeDisplay("  LCR PROJECT!   \n    WELCOME!    ");
}

void toTestPage()
{
    currentMode = Test;
    testDisplay();
    writeDisplay("   LCR TESTER   \nDUT1, DUT2: XXXX");
    runTest(7);
}

void printResult(RESULT result)
{
    clearDisplay();

    uint8_t i;

    char * suffix = " _ ";

    char fullString[] = "                ";

    switch (result.type)
    {
    case Voltage:
        writeDisplayLine(0, "    VOLTAGE     ");
        suffix = " V ";
        break;
    case Resistance:
        writeDisplayLine(0, "   RESISTANCE   ");
        suffix = " ^0";
        break;
    case Capacitance:
        writeDisplayLine(0, "  CAPACITANCE    ");
        suffix = " F ";
        break;
    case Inductance:
        writeDisplayLine(0, "   INDUCTANCE   ");
        suffix = " H ";
        break;
    case ESR:
        writeDisplayLine(0, "      ESR       ");
        suffix = " ^0";
        break;
    case AUTO:
        writeDisplayLine(0, "      AUTO      ");
        suffix = " _ ";
        break;
    }

    for (i = 0; i < 8; i++)
        fullString[i + 3] = result.valueString[i];
    for (i = 0; i < 3; i++)
        fullString[i + 11] = suffix[i];

    writeDisplayLine(1, fullString);
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

    uint8_t ohm[] = {0,14,17,17,17,10,27,0};

    setCharacter(0, ohm);

    enableAllListen();

    toHomePage();

    // Endless loop
    while(true);
}
