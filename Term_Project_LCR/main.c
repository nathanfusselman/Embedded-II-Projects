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
#include "wait.h"
#include "gpio.h"
#include "uart0.h"
#include "lcr.h"
#include "display.h"
#include "buttons.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

MODE currentMode;

STATUS currentStatus = STOPPED;

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
                cancelTest();
                toHomePage();
                break;
            case 1:
                handleMeasure(Voltage);
                break;
            case 2:
                handleMeasure(Resistance);
                break;
            case 3:
                handleMeasure(Capacitance);
                break;
            case 4:
                handleMeasure(Inductance);
                break;
            case 5:
                handleMeasure(ESR);
                break;
            case 6:
                handleMeasure(AUTO);
                break;
            case 7:
                cancelTest();
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
    currentStatus = STOPPED;
    currentMode = Norm;
    writeDisplay("  LCR PROJECT!   \n    WELCOME!    ");
}

void toTestPage()
{
    currentStatus = STOPPED;
    currentMode = Test;
    testDisplay();
    writeDisplay("   LCR TESTER   \nDUT1, DUT2: XXXX");
    runTest(7);
}

uint8_t printWaiting(uint8_t num)
{
    switch (num)
    {
    case 0:
        writeDisplayLine(1, "   WAITING      ");
        return 1;
    case 1:
        writeDisplayLine(1, "   WAITING.     ");
        return 2;
    case 2:
        writeDisplayLine(1, "   WAITING..    ");
        return 3;
    case 3:
        writeDisplayLine(1, "   WAITING...   ");
        return 0;
    }
    return 0;
}

void handleMeasure(TYPE test)
{
    currentStatus = TESTING;
    clearDisplay();

    uint8_t i;

    bool first = true;

    char * suffix = " __ ";

    char fullString[] = "                 ";

    switch (test)
    {
    case Voltage:
        writeDisplayLine(0, "    VOLTAGE     ");
        break;
    case Resistance:
        writeDisplayLine(0, "   RESISTANCE   ");
        break;
    case Capacitance:
        writeDisplayLine(0, "  CAPACITANCE    ");
        break;
    case Inductance:
        writeDisplayLine(0, "   INDUCTANCE   ");
        break;
    case ESR:
        writeDisplayLine(0, "      ESR       ");
        break;
    case AUTO:
        writeDisplayLine(0, "      AUTO      ");
        break;
    }

    while (true)
    {
        RESULT result = runMeasure(test, first);

        first = false;

        if (currentStatus == STOPPED)
            return;

        if (result.value == -1)
            return;

        switch (result.type)
        {
        case Voltage:
            suffix = " V  ";
            break;
        case Resistance:
            suffix = " ^0 ";
            break;
        case Capacitance:
            suffix = " ^1F";
            break;
        case Inductance:
            suffix = " ^1H";
            break;
        case ESR:
            suffix = " ^0 ";
            break;
        }

        if (result.valueString[7] == '.') // Removes Trailing '.'
            result.valueString[7] = ' ';

        for (i = 0; i < 8; i++)
            fullString[i + 3] = result.valueString[i];
        for (i = 0; i < 4; i++)
        {
            if (result.type == Capacitance || result.type == Inductance)
                fullString[i + 10] = suffix[i];
            else
                fullString[i + 11] = suffix[i];
        }

        writeDisplayLine(1, fullString);
        waitMicrosecond(1000000);
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
    initUart0();
    setUart0BaudRate(115200, 40e6);

    uint8_t ohm[] = {0,14,17,17,17,10,27,0};
    uint8_t micro[] = {0,0,18,18,18,29,16,16};

    setCharacter(0, ohm);
    setCharacter(1, micro);

    enableAllListen();

    toHomePage();

    //handleButtonPressed(2);

    // Endless loop
    while(true);
}
