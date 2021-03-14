// Buttons Library
// Nathan Fusselman

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL
// Target uC:       TM4C123GH6PM
// System Clock:    -

// Hardware configuration:
// Buttons Module

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "gpio.h"
#include "wait.h"
#include "buttons.h"
#include "tm4c123gh6pm.h"

#define Button0 PORTF,4
#define Button1 PORTE,0
#define Button2 PORTE,1
#define Button3 PORTE,2
#define Button4 PORTE,3
#define Button5 PORTE,4
#define Button6 PORTE,5
#define Button7 PORTF,0

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

// Initialize Display
void initButtons()
{

    NVIC_PRI1_R |= NVIC_PRI1_INT4_M; // Lower priority for Port E buttons

    // Enable clocks
    enablePort(PORTE);
    enablePort(PORTF);

    // Configure Port E Input pins
    selectPinDigitalInput(Button1);
    selectPinDigitalInput(Button2);
    selectPinDigitalInput(Button3);
    selectPinDigitalInput(Button4);
    selectPinDigitalInput(Button5);
    selectPinDigitalInput(Button6);
    enablePinPulldown(Button1);
    enablePinPulldown(Button2);
    enablePinPulldown(Button3);
    enablePinPulldown(Button4);
    enablePinPulldown(Button5);
    enablePinPulldown(Button6);

    // Configure Port F Input pins
    selectPinDigitalInput(Button0);
    enablePinPullup(Button0);
    setPinCommitControl(Button7);
    selectPinDigitalInput(Button7);
    enablePinPullup(Button7);
}

void enableAllListen()
{
    uint8_t i;
    for (i = 0; i < 8; i++)
        enableListen(i);
}

void disableAllListen()
{
    uint8_t i;
    for (i = 0; i < 8; i++)
        enableListen(i);
}

void enableListen(uint8_t num)
{
    switch (num)
    {
    case 0:
        selectPinInterruptFallingEdge(Button0);
        enablePinInterrupt(Button0);
        break;
    case 1:
        selectPinInterruptRisingEdge(Button1);
        enablePinInterrupt(Button1);
        break;
    case 2:
        selectPinInterruptRisingEdge(Button2);
        enablePinInterrupt(Button2);
        break;
    case 3:
        selectPinInterruptRisingEdge(Button3);
        enablePinInterrupt(Button3);
        break;
    case 4:
        selectPinInterruptRisingEdge(Button4);
        enablePinInterrupt(Button4);
        break;
    case 5:
        selectPinInterruptRisingEdge(Button5);
        enablePinInterrupt(Button5);
        break;
    case 6:
        selectPinInterruptRisingEdge(Button6);
        enablePinInterrupt(Button6);
        break;
    case 7:
        selectPinInterruptFallingEdge(Button7);
        enablePinInterrupt(Button7);
        break;
    }
}

void disableListen(uint8_t num)
{
    switch (num)
    {
    case 0:
        disablePinInterrupt(Button0);
        break;
    case 1:
        disablePinInterrupt(Button1);
        break;
    case 2:
        disablePinInterrupt(Button2);
        break;
    case 3:
        disablePinInterrupt(Button3);
        break;
    case 4:
        disablePinInterrupt(Button4);
        break;
    case 5:
        disablePinInterrupt(Button5);
        break;
    case 6:
        disablePinInterrupt(Button6);
        break;
    case 7:
        disablePinInterrupt(Button7);
        break;
    }
}

uint8_t getButtonPressed()
{
    uint8_t num = 255;
    if (!getPinValue(Button0))
    {
        if (num != 255)
            return 255; // Multiple buttons pressed
        num = 0;
    }
    if (getPinValue(Button1))
    {
        if (num != 255)
            return 255; // Multiple buttons pressed
        num = 1;
    }
    if (getPinValue(Button2))
    {
        if (num != 255)
            return 255; // Multiple buttons pressed
        num = 2;
    }
    if (getPinValue(Button3))
    {
        if (num != 255)
            return 255; // Multiple buttons pressed
        num = 3;
    }
    if (getPinValue(Button4))
    {
        if (num != 255)
            return 255; // Multiple buttons pressed
        num = 4;
    }
    if (getPinValue(Button5))
    {
        if (num != 255)
            return 255; // Multiple buttons pressed
        num = 5;
    }
    if (getPinValue(Button6))
    {
        if (num != 255)
            return 255; // Multiple buttons pressed
        num = 6;
    }
    if (!getPinValue(Button7))
    {
        if (num != 255)
            return 255; // Multiple buttons pressed
        num = 7;
    }
    return num;
}

void buttonPressed()
{
    uint8_t buttonNum = getButtonPressed();
    waitMicrosecond(10000);
    switch (buttonNum)
    {
    case 0:
        clearPinInterrupt(Button0);
        break;
    case 1:
        clearPinInterrupt(Button1);
        break;
    case 2:
        clearPinInterrupt(Button2);
        break;
    case 3:
        clearPinInterrupt(Button3);
        break;
    case 4:
        clearPinInterrupt(Button4);
        break;
    case 5:
        clearPinInterrupt(Button5);
        break;
    case 6:
        clearPinInterrupt(Button6);
        break;
    case 7:
        clearPinInterrupt(Button7);
        break;
    }
    handleButtonPressed(buttonNum);
}
