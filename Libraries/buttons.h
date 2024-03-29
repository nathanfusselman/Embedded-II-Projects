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

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdint.h>
#include <stdbool.h>

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

void initButtons(void);
void enableListen(uint8_t num);
void disableListen(uint8_t num);
void enableAllListen(void);
void disableAllListen(void);
uint8_t getButtonPressed(void);

#endif
