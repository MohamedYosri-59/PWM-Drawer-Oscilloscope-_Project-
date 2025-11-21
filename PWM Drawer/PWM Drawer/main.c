/*
 * PWM Analyzer with GLCD waveform display
 * CPU = 16 MHz, Timer1 prescaler = 8
 * Measures PWM frequency, duty cycle, and period
 * Displays waveform graphically and parameters textually
 */
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#include "Service/std_types.h"
#include "Service/bit_math.h"
#include "MCAL/DIO/DIO_interface.h"
#include "HAL/GLCD/GLCD_int.h"

/* ---------------------- Global Variables ---------------------- */
volatile uint16_t last_rise = 0;
volatile uint16_t high_start = 0;
volatile uint16_t pulse_high_ticks = 0;
volatile uint16_t period_ticks = 0;
volatile uint8_t new_measurement = 0;
volatile uint8_t waiting_for_fall = 0;

/* ---------------------- Function Prototypes ---------------------- */
void Timer1_InputCapture_Init(void);
void Draw_Waveform(float duty, float freq);
void Clear_TextArea(void);

/* ---------------------- Interrupt Service Routine ---------------------- */
ISR(TIMER1_CAPT_vect)
{
    static uint16_t rise, fall;

    if (TCCR1B & (1 << ICES1))   // Rising edge detected
    {
        rise = ICR1;
        period_ticks = rise - last_rise;   // time between rising edges
        last_rise = rise;

        // Switch to detect falling edge for high pulse width
        TCCR1B &= ~(1 << ICES1);
        waiting_for_fall = 1;
    }
    else if (waiting_for_fall)   // Falling edge detected
    {
        fall = ICR1;
        pulse_high_ticks = fall - last_rise;
        waiting_for_fall = 0;
        TCCR1B |= (1 << ICES1);   // Back to rising edge
        new_measurement = 1;
    }
}

/* ---------------------- Timer1 Input Capture Init ---------------------- */
void Timer1_InputCapture_Init(void)
{
    TCCR1A = 0;
    TCCR1B = (1 << ICES1) | (1 << CS11); // capture rising edge, prescaler=8
    TIMSK |= (1 << TICIE1);
    sei();
}

/* ---------------------- Main Function ---------------------- */
int main(void)
{
    char buf[32];
    uint8_t duty_cycle = 0;

    GLCD_voidInit();
    Timer1_InputCapture_Init();

    GLCD_voidGotoXY(0, 0);
    GLCD_voidDisplayString((uint8_t *)"PWM ANALYZER");
    _delay_ms(1000);
    GLCD_voidClear();

    while (1)
    {
        if (new_measurement)
        {
            new_measurement = 0;

            /* ----- Compute Frequency & Duty Cycle ----- */
            uint32_t freq_hz = 0;
            if (period_ticks > 0)
                freq_hz = (2000000UL / period_ticks); // 16MHz/8 = 2MHz tick rate

            if (period_ticks > 0)
            {
                duty_cycle = (uint8_t)((pulse_high_ticks * 100UL) / period_ticks);
                if (duty_cycle > 100) duty_cycle = 100;
            }

            /* ----- Compute Period ----- */
            uint32_t period_us = (uint32_t)(period_ticks * 0.5); // 0.5 µs per tick
            uint32_t period_ms_int = period_us / 1000;
            uint16_t period_ms_dec = period_us % 1000;

            /* ----- Display Results (clear only text area) ----- */
            Clear_TextArea();

            uint32_t freq_khz_int = freq_hz / 1000;
            uint16_t freq_khz_dec = freq_hz % 1000;

            GLCD_voidGotoXY(0, 0);
            sprintf(buf, "FREQ=%lu.%03uKHZ", freq_khz_int, freq_khz_dec);
            GLCD_voidDisplayString((uint8_t *)buf);

            GLCD_voidGotoXY(1, 80);
            sprintf(buf, "DUTY=%u%%", duty_cycle);
            GLCD_voidDisplayString((uint8_t *)buf);

            GLCD_voidGotoXY(2, 0);
            sprintf(buf, "TIME=%lu.%03uMS", period_ms_int, period_ms_dec);
            GLCD_voidDisplayString((uint8_t *)buf);

            //GLCD_voidGotoXY(3, 0);
            //sprintf(buf, "TICKS=%lu", (uint32_t)period_ticks);
            //GLCD_voidDisplayString((uint8_t *)buf);

            /* Draw waveform */
            Draw_Waveform(duty_cycle, freq_hz);
        }
    }
}

/* ---------------------- Draw Waveform ---------------------- */
void Draw_Waveform(float duty, float freq)
{
	static uint8_t buffer[128] = {0}; // waveform level per column
	static uint8_t t = 0;             // time index for PWM shape

	const uint8_t y_top = 40;
	const uint8_t y_bottom = 63;
	//const uint8_t height = y_bottom - y_top + 1;
	const uint8_t period_px = 50;

	// Calculate on-time pixels for PWM
	uint8_t high_px = (uint8_t)(period_px * (duty / 100.0));

	// Generate next PWM point (1 pixel wide)
	uint8_t bit_val = (t < high_px) ? 1 : 0;
	t = (t + 1) % period_px;

	// Scroll buffer left and add new bit
	for (uint8_t i = 0; i < 127; i++) buffer[i] = buffer[i + 1];
	buffer[127] = bit_val;

	// Clear waveform area (pages 5–7)
	for (uint8_t page = 5; page < 8; page++) {
		GLCD_voidGotoXY(page, 0);
		for (uint8_t col = 0; col < 64; col++) GLCD_voidWriteData(0x00, 1);
		GLCD_voidGotoXY(page, 64);
		for (uint8_t col = 0; col < 64; col++) GLCD_voidWriteData(0x00, 2);
	}

	// Draw waveform (continuous thick line with vertical connections)
	for (uint8_t x = 0; x < 128; x++) {
		uint8_t y_current = buffer[x] ? y_top : y_bottom;  // current waveform level
		uint8_t y_prev = (x > 0) ? (buffer[x-1] ? y_top : y_bottom) : y_current; // previous level

		// Draw horizontal line (2 pixels thick)
		for (int dy = 0; dy < 2; dy++) {
			uint8_t yy = y_current + dy;
			if (yy > y_bottom) break;

			uint8_t page = yy / 8;
			uint8_t bit = yy % 8;
			uint8_t chip = (x < 64) ? 1 : 2;
			uint8_t local_col = (x < 64) ? x : (x - 64);

			GLCD_voidCommand(0xB8 | page, chip);
			GLCD_voidCommand(0x40 | local_col, chip);
			GLCD_voidWriteData(1 << bit, chip);
		}

// Draw vertical connecting line when level changes - CONTINUOUS VERSION
if (x > 0 && y_current != y_prev) {
	// Draw vertical line with 2-3 pixel width
	for (int dx = 0; dx < 2; dx++) {
		uint8_t xx = x - dx;
		
		// Draw from y_top to y_bottom
		for (uint8_t y_vert = y_top; y_vert <= y_bottom; y_vert++) {
			uint8_t page = y_vert / 8;
			uint8_t bit = y_vert % 8;
			uint8_t chip = (xx < 64) ? 1 : 2;
			uint8_t local_col = (xx < 64) ? xx : (xx - 64);
			
			// Set position
			GLCD_voidCommand(0xB8 | page, chip);
			GLCD_voidCommand(0x40 | local_col, chip);
			
			// Read current byte value
			uint8_t current_byte = GLCD_u8ReadData(chip);
			
			// Set the bit for this pixel
			current_byte |= (1 << bit);
			
			// Write back the modified byte
			GLCD_voidCommand(0xB8 | page, chip);
			GLCD_voidCommand(0x40 | local_col, chip);
			GLCD_voidWriteData(current_byte, chip);
		}
	}
}
	}

	_delay_ms(10); // control waveform speed
}

/* ---------------------- Clear Only Text Area ---------------------- */
void Clear_TextArea(void)
{
    for (uint8_t page = 0; page < 4; page++)
    {
        for (uint8_t chip = 1; chip <= 2; chip++)
        {
            GLCD_voidCommand(0xB8 | page, chip);
            GLCD_voidCommand(0x40, chip);
            for (uint8_t col = 0; col < 64; col++)
                GLCD_voidWriteData(0x00, chip);
        }
    }
}
