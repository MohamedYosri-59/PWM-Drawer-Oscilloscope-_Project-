#include "../../Service/std_types.h"

#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_

/* DIO (Digital Input/Output) Configuration Constants */

// Port identifiers
#define DIO_PORTA 0  // Port A identifier
#define DIO_PORTB 1  // Port B identifier
#define DIO_PORTC 2  // Port C identifier
#define DIO_PORTD 3  // Port D identifier

// Pin numbers (0-7)
#define DIO_PIN_0 0  // Pin 0
#define DIO_PIN_1 1  // Pin 1
#define DIO_PIN_2 2  // Pin 2
#define DIO_PIN_3 3  // Pin 3
#define DIO_PIN_4 4  // Pin 4
#define DIO_PIN_5 5  // Pin 5
#define DIO_PIN_6 6  // Pin 6
#define DIO_PIN_7 7  // Pin 7

// Pin direction settings
#define DIO_PIN_OUTPUT 1  // Set pin as output
#define DIO_PIN_INPUT 0   // Set pin as input

// Pin value settings
#define DIO_PIN_HIGH 1  // Set pin to high (5V)
#define DIO_PIN_LOW 0   // Set pin to low (0V)

// Port value settings
#define DIO_PORT_HIGH 0XFF  // Set all port pins high
#define DIO_PORT_LOW 0X00   // Set all port pins low

// Port direction settings
#define DIO_PORT_OUTPUT 0XFF  // Set all port pins as output
#define DIO_PORT_INPUT 0X00   // Set all port pins as input

/* Function Prototypes for DIO Operations */

// Set individual pin direction (input/output)
void DIO_voidSetPinDirection(u8 Copy_u8PortID, u8 Copy_u8PinID, u8 Copy_u8Dir);

// Set individual pin value (high/low)
void DIO_voidSetPinValue(u8 Copy_u8PortID, u8 Copy_u8PinID, u8 Copy_u8Val);

// Set entire port direction
void DIO_voidSetPortDirection(u8 Copy_u8PortID, u8 Copy_u8Dir);

// Set entire port value
void DIO_voidSetPortValue(u8 Copy_u8PortID, u8 Copy_u8Val);

// Read individual pin value
u8 DIO_u8GetPinValue(u8 Copy_u8PortID, u8 Copy_u8PinID);

// Read entire port value
u8 DIO_u8GetPortValue(u8 Copy_u8PortID);

// Enable internal pull-up resistor for pin
void DIO_voidEnablePullUp(u8 Copy_u8PortID, u8 Copy_u8PinID);

// Toggle individual pin value
void DIO_voidTogPinValue(u8 Copy_u8PortID, u8 Copy_u8PinID);

#endif /* DIO_INTERFACE_H_ */