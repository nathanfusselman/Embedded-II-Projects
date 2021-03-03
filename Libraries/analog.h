// Analog Library
// Nathan Fusselman

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL
// Target uC:       TM4C123GH6PM
// System Clock:    -

// Hardware configuration:
// GPIO APB ports A-F

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#ifndef ANALOG_H_
#define ANALOG_H_

#include <stdint.h>
#include <stdbool.h>

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

void initADC(uint8_t ADC, uint8_t SS);
void setADCSSLog2AverageCount(uint8_t ADC, uint8_t SS, uint8_t log2AverageCount, uint32_t ADC_CTL_DITHER);
void setADCSSMux(uint8_t ADC, uint8_t SS, uint8_t input);
int16_t readADCSS(uint8_t ADC, uint8_t SS);

void initAC(uint8_t AC, bool EN, bool RNG, uint8_t VREF, uint8_t ISEN);
void enableACInterrupt(uint8_t AC);
void clearACInterrupt(uint8_t AC);
bool getAC(uint8_t AC, bool flip);

#endif
