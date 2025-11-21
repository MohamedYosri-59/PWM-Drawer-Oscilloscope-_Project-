#ifndef BIT_MATH_H_
#define BIT_MATH_H_

/* Bit manipulation macros for AVR microcontroller */
// Set specific bit in register
#define SET_BIT(reg, bit) reg|=(1<<bit)
// Clear specific bit in register
#define CLR_BIT(reg, bit) reg&=~(1<<bit)
// Toggle specific bit in register
#define TOG_BIT(reg, bit) reg^=(1<<bit)
// Get specific bit value from register
#define GET_BIT(reg, bit) (reg>>bit)&1

#endif /* BIT_MATH_H_ */