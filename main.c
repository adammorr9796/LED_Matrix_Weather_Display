/*
 * main.c
 *
 * Created on: March 28, 2021
 *     Author: Adam Morrison
 */ 

#define F_CPU 1000000
#define MTX_0 0b11100000
#define MTX_1 0b11100010
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <c:/users/adamm/Documents/Atmel Studio/7.0/LED_Matrix_Weather_Display/i2c.h>

uint8_t motion_detect_flag = 0;

void clear_matrix(char addr);
char shift_it(char byte);

const char zero_bitmap[] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00111000,
	0b00101000,
	0b00101000,
	0b00101000,
	0b00111000	
};

const char one_bitmap[] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00001000
};

const char two_bitmap[] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00111000,
	0b00001000,
	0b00111000,
	0b00100000,
	0b00111000
};

const char three_bitmap[] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00111000,
	0b00001000,
	0b00111000,
	0b00001000,
	0b00111000
};

const char four_bitmap[] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00101000,
	0b00101000,
	0b00111000,
	0b00001000,
	0b00001000
};

const char five_bitmap[] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00111000,
	0b00100000,
	0b00111000,
	0b00001000,
	0b00111000
};

const char six_bitmap[] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00100000,
	0b00100000,
	0b00111000,
	0b00101000,
	0b00111000
};

const char seven_bitmap[] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00111000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b00001000
};

const char eight_bitmap[] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00111000,
	0b00101000,
	0b00111000,
	0b00101000,
	0b00111000
};

const char nine_bitmap[] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00111000,
	0b00101000,
	0b00111000,
	0b00001000,
	0b00001000
};

const char neg_bitmap[] = {
	0b00000000,
	0b00011100,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000
};

int main(void)
{
	/******GPIO setup******/
	DDRA |= 0x08;					// set PA3 to OUTPUT
	/**********************/
	
	/****Interrupt setup***/
	sei();							// global interrupt enable
	//GIMSK |= 0x10;				// set PCIE0 bit - PCINT7:0 interrupts enabled	
	//PCMSK0 |= 0x01;				// enable PCINT0 interrupt on pin 13 of ATTINY84
	/**********************/

	/*******I2C setup******/
	USICR |= 0x20;					// set USI to Two-Wire mode
	USICR &= ~(0x0C);				// set USICS1 and USICS0 bits to 0 for software clock strobe 

	DDRA |= 0x50;					// set direction of SCL & SDA pin (PA6, PA4) to enable data output from the USI Data Register
	
	PORTA |= 0x10;					//PA4 SCL
	PORTA |= 0x40;					//PA6 SDA
	USIDR |= 0xFF;
	_delay_us(10);
	/**********************/
	
	int m = 0;
	int currMatrix = 0;
	
	for(m = 0; m < 2; m++) {			// interacting with 2 LED matrices with 2 different addresses
		if (m == 0) {
			currMatrix = MTX_0;
		}
		else {
			currMatrix = MTX_1;
		}
		i2c_start();
		i2c_send_addr(currMatrix);		// addr followed by R/W bit - 0 = controller write, 1 = controller read
		i2c_send_data(0x21);			// internal clock enable
		i2c_stop();
	
		_delay_us(10);
	
		i2c_start();
		i2c_send_addr(currMatrix);
		i2c_send_data(0xA0);			// row/int set
		i2c_stop();
	
		_delay_us(10);
	
		i2c_start();
		i2c_send_addr(currMatrix);
		i2c_send_data(0xE1);			// dimming set
		i2c_stop();
	
		_delay_us(10);
	
		i2c_start();
		i2c_send_addr(currMatrix);
		i2c_send_data(0x81);			// blinking set
		i2c_stop();
	
		_delay_us(10);
		
		clear_matrix(currMatrix);
		_delay_us(10);

		i2c_start();
		i2c_send_addr(currMatrix);
		i2c_send_data(0x00);
		
		int c = 0;
		for (c = 0; c < 8; c++) {
			i2c_send_data(shift_it(((neg_bitmap[c]) | (zero_bitmap[c] << 1) | (seven_bitmap[c] >> 3))));	// write out bitmaps to LED and shift them to opposite sides so both digits appear on screen
			i2c_send_data(0x00);														// skip one address in display RAM since only every-other address is tied to an LED row on the matrix
		}
		
		i2c_stop();
	}
	
	while (1) 
    {
		//if (motion_detect_flag == 1) {
		//	motion_detect_flag = 0;
		//}
    }
}

char shift_it(char byte) {
	//reverse the byte and shift it by 1 bit due to the way the led matrix writes bits to the LEDs
	char mod_byte = 0x00;
	
	int i = 0;
	for(i = 7; i >=0; i--) {
		if (byte & 0x01) {
			mod_byte |= (0x01 << i);
		}
		byte >>= 1;
	}
	
	if (mod_byte & 0x01) {
		return (mod_byte >> 1) | 0x80;		// circular shift and carry the one
	}
	else {
		return mod_byte >> 1;				// circular shift but no need to carry 
	}
}

void clear_matrix(char addr) {
	i2c_start();
	i2c_send_addr(addr);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_send_data(0x00);
	i2c_stop();
}

//ISR(PCINT0_vect) {
//	//_delay_us(10);
//	motion_detect_flag = 1;
//}