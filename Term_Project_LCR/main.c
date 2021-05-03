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
#include "timers.h"
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
    //initSystemClock(_40MHZ, _16_0MHZ, MOSC);
    initSystemClockTo80Mhz();
}

//Restart the System
void rebootSystem()
{
    NVIC_APINT_R = (0x05FA0000 | NVIC_APINT_SYSRESETREQ);
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
                handleMeasure(Resistance, true);
                break;
            case 2:
                handleMeasure(Capacitance, true);
                break;
            case 3:
                handleMeasure(Inductance, true);
                break;
            case 4:
                handleMeasure(ESR, true);
                break;
            case 5:
                handleMeasure(Voltage, true);
                break;
            case 6:
                handleMeasure(AUTO, true);
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
                putsUart0("\n   LCR TESTER   \nDUT1: 3.10-3.20V\n");
                break;
            case 2:
                writeDisplay("   LCR TESTER   \nDUT1: 0.10-0.20V");
                putsUart0("\n   LCR TESTER   \nDUT1: 0.10-0.20V\n");
                break;
            case 3:
                writeDisplay("   LCR TESTER   \nDUT2: 3.00-3.15V");
                putsUart0("\n   LCR TESTER   \nDUT2: 3.00-3.15V\n");
                break;
            case 4:
                writeDisplay("   LCR TESTER   \nDUT2: 0.10-0.20V");
                putsUart0("\n   LCR TESTER   \nDUT2: 0.10-0.20V\n");
                break;
            case 5:
                writeDisplay("   LCR TESTER   \nDUT2: 0.15-0.25V");
                putsUart0("\n   LCR TESTER   \nDUT2: 0.15-0.25V\n");
                break;
            case 6:
                writeDisplay("   LCR TESTER   \nDUT2: 3.00-3.15V");
                putsUart0("\n   LCR TESTER   \nDUT2: 3.00-3.15V\n");
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
    putsUart0("\n  LCR PROJECT!   \n    WELCOME!    \n");
}

void toTestPage()
{
    currentStatus = STOPPED;
    currentMode = Test;
    testDisplay();
    writeDisplay("   LCR TESTER   \nDUT1, DUT2: XXXX");
    putsUart0("\n   LCR TESTER   \nDUT1, DUT2: XXXX\n");
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

void handleMeasure(TYPE test, bool repeat)
{
    currentStatus = TESTING;
    clearDisplay();

    uint8_t i;

    bool first = true;

    char * suffix = " __ ";
    char * suffixUart = " __ ";

    char fullString[] = "                 ";

    switch (test)
    {
    case Voltage:
        writeDisplayLine(0, "    VOLTAGE     ");
        putsUart0("\nMeasuring Voltage: \n");
        break;
    case Resistance:
        writeDisplayLine(0, "   RESISTANCE   ");
        putsUart0("\nMeasuring Resistance: \n");
        break;
    case Capacitance:
        writeDisplayLine(0, "  CAPACITANCE    ");
        putsUart0("\nMeasuring Capacitance: \n");
        break;
    case Inductance:
        writeDisplayLine(0, "   INDUCTANCE   ");
        putsUart0("\nMeasuring Inductance: \n");
        break;
    case ESR:
        writeDisplayLine(0, "      ESR       ");
        putsUart0("\nMeasuring ESR: \n");
        break;
    case AUTO:
        writeDisplayLine(0, "      AUTO      ");
        putsUart0("\nMeasuring Auto: \n");
        break;
    }

    do
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
            suffixUart = " V  ";
            break;
        case Resistance:
            suffix = " ^0 ";
            suffixUart = " Ohm";
            break;
        case Capacitance:
            suffix = " ^1F";
            suffixUart = " uF ";
            break;
        case Inductance:
            suffix = " ^1H";
            suffixUart = " uH ";
            break;
        case ESR:
            suffix = " ^0 ";
            suffixUart = " Ohm";
            break;
        }

        if (result.valueString[7] == '.')
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
        putcUart0('\t');
        putsUart0(result.valueString);
        putsUart0(suffixUart);
        putcUart0('\n');
        waitMicrosecond(1000000);
    }
    while (repeat);
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(void)
{
    USER_DATA serialData;

    // Initialize hardware
    initHw();
    initLCR();
    initDisp();
    initButtons();
    initUart0();
    setUart0BaudRate(115200, PROCESSOR_FREQUENCY);

    uint8_t ohm[] = {0,14,17,17,17,10,27,0};
    uint8_t micro[] = {0,0,18,18,18,29,16,16};

    setCharacter(0, ohm);
    setCharacter(1, micro);

    enableAllListen();

    toHomePage();

    while(true)
    {
        if (kbhitUart0())
        {
            getsUart0(&serialData);
            parseFields(&serialData);
            bool validCmd = false;
            if (isCommand(&serialData, "HELP", 0))
            {
                putsUart0("List of available commands:\n");
                putsUart0("\tHELP\n");
                putsUart0("\tRES\n");
                putsUart0("\tCAP\n");
                putsUart0("\tINDUCT\n");
                putsUart0("\tESR\n");
                putsUart0("\tVOLT\n");
                putsUart0("\tAUTO\n");
                putsUart0("\tTEST\n");
                putsUart0("\tCANCEL\n");
                putsUart0("\tREBOOT\n");
                validCmd = true;
            }
            if (isCommand(&serialData, "RES", 0))
            {
                validCmd = true;
                handleMeasure(Resistance, false);
            }
            if (isCommand(&serialData, "CAP", 0))
            {
                validCmd = true;
                handleMeasure(Capacitance, false);
            }
            if (isCommand(&serialData, "INDUCT", 0))
            {
                validCmd = true;
                handleMeasure(Inductance, false);
            }
            if (isCommand(&serialData, "ESR", 0))
            {
                validCmd = true;
                handleMeasure(ESR, false);
            }
            if (isCommand(&serialData, "VOLT", 0))
            {
                validCmd = true;
                handleMeasure(Voltage, false);
            }
            if (isCommand(&serialData, "AUTO", 0))
            {
                validCmd = true;
                handleMeasure(AUTO, false);
            }
            if (isCommand(&serialData, "TEST", 0))
            {
                validCmd = true;
                cancelTest();
                toTestPage();
            }
            if (isCommand(&serialData, "CANCEL", 0))
            {
                validCmd = true;
                cancelTest();
                toHomePage();
            }
            if (isCommand(&serialData, "REBOOT", 0))
            {
                validCmd = true;
                rebootSystem();
            }
            if (!validCmd)
            {
                if(serialData.fieldCount != 0)
                    putsUart0("*Invalid command.Try again*\n");
            }
        }
    }
}
