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

#ifndef LCR_H_
#define LCR_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum type {Voltage,Resistance,Capacitance,Inductance,ESR,AUTO} TYPE;

typedef enum state {IDLE,TESTING_R,TESTING_C,TESTING_L,CANCELED} STATE;

typedef struct result {
    TYPE type;
    double value;
    char valueString[8];
} RESULT;

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

void initLCR(void);
void onAC0(void);
void runTest(uint8_t test);
RESULT runMeasure(TYPE type, bool first);
double testESR(bool first);
double testResistance(bool first);
double testCapacitance(bool first);
double testInductance(bool first);
double testVoltage(bool first);
double getVoltage();
double getTime(uint64_t cycles);
void cancelTest(void);
void setOff(void);
void reverse(char* str, int len);
int intToStr(int x, char str[], int d);
void ftoa(double n, char* res, int afterpoint);
int power(uint8_t b, uint8_t e);
void AC0Trigger();

#endif
