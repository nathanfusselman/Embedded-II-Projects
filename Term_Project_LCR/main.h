// Term Project
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

#ifndef MAIN_H_
#define MAIN_H_

#define PROCESSOR_FREQUENCY 80000000

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "main.h"
#include "clock.h"
#include "gpio.h"
#include "wait.h"
#include "uart0.h"
#include "lcr.h"
#include "display.h"
#include "buttons.h"

typedef enum mode {Norm,Test} MODE;

typedef enum status {STOPPED,TESTING} STATUS;

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

void initHw(void);
void handleButtonPressed(uint8_t buttonNum);
void toHomePage();
void toTestPage();
uint8_t printWaiting(uint8_t num);
void handleMeasure(TYPE test);

#endif
