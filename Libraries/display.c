// Display Library
// Nathan Fusselman

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL
// Target uC:       TM4C123GH6PM
// System Clock:    -

// Hardware configuration:
// Display Module

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "display.h"
#include "gpio.h"
#include "wait.h"
#include "tm4c123gh6pm.h"

#define RS PORTC,6 //PIN: 4
#define RW PORTC,5 //PIN: 5
#define E PORTC,4 //PIN: 6
#define DB0 PORTB,0 //PIN: 7
#define DB1 PORTB,1 //PIN: 8
#define DB2 PORTB,2 //PIN: 9
#define DB3 PORTB,3 //PIN: 10
#define DB4 PORTB,4 //PIN: 11
#define DB5 PORTB,5 //PIN: 12
#define DB6 PORTB,6 //PIN: 13
#define DB7 PORTB,7 //PIN: 14

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

// Initialize Display
void initDisp()
{
    // Enable clocks
    enablePort(PORTB);
    enablePort(PORTC);

    // Configure Control pins
    selectPinPushPullOutput(RS);
    selectPinPushPullOutput(RW);
    selectPinPushPullOutput(E);

    // Configure Data pins
    selectPinOpenDrainOutput(DB0);
    selectPinOpenDrainOutput(DB1);
    selectPinOpenDrainOutput(DB2);
    selectPinOpenDrainOutput(DB3);
    selectPinOpenDrainOutput(DB4);
    selectPinOpenDrainOutput(DB5);
    selectPinOpenDrainOutput(DB6);
    selectPinOpenDrainOutput(DB7);

    // Setting Pins to 0
    setPinValue(RS, 0);
    setPinValue(RW, 0);
    setPinValue(E, 0);
    setPortValue(PORTB, 0);

    // Setting 8-Bit Mode
    waitMicrosecond(50 * 1000);
    sendData(0, 0, 0x30);
    waitMicrosecond(5 * 1000);
    sendData(0, 0, 0x30);
    waitMicrosecond(100);
    sendData(0, 0, 0x30);

    // Display Setup
    sendData(0, 0, 0x3C);
    sendData(0, 0, 0x0C);
    sendData(0, 0, 0x01);
    sendData(0, 0, 0x07);

    waitBusy();
}

void sendData(bool rs, bool rw, uint8_t data)
{
    setPinValue(RS, rs);
    setPinValue(RW, rw);

    setPortValue(PORTB, data);

    setPinValue(E, 0);
    waitMicrosecond(25);
    setPinValue(E, 1);
    waitMicrosecond(25
                    );
    setPinValue(E, 0);

    setPinValue(RS, 0);
    setPinValue(RW, 0);

    setPortValue(PORTB, 0);

    waitBusy();
}

uint8_t readData(bool rs)
{
    setPinValue(RS, rs);
    setPinValue(RW, 1);

    return getPortValue(PORTB);
}

void writeDisplay(char * data)
{
    sendData(0, 0, 0x01);
    sendData(0, 0, 0x07);
    waitMicrosecond(5 * 1000);
    uint8_t i = 0;
    uint8_t l = 0;
    while (data[i] != '\0')
    {
        if (data[i] == '\n')
        {
            if (l < 1)
                setCursour(0, 1);
            l++;
        }
        if (data[i] != '\n' && data[i] != '^')
            sendData(1, 0, data[i]);
        if (data[i] == '^')
        {
            i++;
            switch (data[i])
            {
            case '@':
                sendData(1, 0, 0);
                break;
            case 'A':
                sendData(1, 0, 1);
                break;
            case 'B':
                sendData(1, 0, 2);
                break;
            case 'C':
                sendData(1, 0, 3);
                break;
            case 'D':
                sendData(1, 0, 4);
                break;
            case 'E':
                sendData(1, 0, 5);
                break;
            case 'F':
                sendData(1, 0, 6);
                break;
            case 'G':
                sendData(1, 0, 7);
                break;
            }
        }
        i++;
        waitBusy();
    }
}

void setCursour(uint8_t x, uint8_t y)
{
    if (x < 16)
    {
        if (y == 0)
            sendData(0, 0, 0x80 + x);
        if (y == 1)
            sendData(0, 0, 0xC0 + x);
    }
    waitBusy();
}

void shiftDisplay(bool sc, bool rl)
{
    sendData(0, 0, ((sc * 2) & rl) << 2);
}

void waitBusy()
{
    while (readData(0) > 0x80);
}

void setCharacter(uint8_t n, uint8_t c[])
{
    if (n < 8)
    {
        sendData(0, 0, 0x40 + (n * 10));
        uint8_t i = 0;
        for (i = 0; i < 10; i++)
            sendData(1, 0, c[i]);
    }
}
