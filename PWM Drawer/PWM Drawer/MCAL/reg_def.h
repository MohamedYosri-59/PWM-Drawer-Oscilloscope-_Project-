#ifndef REG_DEF_H_
#define REG_DEF_H_

/*------------------------------ DIO REGISTERS ------------------------------*/
// Port A registers
#define DIO_PINA_REG   (*(volatile u8*)(0x39))  // Port A Input Pins
#define DIO_DDRA_REG   (*(volatile u8*)(0x3A))  // Port A Data Direction Register
#define DIO_PORTA_REG  (*(volatile u8*)(0x3B))  // Port A Data Register

// Port B registers
#define DIO_PINB_REG   (*(volatile u8*)(0x36))  // Port B Input Pins
#define DIO_DDRB_REG   (*(volatile u8*)(0x37))  // Port B Data Direction Register
#define DIO_PORTB_REG  (*(volatile u8*)(0x38))  // Port B Data Register

// Port C registers
#define DIO_PINC_REG   (*(volatile u8*)(0x33))  // Port C Input Pins
#define DIO_DDRC_REG   (*(volatile u8*)(0x34))  // Port C Data Direction Register
#define DIO_PORTC_REG  (*(volatile u8*)(0x35))  // Port C Data Register

// Port D registers
#define DIO_PIND_REG   (*(volatile u8*)(0x30))  // Port D Input Pins
#define DIO_DDRD_REG   (*(volatile u8*)(0x31))  // Port D Data Direction Register
#define DIO_PORTD_REG  (*(volatile u8*)(0x32))  // Port D Data Register

/*------------------------------ GLOBAL INTERRUPT ---------------------------*/
#define SREG_REG (*(volatile u8*)(0x5F))  // Status Register

/*------------------------------ EXTERNAL INTERRUPTS ------------------------*/
// External interrupt control registers
#define EXTI_MCUCR_REG   (*(volatile u8*)(0x55))  // MCU Control Register
#define EXTI_MCUCSR_REG  (*(volatile u8*)(0x54))  // MCU Control and Status Register
#define EXTI_GICR_REG    (*(volatile u8*)(0x5B))  // General Interrupt Control Register
#define EXTI_GIFR_REG    (*(volatile u8*)(0x5A))  // General Interrupt Flag Register

// External interrupt bit definitions
#define EXTI_MCUCR_ISC00 0  // Interrupt Sense Control 0 Bit 0
#define EXTI_MCUCR_ISC01 1  // Interrupt Sense Control 0 Bit 1
#define EXTI_MCUCR_ISC10 2  // Interrupt Sense Control 1 Bit 0
#define EXTI_MCUCR_ISC11 3  // Interrupt Sense Control 1 Bit 1
#define EXTI_MCUCSR_ISC2 6  // Interrupt Sense Control 2
#define EXTI_GICR_INT1   7  // INT1 Enable
#define EXTI_GICR_INT0   6  // INT0 Enable
#define EXTI_GICR_INT2   5  // INT2 Enable
#define EXTI_GIFR_INTF1  7  // INT1 Flag
#define EXTI_GIFR_INTF0  6  // INT0 Flag
#define EXTI_GIFR_INTF2  5  // INT2 Flag

/*------------------------------ ADC REGISTERS ------------------------------*/
// ADC registers
#define ADC_ADMUX_REG   (*(volatile u8*)(0x27))  // ADC Multiplexer Selection Register
#define ADC_ADCSRA_REG  (*(volatile u8*)(0x26))  // ADC Control and Status Register A
#define ADC_ADCH_REG    (*(volatile u8*)(0x25))  // ADC Data Register High
#define ADC_ADCL_REG    (*(volatile u8*)(0x24))  // ADC Data Register Low
#define ADC_ADC_REG     (*(volatile u16*)(0x24)) // ADC Data Register (16-bit access)

// ADC bit definitions
#define ADC_ADMUX_REFS1 7  // Reference Selection Bit 1
#define ADC_ADMUX_REFS0 6  // Reference Selection Bit 0
#define ADC_ADMUX_ADLAR 5  // ADC Left Adjust Result
#define ADC_ADCSRA_ADEN 7  // ADC Enable
#define ADC_ADCSRA_ADSC 6  // ADC Start Conversion

/*------------------------------ TIMER0 REGISTERS ---------------------------*/
// Timer0 registers
#define TIMER0_TCCR0_REG (*(volatile u8*)(0x53))  // Timer0 Control Register
#define TIMER0_TCNT0_REG (*(volatile u8*)(0x52))  // Timer0 Counter Register
#define TIMER0_TIMSK_REG (*(volatile u8*)(0x59))  // Timer0 Interrupt Mask Register
#define TIMER0_TIFR_REG  (*(volatile u8*)(0x58))  // Timer0 Interrupt Flag Register
#define TIMER0_OCR0_REG  (*(volatile u8*)(0x5C))  // Timer0 Output Compare Register

#endif /* REG_DEF_H_ */