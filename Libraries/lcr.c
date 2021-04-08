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
#include <math.h>
#include "gpio.h"
#include "wait.h"
#include "lcr.h"
#include "analog.h"
#include "timers.h"
#include "uart0.h"
#include "main.h"
#include "tm4c123gh6pm.h"

#define DELAY 250000

#define HIGH_LEVEL 3.3       // High Voltage
#define LOW_LEVEL 0          // Low Voltage
#define ADC_RES 4096.0       // Resolution of the ADC module
#define R3 33                // R3 Resistor value 33 Ohms
#define R15 100000           // R15 Resistor value 100k Ohms
#define C1 1e-6              // C1 Capacitance value 1 uF
#define COMP_THREASHOLD 2.469// Voltage level of Comparator

#define RESISTANCE_MULT 1
#define RESISTANCE_DELAY 1000
#define RESISTANCE_TIMER WTIMER_0

#define CAPACITANCE_MULT 1000000
#define CAPACITANCE_DELAY 3000
#define CAPACITANCE_TIMER WTIMER_0

#define INDUCATNCE_CAL_CUTOFF 100
#define INDUCTANCE_CAL_MULT 1.3196
#define INDUCTANCE_CAL_OFFSET -21.179
#define INDUCTANCE_MULT 1000000
#define INDUCTANCE_DELAY 50
#define INDUCTANCE_TIMER WTIMER_0

#define ESR_OFFSET 0.40

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
    disableTimer(INDUCTANCE_TIMER);
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

double testVoltage(bool first)
{
    setOff();

    setPinValue(MEAS_C, 1);

    if (first)
        printWaiting(0);

    waitMicrosecond(DELAY);

    double value = getVoltage();

    if (currentState == CANCELED)
        return -1;

    setOff();

    return value;
}

double testResistance(bool first)
{
    double value = 0;

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

    enableTimer(RESISTANCE_TIMER);

    setPinValue(LOWSIDE_R, 0);
    setPinValue(MEAS_LR, 1);

    while (currentState == TESTING_R)
    {
        if (first)
        {
            count = printWaiting(count);
            waitMicrosecond(DELAY);
        }
    }

    disableACInterrupt(0);

    setOff();

    uint64_t raw_t = getTimerValue(RESISTANCE_TIMER);

    double t = getTime((double) raw_t - RESISTANCE_DELAY);

    if (raw_t < RESISTANCE_DELAY)
        t = 0;

    value = -(t / ( C1 * log( -( (COMP_THREASHOLD - HIGH_LEVEL) / HIGH_LEVEL ) ) ) ); // Derived from RC charging equation

    value *= RESISTANCE_MULT;

    if (value < 0)
        value = 0;

    if (currentState == CANCELED)
            return -1;

    return value;
}

double testCapacitance(bool first)
{
    double value = 0;

    uint8_t count = 0;

    setOff();

    setPinValue(LOWSIDE_R, 1);
    setPinValue(MEAS_C, 1);

    waitMicrosecond(DELAY);

    enableACInterrupt(0);

    currentState = TESTING_C;

    disableTimer(CAPACITANCE_TIMER);

    resetTimer(CAPACITANCE_TIMER);

    enableTimer(CAPACITANCE_TIMER);

    setPinValue(LOWSIDE_R, 0);
    setPinValue(HIGHSIDE_R, 1);

    while (currentState == TESTING_C)
    {
        if (first)
        {
            count = printWaiting(count);
            waitMicrosecond(DELAY);
        }
    }

    disableACInterrupt(0);

    setOff();

    uint64_t raw_t = getTimerValue(CAPACITANCE_TIMER);

    double t = getTime((double) raw_t - CAPACITANCE_DELAY);

    if (raw_t < CAPACITANCE_DELAY)
        t = 0;

    value = -(t / ( R15 * log( -( (COMP_THREASHOLD - HIGH_LEVEL) / HIGH_LEVEL ) ) ) ); // Derived from RC charging equation

    value *= CAPACITANCE_MULT; // Convert to uF

    if (value < 0)
        value = 0;

    if (currentState == CANCELED)
            return -1;

    return value;
}

double testInductance(bool first)
{
    double value = 0;

    uint64_t raw_t = 0;

    uint8_t count = 0;

    uint8_t level = 0x5;

    setOff();

    double ESR = testESR(false);

    if (ESR < 5)
        level = 0xF;

    initAC(0, true, false, level, 0x02, true);

    setOff();

    setPinValue(LOWSIDE_R, 1);
    setPinValue(MEAS_C, 1);

    waitMicrosecond(DELAY);

    setPinValue(MEAS_C, 0);

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

    disableACInterrupt(0);

    setOff();

    raw_t = getTimerValue(INDUCTANCE_TIMER);

    double t = getTime((double) raw_t - INDUCTANCE_DELAY);

    if (raw_t < INDUCTANCE_DELAY)
        t = 0;

    double I = (0.786 + (level * 0.112)) / R3;

    double R = ESR + R3;

    value = -( ( R * t ) / log( -( ( ( R * I ) - HIGH_LEVEL ) / HIGH_LEVEL ) ) );

    value *= INDUCTANCE_MULT; // Convert to uH

    if (value < INDUCATNCE_CAL_CUTOFF)
        value = ( ( INDUCTANCE_CAL_MULT * value ) + INDUCTANCE_CAL_OFFSET );

    if (value < 0)
        value = 0;

    if (currentState == CANCELED)
            return -1;

    initAC(0, true, false, 0xF, 0x02, true);

    return value;
}

double testESR(bool first)
{
    double value = 0;

    setOff();

    setPinValue(MEAS_LR, 1);
    setPinValue(LOWSIDE_R, 1);

    if (first)
        printWaiting(0);

    waitMicrosecond(DELAY);

    double raw_DUT2 = getVoltage();

    setOff();

    if (currentState == CANCELED)
        return -1;

    double DUT2 = raw_DUT2 + ESR_OFFSET;

    if (DUT2 > HIGH_LEVEL)
        DUT2 = HIGH_LEVEL;

    value = ( ( ( HIGH_LEVEL * R3 ) / DUT2 ) - R3 ); // Derived from Voltage Divider Law

    return value;
}

double getVoltage()
{
    uint16_t raw = readADCSS(0,0);

    return ((double) raw / (ADC_RES / HIGH_LEVEL));
}

double getTime(uint64_t cycles)
{
    double dCycles = (double) cycles;
    double dFreq = (double) PROCESSOR_FREQUENCY;
    return (dCycles / dFreq);
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

void ftoa(double n, char* res, int afterpoint)
{
    int ipart = (int)n;

    double fpart = n - (double)ipart;

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
