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
#include "analog.h"
#include "timers.h"
#include "uart0.h"
#include "main.h"
#include "tm4c123gh6pm.h"

#define HIGH_LEVEL 3.3      // High Voltage
#define LOW_LEVEL 0         // Low Voltage
#define ADC_RES 4096.0      // Resolution of the ADC module
#define R3 33               // R3 Resistor value

#define RESISTANCE_OFFSET -8      // 0Ohm Timer offset
#define RESISTANCE_MULT 0.0188
#define RESISTANCE_TIMER WTIMER_0

#define CAPACITANCE_MULT 0.00000018
#define CAPACITANCE_TIMER WTIMER_0

#define INDUCTANCE_DIV 1.37 //-(ln(19/75))
#define INDUCTANCE_TIMER WTIMER_0

#define DELAY 250000

#define MEAS_LR PORTA,2
#define MEAS_C PORTA,3
#define HIGHSIDE_R PORTA,4
#define LOWSIDE_R PORTA,5
#define INTEGRATE PORTA,6
#define SENSE_ADC PORTD,2
#define SENSE_AC PORTC,7

#define SENSE_ADC_AIN 5
#define SENSE_ADC_HS 4
#define SENSE_ADC_D 0x100

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

STATE volatile currentState = IDLE;

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

// Initialize Display
void initLCR()
{
    // Enable clocks
    enablePort(PORTA);
    enablePort(PORTC);
    enablePort(PORTD);

    // Configure Control pins
    selectPinPushPullOutput(MEAS_LR);
    selectPinPushPullOutput(MEAS_C);
    selectPinPushPullOutput(HIGHSIDE_R);
    selectPinPushPullOutput(LOWSIDE_R);
    selectPinPushPullOutput(INTEGRATE);

    // Configure Input pins
    selectPinAnalogInput(SENSE_ADC);
    selectPinAnalogInput(SENSE_AC);
    initADC(0,0);
    setADCSSLog2AverageCount(0,0,SENSE_ADC_HS,SENSE_ADC_D);
    setADCSSMux(0,0,SENSE_ADC_AIN);
    initAC(0, true, false, 0xF, 0x02, true);

    // Setup Timer
    initTimer(RESISTANCE_TIMER);

    // Setting Pins to 0
    setOff();

    currentState = IDLE;
}

void onAC0()
{
    disableTimer(CAPACITANCE_TIMER);
    disableTimer(RESISTANCE_TIMER);
    currentState = IDLE;
    clearACInterrupt(0);
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
    case 7:
        break;
    }
}

RESULT runMeasure(TYPE type, bool first)
{
    RESULT result;
    setOff();
    result.type = type;
    switch (type)
    {
    case Voltage:
        result.value = testVoltage(first);
        break;
    case Resistance:
        result.value = testResistance(first);
        break;
    case Capacitance:
        result.value = testCapacitance(first);
        break;
    case Inductance:
        result.value = testInductance(first);
        break;
    case ESR:
        result.value = testESR(first);
        break;
    default:
        result.value = 99.99999;
        break;
    }
    ftoa(result.value, result.valueString, 8);
    return result;
}

float testESR(bool first)
{
    setOff();

    setPinValue(MEAS_LR, 1);
    setPinValue(LOWSIDE_R, 1);

    if (first)
        printWaiting(0);

    waitMicrosecond(DELAY);

    float DUT2 = getVoltage();

    if (currentState == CANCELED)
        return -1;

    setOff();

    return DUT2;
}

float testResistance(bool first)
{
    float value = 0;

    uint8_t count = 0;

    setOff();

    setPinValue(INTEGRATE, 1);

    setPinValue(LOWSIDE_R, 1);
    setPinValue(MEAS_LR, 0);

    waitMicrosecond(DELAY);

    enableACInterrupt(0);

    currentState = TESTING_R;

    disableTimer(RESISTANCE_TIMER);

    resetTimer(RESISTANCE_TIMER);

    setPinValue(LOWSIDE_R, 0);
    setPinValue(MEAS_LR, 1);

    enableTimer(RESISTANCE_TIMER);

    while (currentState == TESTING_R)
    {
        if (first)
        {
            count = printWaiting(count);
            waitMicrosecond(DELAY);
        }
    }

    value = ((RESISTANCE_MULT * ((float) getTimerValue(RESISTANCE_TIMER))) + RESISTANCE_OFFSET);

    if (value < 0)
        value = 0;

    disableACInterrupt(0);

    if (currentState == CANCELED)
            return -1;

    setOff();

    return value;
}

float testCapacitance(bool first)
{
    float value = 0;

    uint8_t count = 0;

    setOff();

    setPinValue(LOWSIDE_R, 1);
    setPinValue(MEAS_C, 1);

    waitMicrosecond(DELAY);

    enableACInterrupt(0);

    currentState = TESTING_C;

    disableTimer(CAPACITANCE_TIMER);

    resetTimer(CAPACITANCE_TIMER);

    setPinValue(LOWSIDE_R, 0);
    setPinValue(HIGHSIDE_R, 1);

    enableTimer(CAPACITANCE_TIMER);

    while (currentState == TESTING_C)
    {
        if (first)
        {
            count = printWaiting(count);
            waitMicrosecond(DELAY);
        }
    }

    value = ((float) getTimerValue(CAPACITANCE_TIMER) * CAPACITANCE_MULT);

    if (value < 0)
        value = 0;

    disableACInterrupt(0);

    if (currentState == CANCELED)
            return -1;

    setOff();

    return value;
}

float testInductance(bool first)
{
    float value = 0;

    uint8_t count = 0;

    setOff();

    setPinValue(LOWSIDE_R, 1);

    waitMicrosecond(DELAY);

    enableACInterrupt(0);

    currentState = TESTING_L;

    disableTimer(INDUCTANCE_TIMER);

    resetTimer(INDUCTANCE_TIMER);

    enableTimer(INDUCTANCE_TIMER);

    setPinValue(MEAS_LR, 1);

    while (currentState == TESTING_L)
    {
        if (first)
        {
            count = printWaiting(count);
            waitMicrosecond(DELAY);
        }
    }

    float ESR = testESR(false);

    value = (((float) getTimerValue(INDUCTANCE_TIMER) * (R3 + ESR)) / INDUCTANCE_DIV);

    if (value < 0)
        value = 0;

    disableACInterrupt(0);

    if (currentState == CANCELED)
            return -1;

    setOff();

    return value;
}

float testVoltage(bool first)
{
    setOff();

    setPinValue(MEAS_C, 1);

    if (first)
        printWaiting(0);

    waitMicrosecond(DELAY);

    float value = getVoltage();

    if (currentState == CANCELED)
        return -1;

    setOff();

    return value;
}

float getVoltage()
{
    uint16_t raw = readADCSS(0,0);

    return ((float) raw / (ADC_RES / HIGH_LEVEL));
}

void cancelTest()
{
    currentState = CANCELED;
    // Setting Pins to 0
    setPinValue(MEAS_LR, 0);
    setPinValue(MEAS_C, 0);
    setPinValue(HIGHSIDE_R, 0);
    setPinValue(LOWSIDE_R, 0);
    setPinValue(INTEGRATE, 0);
}

void setOff()
{
    currentState = IDLE;
    // Setting Pins to 0
    setPinValue(MEAS_LR, 0);
    setPinValue(MEAS_C, 0);
    setPinValue(HIGHSIDE_R, 0);
    setPinValue(LOWSIDE_R, 0);
    setPinValue(INTEGRATE, 0);
}

void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

void ftoa(float n, char* res, int afterpoint)
{
    int ipart = (int)n;

    float fpart = n - (float)ipart;

    int i = intToStr(ipart, res, 0);

    if (afterpoint != 0)
    {
        res[i] = '.';

        fpart = fpart * power(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

int power(uint8_t b, uint8_t e)
{
    int ans = 1;
    uint8_t i;
    for (i = 0; i < e; i++)
        ans *= b;
    return ans;
}
