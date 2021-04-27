/*
 * i2c.h
 *
 * Created on: Apr 17, 2021
 *     Author: Adam Morrison
 */

#ifndef I2C_H
#define I2C_H

void i2c_start();
void i2c_stop();
void i2c_send_addr(char addr);
void i2c_send_data(char data);

#endif