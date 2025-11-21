#ifndef GLCD_PRIV_H_
#define GLCD_PRIV_H_

/* GLCD (Graphical LCD) Private Commands - KS0108 Controller */

// Command definitions for KS0108 display controller
#define GLCD_CMD_DISPLAY_ON  0x3F  // Turn display on
#define GLCD_CMD_DISPLAY_OFF 0x3E  // Turn display off
#define GLCD_CMD_SET_Y       0x40  // Set Y address (column) - 0-63
#define GLCD_CMD_SET_X       0xB8  // Set X address (page) - 0-7
#define GLCD_CMD_START_LINE  0xC0  // Set display start line - 0-63

#endif /* GLCD_PRIV_H_ */