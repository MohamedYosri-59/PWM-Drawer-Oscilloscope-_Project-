#ifndef GLCD_CFG_H_
#define GLCD_CFG_H_

/* GLCD (Graphical LCD) Hardware Configuration - KS0108 128x64 */

// Data port for GLCD data lines (D0-D7)
#define GLCD_DATA_PORT DIO_PORTA

// Control port for GLCD control signals
#define GLCD_CTRL_PORT DIO_PORTB

// Control pin definitions
#define GLCD_RS_PIN DIO_PIN_0   // Register Select (Command/Data)
#define GLCD_RW_PIN DIO_PIN_1   // Read/Write Select
#define GLCD_EN_PIN DIO_PIN_2   // Enable Signal
#define GLCD_CS1_PIN DIO_PIN_3  // Chip Select 1 (Left half: columns 0-63)
#define GLCD_CS2_PIN DIO_PIN_4  // Chip Select 2 (Right half: columns 64-127)
#define GLCD_RST_PIN DIO_PIN_5  // Reset Signal

#endif