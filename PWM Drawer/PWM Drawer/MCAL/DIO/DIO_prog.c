#include "../../Service/std_types.h"
#include "../../Service/bit_math.h"
#include "../reg_def.h"
#include "DIO_interface.h"

/* Set individual pin direction (input/output) */
void DIO_voidSetPinDirection(u8 Copy_u8PortID, u8 Copy_u8PinID, u8 Copy_u8Dir)
{
	// Check if pin number is valid (0-7)
	if(Copy_u8PinID <=7)
	{
		// Check if direction is output
		if(Copy_u8Dir == DIO_PIN_OUTPUT)
		{
			// Set corresponding bit in DDR register based on port
			switch(Copy_u8PortID)
			{
				case DIO_PORTA: SET_BIT(DIO_DDRA_REG, Copy_u8PinID); break;  // Set Port A pin as output
				case DIO_PORTB: SET_BIT(DIO_DDRB_REG, Copy_u8PinID); break;  // Set Port B pin as output
				case DIO_PORTC: SET_BIT(DIO_DDRC_REG, Copy_u8PinID); break;  // Set Port C pin as output
				case DIO_PORTD: SET_BIT(DIO_DDRD_REG, Copy_u8PinID); break;  // Set Port D pin as output
				default: break; // Error handling for invalid port
			}
		}
		// Check if direction is input
		else if (Copy_u8Dir == DIO_PIN_INPUT)
		{
			// Clear corresponding bit in DDR register based on port
			switch(Copy_u8PortID)
			{
				case DIO_PORTA: CLR_BIT(DIO_DDRA_REG, Copy_u8PinID); break;  // Set Port A pin as input
				case DIO_PORTB: CLR_BIT(DIO_DDRB_REG, Copy_u8PinID); break;  // Set Port B pin as input
				case DIO_PORTC: CLR_BIT(DIO_DDRC_REG, Copy_u8PinID); break;  // Set Port C pin as input
				case DIO_PORTD: CLR_BIT(DIO_DDRD_REG, Copy_u8PinID); break;  // Set Port D pin as input
				default: break; // Error handling for invalid port
			}
		}
		else
		{
			// Error handling for invalid direction
		}
	}
	else
	{
		// Error handling for invalid pin number
	}
}

/* Set individual pin value (high/low) */
void DIO_voidSetPinValue(u8 Copy_u8PortID, u8 Copy_u8PinID, u8 Copy_u8Val)
{
	// Check if pin number is valid (0-7)
	if(Copy_u8PinID <=7)
	{
		// Check if value is high
		if(Copy_u8Val == DIO_PIN_HIGH)
		{
			// Set corresponding bit in PORT register based on port
			switch(Copy_u8PortID)
			{
				case DIO_PORTA: SET_BIT(DIO_PORTA_REG, Copy_u8PinID); break;  // Set Port A pin high
				case DIO_PORTB: SET_BIT(DIO_PORTB_REG, Copy_u8PinID); break;  // Set Port B pin high
				case DIO_PORTC: SET_BIT(DIO_PORTC_REG, Copy_u8PinID); break;  // Set Port C pin high
				case DIO_PORTD: SET_BIT(DIO_PORTD_REG, Copy_u8PinID); break;  // Set Port D pin high
				default: break; // Error handling for invalid port
			}
		}
		// Check if value is low
		else if (Copy_u8Val == DIO_PIN_LOW)
		{
			// Clear corresponding bit in PORT register based on port
			switch(Copy_u8PortID)
			{
				case DIO_PORTA: CLR_BIT(DIO_PORTA_REG, Copy_u8PinID); break;  // Set Port A pin low
				case DIO_PORTB: CLR_BIT(DIO_PORTB_REG, Copy_u8PinID); break;  // Set Port B pin low
				case DIO_PORTC: CLR_BIT(DIO_PORTC_REG, Copy_u8PinID); break;  // Set Port C pin low
				case DIO_PORTD: CLR_BIT(DIO_PORTD_REG, Copy_u8PinID); break;  // Set Port D pin low
				default: break; // Error handling for invalid port
			}
		}
		else
		{
			// Error handling for invalid value
		}
	}
	else
	{
		// Error handling for invalid pin number
	}
}

/* Set entire port direction */
void DIO_voidSetPortDirection(u8 Copy_u8PortID, u8 Copy_u8Dir)
{
	// Set entire DDR register based on port
	switch(Copy_u8PortID)
	{
		case DIO_PORTA: DIO_DDRA_REG = Copy_u8Dir; break;  // Set Port A direction
		case DIO_PORTB: DIO_DDRB_REG = Copy_u8Dir; break;  // Set Port B direction
		case DIO_PORTC: DIO_DDRC_REG = Copy_u8Dir; break;  // Set Port C direction
		case DIO_PORTD: DIO_DDRD_REG = Copy_u8Dir; break;  // Set Port D direction
	}
}

/* Set entire port value */
void DIO_voidSetPortValue(u8 Copy_u8PortID, u8 Copy_u8Val)
{
	// Set entire PORT register based on port
	switch(Copy_u8PortID)
	{
		case DIO_PORTA: DIO_PORTA_REG = Copy_u8Val; break;  // Set Port A value
		case DIO_PORTB: DIO_PORTB_REG = Copy_u8Val; break;  // Set Port B value
		case DIO_PORTC: DIO_PORTC_REG = Copy_u8Val; break;  // Set Port C value
		case DIO_PORTD: DIO_PORTD_REG = Copy_u8Val; break;  // Set Port D value
	}
}

/* Read individual pin value */
u8 DIO_u8GetPinValue(u8 Copy_u8PortID, u8 Copy_u8PinID)
{
	u8 local_u8data = 0;  // Variable to store pin value
	
	// Check if pin number is valid (0-7)
	if(Copy_u8PinID <= 7)
	{
		// Read corresponding bit from PIN register based on port
		switch(Copy_u8PortID)
		{
			case DIO_PORTA: local_u8data = GET_BIT(DIO_PINA_REG, Copy_u8PinID); break;  // Read Port A pin
			case DIO_PORTB: local_u8data = GET_BIT(DIO_PINB_REG, Copy_u8PinID); break;  // Read Port B pin
			case DIO_PORTC: local_u8data = GET_BIT(DIO_PINC_REG, Copy_u8PinID); break;  // Read Port C pin
			case DIO_PORTD: local_u8data = GET_BIT(DIO_PIND_REG, Copy_u8PinID); break;  // Read Port D pin
			default: break;  // Error handling for invalid port
		}
	}
	else
	{
		// Error handling for invalid pin number
	}
	return local_u8data;  // Return pin value (0 or 1)
}

/* Read entire port value */
u8 DIO_u8GetPortValue(u8 Copy_u8PortID)
{
	u8 local_u8portData = 0;  // Variable to store port value
	
	// Read entire PIN register based on port
	switch(Copy_u8PortID)
	{
		case DIO_PORTA: local_u8portData = DIO_PINA_REG; break;  // Read Port A
		case DIO_PORTB: local_u8portData = DIO_PINB_REG; break;  // Read Port B
		case DIO_PORTC: local_u8portData = DIO_PINC_REG; break;  // Read Port C
		case DIO_PORTD: local_u8portData = DIO_PIND_REG; break;  // Read Port D
		default: break;  // Error handling for invalid port
	}
	return local_u8portData;  // Return port value (0-255)
}

/* Enable internal pull-up resistor for pin */
void DIO_voidEnablePullUp(u8 Copy_u8PortID, u8 Copy_u8PinID)
{
	// Check if pin number is valid (0-7)
	if(Copy_u8PinID <= 7)
	{
		// Set corresponding bit in PORT register to enable pull-up
		switch(Copy_u8PortID)
		{
			case DIO_PORTA: SET_BIT(DIO_PORTA_REG, Copy_u8PinID); break;  // Enable Port A pull-up
			case DIO_PORTB: SET_BIT(DIO_PORTB_REG, Copy_u8PinID); break;  // Enable Port B pull-up
			case DIO_PORTC: SET_BIT(DIO_PORTC_REG, Copy_u8PinID); break;  // Enable Port C pull-up
			case DIO_PORTD: SET_BIT(DIO_PORTD_REG, Copy_u8PinID); break;  // Enable Port D pull-up
		}
	}
	else
	{
		// Error handling for invalid pin number
	}
}

/* Toggle individual pin value */
void DIO_voidTogPinValue(u8 Copy_u8PortID, u8 Copy_u8PinID)
{
	// Check if pin number is valid (0-7)
	if(Copy_u8PinID <= 7)
	{
		// Toggle corresponding bit in PORT register based on port
		switch(Copy_u8PortID)
		{
			case DIO_PORTA: TOG_BIT(DIO_PORTA_REG, Copy_u8PinID); break;  // Toggle Port A pin
			case DIO_PORTB: TOG_BIT(DIO_PORTB_REG, Copy_u8PinID); break;  // Toggle Port B pin
			case DIO_PORTC: TOG_BIT(DIO_PORTC_REG, Copy_u8PinID); break;  // Toggle Port C pin
			case DIO_PORTD: TOG_BIT(DIO_PORTD_REG, Copy_u8PinID); break;  // Toggle Port D pin
		}
	}
	else
	{
		// Error handling for invalid pin number
	}
}