// ADC Library
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

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <stdbool.h>

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

void initADC(uint8_t ADC, uint8_t SS);
void setADCSSLog2AverageCount(uint8_t ADC, uint8_t SS, uint8_t log2AverageCount, uint32_t ADC_CTL_DITHER);
void setADCSSMux(uint8_t ADC, uint8_t SS, uint8_t input);
int16_t readADCSS(uint8_t ADC, uint8_t SS);

#endif
