// ADC Library
// Nathan Fusselman

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: EK-TM4C123GXL with LCD/Keyboard Interface
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz

// Hardware configuration:
// GPIO APB ports A-F

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "adc.h"

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

void initADC(uint8_t ADC, uint8_t SS)
{
    if (!(ADC < 2 && SS < 4))
        return;

    // Enable clocks
    SYSCTL_RCGCADC_R |= (1 << ADC);
    _delay_cycles(16);

    switch (ADC)
    {
    case 0:
        ADC0_ACTSS_R &= ~(1 << SS);                      // disable sample sequencer SS (SS[SS]) for programming
        ADC0_CC_R = ADC_CC_CS_SYSPLL;                    // select PLL as the time base (not needed, since default value)
        ADC0_PC_R = ADC_PC_SR_1M;                        // select 1Msps rate
        ADC0_EMUX_R = 0x0;                               // select SS[SS] bit in ADCPSSI as trigger
        switch (SS)
        {
        case 0:
            ADC0_SSCTL0_R = ADC_SSCTL0_END0;
            break;
        case 1:
            ADC0_SSCTL1_R = ADC_SSCTL1_END0;
            break;
        case 2:
            ADC0_SSCTL2_R = ADC_SSCTL2_END0;
            break;
        case 3:
            ADC0_SSCTL3_R = ADC_SSCTL3_END0;
            break;
        }
        ADC0_SSCTL3_R = 0x2;                             // mark first sample as the end
        ADC0_ACTSS_R |= (1 << SS);                       // enable SS[SS] for operation
        break;
    case 1:
        ADC1_ACTSS_R &= ~(1 << SS);                      // disable sample sequencer SS (SS[SS]) for programming
        ADC1_CC_R = ADC_CC_CS_SYSPLL;                    // select PLL as the time base (not needed, since default value)
        ADC1_PC_R = ADC_PC_SR_1M;                        // select 1Msps rate
        ADC1_EMUX_R = 0x0;                               // select SS[SS] bit in ADCPSSI as trigger
        switch (SS)
        {
        case 0:
            ADC1_SSCTL0_R = ADC_SSCTL0_END0;
            break;
        case 1:
            ADC1_SSCTL1_R = ADC_SSCTL1_END0;
            break;
        case 2:
            ADC1_SSCTL2_R = ADC_SSCTL2_END0;
            break;
        case 3:
            ADC1_SSCTL3_R = ADC_SSCTL3_END0;
            break;
        }
        ADC1_SSCTL3_R = 0x2;                             // mark first sample as the end
        ADC1_ACTSS_R |= (1 << SS);                       // enable SS[SS] for operation
        break;
    }
}

void setADCSSLog2AverageCount(uint8_t ADC, uint8_t SS, uint8_t log2AverageCount, uint32_t ADC_CTL_DITHER)
{
    if (!(ADC < 2 && SS < 4))
            return;

    switch (ADC)
    {
    case 0:
        ADC0_ACTSS_R &= ~(1 << SS);                      // disable sample sequencer SS (SS[SS]) for programming
        ADC0_SAC_R = log2AverageCount;                   // sample HW averaging
        if (log2AverageCount == 0)
            ADC0_CTL_R &= ~ADC_CTL_DITHER;               // turn-off dithering if no averaging
        else
            ADC0_CTL_R |= ADC_CTL_DITHER;                // turn-on dithering if averaging
        ADC0_ACTSS_R |= (1 << SS);                       // enable SS[SS] for operation
        break;
    case 1:
        ADC1_ACTSS_R &= ~(1 << SS);                      // disable sample sequencer SS (SS[SS]) for programming
        ADC1_SAC_R = log2AverageCount;                   // sample HW averaging
        if (log2AverageCount == 0)
            ADC1_CTL_R &= ~ADC_CTL_DITHER;               // turn-off dithering if no averaging
        else
            ADC1_CTL_R |= ADC_CTL_DITHER;                // turn-on dithering if averaging
        ADC1_ACTSS_R |= (1 << SS);                       // enable SS[SS] for operation
        break;
    }
}

void setADCSSMux(uint8_t ADC, uint8_t SS, uint8_t input)
{
    if (!(ADC < 2 && SS < 4))
            return;

    switch (ADC)
    {
    case 0:
        ADC0_ACTSS_R &= ~(1 << SS);                      // disable sample sequencer SS (SS[SS]) for programming
        switch (SS)
        {
        case 0:
            ADC0_SSMUX0_R = input;                       // Set analog input for single sample
            break;
        case 1:
            ADC0_SSMUX1_R = input;                       // Set analog input for single sample
            break;
        case 2:
            ADC0_SSMUX2_R = input;                       // Set analog input for single sample
            break;
        case 3:
            ADC0_SSMUX3_R = input;                       // Set analog input for single sample
            break;
        }
        ADC0_ACTSS_R |= (1 << SS);                       // enable SS[SS] for operation
        break;
    case 1:
        ADC1_ACTSS_R &= ~(1 << SS);                      // disable sample sequencer SS (SS[SS]) for programming
        switch (SS)
        {
        case 0:
            ADC1_SSMUX0_R = input;                       // Set analog input for single sample
            break;
        case 1:
            ADC1_SSMUX1_R = input;                       // Set analog input for single sample
            break;
        case 2:
            ADC1_SSMUX2_R = input;                       // Set analog input for single sample
            break;
        case 3:
            ADC1_SSMUX3_R = input;                       // Set analog input for single sample
            break;
        }
        ADC1_ACTSS_R |= (1 << SS);                       // enable SS[SS] for operation
        break;
    }
}

int16_t readADCSS(uint8_t ADC, uint8_t SS)
{
    if (!(ADC < 2 && SS < 4))
            return 0;

    switch (ADC)
    {
    case 0:
        ADC0_PSSI_R |= (1 << SS);                            // set start bit
        while (ADC0_ACTSS_R & ADC_ACTSS_BUSY);               // wait until SS[SS] is not busy
        switch (SS)
        {
        case 0:
            while (ADC0_SSFSTAT0_R & ADC_SSFSTAT0_EMPTY);
            return ADC0_SSFIFO0_R;                           // get single result from the FIFO
        case 1:
            while (ADC0_SSFSTAT1_R & ADC_SSFSTAT1_EMPTY);
            return ADC0_SSFIFO1_R;                           // get single result from the FIFO
        case 2:
            while (ADC0_SSFSTAT2_R & ADC_SSFSTAT2_EMPTY);
            return ADC0_SSFIFO2_R;                           // get single result from the FIFO
        case 3:
            while (ADC0_SSFSTAT3_R & ADC_SSFSTAT3_EMPTY);
            return ADC0_SSFIFO3_R;                           // get single result from the FIFO
        }
        break;
    case 1:
        ADC1_PSSI_R |= (1 << SS);                            // set start bit
        while (ADC1_ACTSS_R & ADC_ACTSS_BUSY);               // wait until SS[SS] is not busy
        switch (SS)
        {
        case 0:
            while (ADC1_SSFSTAT0_R & ADC_SSFSTAT0_EMPTY);
            return ADC1_SSFIFO0_R;                           // get single result from the FIFO
        case 1:
            while (ADC1_SSFSTAT1_R & ADC_SSFSTAT1_EMPTY);
            return ADC1_SSFIFO1_R;                           // get single result from the FIFO
        case 2:
            while (ADC1_SSFSTAT2_R & ADC_SSFSTAT2_EMPTY);
            return ADC1_SSFIFO2_R;                           // get single result from the FIFO
        case 3:
            while (ADC1_SSFSTAT3_R & ADC_SSFSTAT3_EMPTY);
            return ADC1_SSFIFO3_R;                           // get single result from the FIFO
        }
        break;
    }
    return 0;
}
