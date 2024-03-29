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

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

void initDisp(void);
void clearDisplay();
void sendData(bool rs, bool rw, uint8_t data);
void writeDisplay(char * data);
void writeDisplayLine(uint8_t line, char * data);
void setCursour(uint8_t x, uint8_t y);
void shiftDisplay(bool sc, bool rl);
uint8_t readData(bool rs);
void waitBusy();
void testDisplay();
void setCharacter(uint8_t n, uint8_t c[]);

#endif
