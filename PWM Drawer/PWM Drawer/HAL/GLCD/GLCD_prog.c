/* 
   KS0108 128x64 Graphical LCD Driver
*/

#include <util/delay.h>
#include <stdio.h>
#include "../../Service/bit_math.h"
#include "../../Service/std_types.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "GLCD_cfg.h"
#include "GLCD_priv.h"
#include "GLCD_int.h"

/* 5x7 Font table - ASCII characters from space (32) to Z (90) */
const u8 font5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, {0x00,0x00,0x5F,0x00,0x00}, {0x00,0x07,0x00,0x07,0x00},
    {0x14,0x7F,0x14,0x7F,0x14}, {0x24,0x2A,0x7F,0x2A,0x12}, {0x23,0x13,0x08,0x64,0x62},
    {0x36,0x49,0x55,0x22,0x50}, {0x00,0x05,0x03,0x00,0x00}, {0x00,0x1C,0x22,0x41,0x00},
    {0x00,0x41,0x22,0x1C,0x00}, {0x14,0x08,0x3E,0x08,0x14}, {0x08,0x08,0x3E,0x08,0x08},
    {0x00,0x50,0x30,0x00,0x00}, {0x08,0x08,0x08,0x08,0x08}, {0x00,0x60,0x60,0x00,0x00},
    {0x20,0x10,0x08,0x04,0x02}, {0x3E,0x51,0x49,0x45,0x3E}, {0x00,0x42,0x7F,0x40,0x00},
    {0x42,0x61,0x51,0x49,0x46}, {0x21,0x41,0x45,0x4B,0x31}, {0x18,0x14,0x12,0x7F,0x10},
    {0x27,0x45,0x45,0x45,0x39}, {0x3C,0x4A,0x49,0x49,0x30}, {0x01,0x71,0x09,0x05,0x03},
    {0x36,0x49,0x49,0x49,0x36}, {0x06,0x49,0x49,0x29,0x1E}, {0x00,0x36,0x36,0x00,0x00},
    {0x00,0x56,0x36,0x00,0x00}, {0x08,0x14,0x22,0x41,0x00}, {0x14,0x14,0x14,0x14,0x14},
    {0x00,0x41,0x22,0x14,0x08}, {0x02,0x01,0x51,0x09,0x06}, {0x32,0x49,0x79,0x41,0x3E},
    {0x7E,0x11,0x11,0x11,0x7E}, {0x7F,0x49,0x49,0x49,0x36}, {0x3E,0x41,0x41,0x41,0x22},
    {0x7F,0x41,0x41,0x22,0x1C}, {0x7F,0x49,0x49,0x49,0x41}, {0x7F,0x09,0x09,0x09,0x01},
    {0x3E,0x41,0x49,0x49,0x7A}, {0x7F,0x08,0x08,0x08,0x7F}, {0x00,0x41,0x7F,0x41,0x00},
    {0x20,0x40,0x41,0x3F,0x01}, {0x7F,0x08,0x14,0x22,0x41}, {0x7F,0x40,0x40,0x40,0x40},
    {0x7F,0x02,0x0C,0x02,0x7F}, {0x7F,0x04,0x08,0x10,0x7F}, {0x3E,0x41,0x41,0x41,0x3E},
    {0x7F,0x09,0x09,0x09,0x06}, {0x3E,0x41,0x51,0x21,0x5E}, {0x7F,0x09,0x19,0x29,0x46},
    {0x26,0x49,0x49,0x49,0x32}, {0x01,0x01,0x7F,0x01,0x01}, {0x3F,0x40,0x40,0x40,0x3F},
    {0x1F,0x20,0x40,0x20,0x1F}, {0x7F,0x20,0x18,0x20,0x7F}, {0x63,0x14,0x08,0x14,0x63},
    {0x07,0x08,0x70,0x08,0x07}, {0x61,0x51,0x49,0x45,0x43},
};

/* Global variables for cursor position tracking */
static u8 current_page = 0;  // Current page (0-7)
static u8 current_col = 0;   // Current column (0-127)

/* Send command to GLCD controller */
void GLCD_voidCommand(u8 cmd, u8 cs)
{
    // Set RS low for command mode
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RS_PIN, 0);
    // Set RW low for write mode
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RW_PIN, 0);
    // Output command on data port
    DIO_voidSetPortValue(GLCD_DATA_PORT, cmd);

    // Select appropriate chip (CS1 for left, CS2 for right)
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, (cs == 1) ? 1 : 0);
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, (cs == 2) ? 1 : 0);

    // Generate enable pulse
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, 1);
    _delay_us(5);  // Wait for setup time
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, 0);

    // Deselect both chips
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, 0);
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, 0);
    _delay_us(5);  // Wait for hold time
}

/* Write data to GLCD display RAM */
void GLCD_voidWriteData(u8 data, u8 cs)
{
    // Set RS high for data mode
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RS_PIN, 1);
    // Set RW low for write mode
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RW_PIN, 0);
    // Output data on data port
    DIO_voidSetPortValue(GLCD_DATA_PORT, data);

    // Select appropriate chip
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, (cs == 1) ? 1 : 0);
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, (cs == 2) ? 1 : 0);

    // Generate enable pulse
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, 1);
    _delay_us(5);  // Wait for setup time
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, 0);

    // Deselect both chips
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, 0);
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, 0);
    _delay_us(5);  // Wait for hold time
}

/* Initialize GLCD hardware and controller */
void GLCD_voidInit(void)
{
    // Configure data port as output
    DIO_voidSetPortDirection(GLCD_DATA_PORT, 0xFF);
    // Configure control pins as output
    DIO_voidSetPinDirection(GLCD_CTRL_PORT, GLCD_RS_PIN, 1);
    DIO_voidSetPinDirection(GLCD_CTRL_PORT, GLCD_RW_PIN, 1);
    DIO_voidSetPinDirection(GLCD_CTRL_PORT, GLCD_EN_PIN, 1);
    DIO_voidSetPinDirection(GLCD_CTRL_PORT, GLCD_CS1_PIN, 1);
    DIO_voidSetPinDirection(GLCD_CTRL_PORT, GLCD_CS2_PIN, 1);
    DIO_voidSetPinDirection(GLCD_CTRL_PORT, GLCD_RST_PIN, 1);

    // Reset sequence
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RST_PIN, 0);
    _delay_ms(10);  // Hold reset low
    DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RST_PIN, 1);
    _delay_ms(50);  // Wait for stabilization

    // Turn on both display halves
    GLCD_voidCommand(0x3F, 1);  // Display on for chip 1
    GLCD_voidCommand(0x3F, 2);  // Display on for chip 2
    _delay_ms(100);  // Wait for display ready

    // Clear display and reset cursor
    GLCD_voidClear();
}

/* Set cursor position on display */
void GLCD_voidGotoXY(u8 x, u8 y)
{
    // Update global position variables
    current_page = x;
    current_col = y;

    // Determine which chip to use based on column
    u8 chip = (y < 64) ? 1 : 2;
    // Calculate local column within chip (0-63)
    u8 local_col = (y < 64) ? y : (y - 64);

    // Send position commands to appropriate chip
    GLCD_voidCommand(0xB8 | x, chip);      // Set page
    GLCD_voidCommand(0x40 | local_col, chip); // Set column
}

/* Display single character at current position */
void GLCD_voidDisplayChar(char c)
{
    // Check if character is in valid range (space to Z)
    if (c < 32 || c > 90) return;

    // Calculate font table index
    u8 idx = c - 32;
    
    /* Write each of 5 font bytes */
    for (u8 i = 0; i < 5; i++)
    {
        // Determine chip and local column
        u8 chip = (current_col < 64) ? 1 : 2;
        u8 local_col = (current_col < 64) ? current_col : (current_col - 64);
        
        /* Set position before each write */
        GLCD_voidCommand(0xB8 | current_page, chip);
        GLCD_voidCommand(0x40 | local_col, chip);
        
        /* Write the font byte */
        GLCD_voidWriteData(font5x7[idx][i], chip);
        
        // Move to next column
        current_col++;
        
        /* Handle chip boundary crossing */
        if (current_col == 64)
        {
            /* Moving from chip 1 to chip 2 */
            GLCD_voidCommand(0xB8 | current_page, 2);
            GLCD_voidCommand(0x40 | 0, 2);
        }
    }

    /* Space between characters */
    u8 chip = (current_col < 64) ? 1 : 2;
    u8 local_col = (current_col < 64) ? current_col : (current_col - 64);
    
    // Set position for space
    GLCD_voidCommand(0xB8 | current_page, chip);
    GLCD_voidCommand(0x40 | local_col, chip);
    // Write space (blank column)
    GLCD_voidWriteData(0x00, chip);
    
    // Move to next column after space
    current_col++;
    
    /* Handle chip boundary crossing */
    if (current_col == 64)
    {
        GLCD_voidCommand(0xB8 | current_page, 2);
        GLCD_voidCommand(0x40 | 0, 2);
    }
    
    /* Handle page wrap */
    if (current_col >= 128)
    {
        current_col = 0;
        current_page++;
        if (current_page >= 8) current_page = 0;
    }
}

/* Display string starting at current position */
void GLCD_voidDisplayString(u8 *str)
{
    // Display each character until null terminator
    while (*str)
    {
        GLCD_voidDisplayChar((char)*str);
        str++;
    }
}

/* Display number at current position */
void GLCD_voidDisplayNumber(u32 num)
{
    char buf[12];
    // Convert number to string
    sprintf(buf, "%lu", num);
    // Display the string
    GLCD_voidDisplayString((u8*)buf);
}

/* Clear entire display */
void GLCD_voidClear(void)
{
    // Clear all 8 pages
    for (u8 page = 0; page < 8; page++)
    {
        // Clear both chips
        for (u8 chip = 1; chip <= 2; chip++)
        {
            // Set page address
            GLCD_voidCommand(0xB8 | page, chip);
            // Set column to 0
            GLCD_voidCommand(0x40, chip);

            // Clear all 64 columns in this page
            for (u8 col = 0; col < 64; col++)
            {
                GLCD_voidWriteData(0x00, chip);
            }
        }
    }
    // Reset cursor to top-left
    current_page = 0;
    current_col = 0;
}

/* Read data from GLCD display RAM */
uint8_t GLCD_u8ReadData(u8 cs)
{
	// Set RS high for data mode
	DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RS_PIN, 1);
	// Set RW high for read mode
	DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_RW_PIN, 1);
	
	// Set data port pins as INPUT (read mode)
	DIO_voidSetPortDirection(GLCD_DATA_PORT, 0x00);
	
	// Select appropriate chip
	DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, (cs == 1) ? 1 : 0);
	DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, (cs == 2) ? 1 : 0);
	
	// Generate enable pulse
	DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, 1);
	_delay_us(5);
	uint8_t data = DIO_u8GetPortValue(GLCD_DATA_PORT);
	DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_EN_PIN, 0);
	
	// Set data port back to OUTPUT
	DIO_voidSetPortDirection(GLCD_DATA_PORT, 0xFF);
	
	// Deselect both chips
	DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS1_PIN, 0);
	DIO_voidSetPinValue(GLCD_CTRL_PORT, GLCD_CS2_PIN, 0);
	_delay_us(5);
	
	return data;
}
