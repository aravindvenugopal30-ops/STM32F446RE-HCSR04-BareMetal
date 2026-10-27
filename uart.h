/*
 * uart.h
 *
 *  Created on: Oct 24, 2026
 *      Author: ARAVIND
 */

#ifndef UART_H_
#define UART_H_

#include<stdio.h>
#include<stdint.h>

void uart2_write(int ch);
void ura2_tx_init(void);
void uart_writ_string(char *str);

#endif /* UART_H_ */
