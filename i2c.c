/*
 * i2c.c
 *
 * Created on: Apr 17, 2021
 *     Author: Adam Morrison
 */

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <c:/users/adamm/Documents/Atmel Studio/7.0/LED_Matrix_Weather_Display/i2c.h>

void i2c_start() {
	PORTA |= 0x10;							// SCL HIGH
	PORTA |= 0x40;							// SDA HIGH
	USIDR = 0x00;							// set MSB to 0 - SDA LOW
	_delay_ms(1);
	USICR |= USICLK;						// toggle clock
}

void i2c_stop() {
	PORTA &= ~(0x10);						// SCL LOW
	PORTA &= ~(0x40);						// SDA LOW
	_delay_ms(1);
	PORTA |= 0x10;							// SCL HIGH
	_delay_ms(1);							// wait
	PORTA |= 0x40;							// SDA HIGH
	USIDR = 0x80;							// set MSB to 1 - SDA HIGH
}

void i2c_send_data(char data) {
	int i = 0;
	
	for(i = 0; i < 8; i++) {
		if (i == 0) {
			USIDR = data;
		}
		else {
			USICR |= 0x02;					// output data (shift data register left by 1)
		}
		
		_delay_ms(2.5);
		USICR |= USICLK;					// toggle clock
		_delay_ms(5);
		USICR |= USICLK;					// toggle clock
		_delay_ms(2.5);
	}
	USIDR = 0x00;							// set MSB to 0 so that SDA line doesn't go high after ACK/NACK

	_delay_ms(2.5);
	USICR |= USICLK;						// toggle clock
	_delay_ms(5);
	USICR |= USICLK;						// toggle clock
	_delay_ms(2.5);
}

void i2c_send_addr(char addr) {
	char registerVal = USISR;
	
	if (registerVal & 0b10000000) {			// turn on LED to verify start condition detected
		PORTA |= 0x08;
	}

	USISR &= ~(0x80);						// clear start condition flag
	USISR |= 0x40;							// reset counter flag
	USISR &= ~(0x0F);						// reset counter

	int i = 0;
	for(i = 0; i < 8; i++) {
		if (i == 0) {
			_delay_ms(5);
			USIDR = addr;
		}
		else {
			USICR |= 0x02;					// output data (shift data register left by 1)
		}
		_delay_ms(2.5);
		USICR |= USICLK;					// toggle clock
		_delay_ms(5);
		USICR |= USICLK;					// toggle clock
		_delay_ms(2.5);
	}
	_delay_ms(2.5);
	USICR |= USICLK;						// toggle clock
	_delay_ms(5);
	USICR |= USICLK;						// toggle clock
	_delay_ms(2.5);
}