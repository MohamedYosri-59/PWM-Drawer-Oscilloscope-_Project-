#ifndef GLCD_INT_H_
#define GLCD_INT_H_

#include "../../Service/std_types.h"

/* GLCD (Graphical LCD) Interface Header */

// External declaration of 5x7 font array
extern const u8 font5x7[][5];

// Function prototypes for GLCD operations

// Initialize GLCD hardware and controller
void GLCD_voidInit(void);

// Clear entire display
void GLCD_voidClear(void);

// Set cursor position (x: 0-7 pages, y: 0-127 columns)
void GLCD_voidGotoXY(u8 x, u8 y);

// Display single character
void GLCD_voidDisplayChar(char c);

// Display string
void GLCD_voidDisplayString(u8 *str);

// Display number
void GLCD_voidDisplayNumber(u32 num);

// Send command to specific GLCD chip
void GLCD_voidCommand(u8 cmd, u8 cs);

// Write data to specific GLCD chip
void GLCD_voidWriteData(u8 data, u8 cs);

/* Read Data Function (to be able to read/write/modify) for vertical lines */
uint8_t GLCD_voidReadData(u8 cs);

#endif