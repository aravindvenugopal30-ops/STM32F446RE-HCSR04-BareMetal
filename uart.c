/*
 * uart.c
 *
 *  Created on: Oct 24, 2026
 *      Author: ARAVIND
 */
#include<stdio.h>
#include"uart.h"

#include <stdint.h>
#define PERIPH_BASE        (0x40000000UL)
#define APB1PERIH_OFFSET   (0x0000)
#define APB1PERIH_BASE     (PERIPH_BASE + APB1PERIH_OFFSET)


#define AHB1PERIPH_OFFSET  (0x00020000UL)
#define AHB1PERIPH_BASE    (PERIPH_BASE + AHB1PERIPH_OFFSET)
#define GPIOA_OFFSET       (0x0000)
#define GPIOA_BASE         (AHB1PERIPH_BASE + GPIOA_OFFSET)



#define USART2_OFFSET      (0x4400UL)
#define USART2             (USART2_OFFSET + APB1PERIH_BASE )
#define USART2_SR          (*(volatile unsigned int*)(USART2 + 0x00))
#define USART2_DR          (*(volatile unsigned int*)(USART2 + 0x04))
#define USART2_BRR         (*(volatile unsigned int*)(USART2 + 0x08))
#define USART2_CR1         (*(volatile unsigned int*)(USART2 + 0x0C))



#define MODE_R_OFFSET      (0x00UL)
#define GPIOA_MODE_R       (*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))

#define RCC_OFFSET          (0x3800UL)
#define RCC_BASE            (AHB1PERIPH_BASE+ RCC_OFFSET )



#define  APB1ENR_OFFSET    (0x40UL)
#define  RCC_APB1ENR       (*(volatile unsigned int *)(APB1ENR_OFFSET + RCC_BASE))

#define AHB1ENR_OFFSET     (0x30UL)
#define RCC_AHB1ENR        (*(volatile unsigned int *)(AHB1ENR_OFFSET + RCC_BASE))

#define AFRL_OFFSET        (0x20UL)
//#define AFRL_BASE          (AHB1PERIPH_BASE + AFRL_OFFSET)
#define GPIOA_AFRL         (*(volatile unsigned int *)(GPIOA_BASE + AFRL_OFFSET))



#define GPIOAEN            (1U<<0)
#define USART2EN           (1U<<17)
#define SR_TXE             (1U<<7)
#define SYS_FR                      16000000
#define APB1_CLK                    SYS_FR
#define UART_BAUDRATE               115200


void uart2_write(int ch);
void ura2_tx_init(void);
void uart_writ_string(char *str);
uint32_t static compute_uart_div(uint32_t periph_clk,uint32_t baurdrate);
static void urat_set_baudrate(uint32_t periphclk, uint32_t BaudRate);



void uart2_write(int ch){
	while(!(USART2_SR&(1U<<7))){

	}
	USART2_DR=(ch&0xFF);
}

void uart_writ_string(char *str){
	while(*str){
		uart2_write(*str);
		str++;
	}
}
static void urat_set_baudrate(uint32_t periphclk, uint32_t BaudRate){

	USART2_BRR =compute_uart_div(periphclk,BaudRate);
}

uint32_t static compute_uart_div(uint32_t periph_clk,uint32_t baurdrate){

	return ((periph_clk + (baurdrate/2U))/baurdrate);


}

void ura2_tx_init(void){

	RCC_AHB1ENR |= GPIOAEN;

	GPIOA_MODE_R &=~(3U<<4);
	GPIOA_MODE_R |=(2U<<4);
//  setting  to alternate function mode

    GPIOA_AFRL &=~(0xFU<<8);
    GPIOA_AFRL |=(7U<<8);
// setting AFRL


    RCC_APB1ENR |=USART2EN;
   // USART2_BRR = 0x0683;
	urat_set_baudrate(APB1_CLK, UART_BAUDRATE);
    USART2_CR1 |=(1U<<13)|(1U<<3)|(1<<2);


}









