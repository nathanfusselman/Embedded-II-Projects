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
#include "tm4c123gh6pm.h"

#define HIGH_LEVEL 3.3      // High Voltage
#define LOW_LEVEL 0         // Low Voltage
#define ADC_RES 4096.0        // Resolution of the ADC module
#define R3 33               // R3 Resistor value

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
    case 7:
        break;
    }
}

RESULT runMeasure(TYPE type)
{
    RESULT result;
    setOff();
    result.type = type;
    switch (type)
    {
    case Voltage:
        result.value = testVoltage();
        break;
    case Resistance:
        result.value = testResistanceLowRes();
        break;
    default:
        result.value = 99.99999;
        break;
    }
    ftoa(result.value, result.valueString, 8);
    return result;
}

float testResistanceLowRes()
{
    setOff();
    setPinValue(MEAS_LR, 1);
    setPinValue(LOWSIDE_R, 1);
    float value = testVoltage();
    setOff();
    return ((R3 * (HIGH_LEVEL - value)) / value);
}

float testResistanceHighRes()
{
    setOff();
    setPinValue(INTEGRATE, 1);

    setPinValue(LOWSIDE_R, 1);
    setPinValue(MEAS_LR, 0);

    waitMicrosecond(10000000);

    setPinValue(LOWSIDE_R, 0);
    setPinValue(MEAS_LR, 1);

    setOff();
    return 0;
}

float testVoltage()
{
    uint16_t raw = readADCSS(0,0);
    return (raw / (ADC_RES / HIGH_LEVEL));
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
