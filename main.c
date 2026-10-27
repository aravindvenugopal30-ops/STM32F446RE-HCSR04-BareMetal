
#include <stdint.h>
#include <uart.h>

//PA0 Trig pin
//PA1 Echo pin

#define PERIPH_BASE                 (0x40000000UL)

#define RCC                         (0x40023800UL)

#define AHB1_OFFSET                 (0x20000UL)
#define AHB1_BASE                   (PERIPH_BASE + AHB1_OFFSET)

#define GPIOA_BASE                  (0x40020000UL)

#define AHB1ENR_OFFSET              (0x30UL)

#define RCC_AHB1ENR                 (*(volatile uint32_t *)(RCC + AHB1ENR_OFFSET))

#define MODER_OFFSET                (0x00UL)
#define GPIOA_MODER                 (*(volatile uint32_t *)(GPIOA_BASE + MODER_OFFSET))

#define AFRL_OFFSET                 (0x20UL)
#define GPIOA_AFRL                  (*(volatile uint32_t *)(GPIOA_BASE + AFRL_OFFSET))

#define APB1_OFFSET                 (0x0000UL)
#define APB1_BASE                   (PERIPH_BASE + APB1_OFFSET)

#define APB1ENR_OFFSET              (0x40UL)
#define RCC_APB1ENR                 (*(volatile uint32_t *)(RCC + APB1ENR_OFFSET))

#define TIMER2_BASE                 (0x40000000UL)

#define PSC_OFFSET                  (0x28UL)
#define TIMER2_PSC                  (*(volatile uint32_t *)(TIMER2_BASE + PSC_OFFSET))

#define AAR_OFFSET                  (0x2CUL)
#define TIMER2_ARR                  (*(volatile uint32_t *)(TIMER2_BASE + AAR_OFFSET))

#define CCMR1_OFFSET                (0x18UL)
#define TIMER2_CCMR1                (*(volatile uint32_t *)(TIMER2_BASE + CCMR1_OFFSET))

#define CCER_OFFSET                 (0x20UL)
#define TIMER2_CCER                 (*(volatile uint32_t *)(TIMER2_BASE + CCER_OFFSET))

#define CR1_OFFSET                  (0x00UL)
#define TIMER2_CR1                  (*(volatile uint32_t *)(TIMER2_BASE + CR1_OFFSET))

#define TIMER2_CNT                  (*(volatile uint32_t *)(TIMER2_BASE + 0x24))
#define TIMER2_SR                   (*(volatile uint32_t *)(TIMER2_BASE + 0x10))
#define TIMER2_CCR2                 (*(volatile uint32_t *)(TIMER2_BASE + 0x38))



void PIN_Set(){

	RCC_AHB1ENR |=(1U<<0);

	GPIOA_MODER &=~(3U<<0);
	GPIOA_MODER |=(1U<<0);

	GPIOA_MODER &=~(3U<<2);
	GPIOA_MODER |=(2U<<2);

	GPIOA_AFRL &=~(0xFU<<4);
	GPIOA_AFRL |=(1U<<4);

}
void Timer2_InIt(){

	RCC_APB1ENR |=(1U<<0);

	TIMER2_PSC =16-1;
	TIMER2_ARR=0xFFFF;

	TIMER2_CCMR1 &=~(3U<<8);
	TIMER2_CCMR1 |=(1U<<8);

	TIMER2_CCER &=~(1U<<5);
	TIMER2_CCER |=(1U<<4);

	TIMER2_CR1|=(1U<<0);

}

void Delay_InIt(uint32_t a){

	uint32_t X=TIMER2_CNT;
	while((TIMER2_CNT-X)<=a);


}


float Read_Distance(void){


	uint32_t S_Time=0;
	uint32_t E_Time=0;
	uint32_t PW=0;
	float distance=0;

	GPIOA_MODER |=(1U<<0);
	Delay_InIt(10);
	GPIOA_MODER &=~(1U<<0);

	while((TIMER2_SR &(1U<<2)));
	S_Time=TIMER2_CCR2;
	TIMER2_SR &=~(1U<<2);

	TIMER2_CCER |=(1U<<5);

	while(!(TIMER2_SR & (1U<<2)));
	E_Time=TIMER2_CCR2;
	TIMER2_CCER &=~(1U<<5);

	if (E_Time >= S_Time) {
		PW = E_Time - S_Time;
	    } else {
	    	PW = (0xFFFF - S_Time) + E_Time;
	    }


	    distance = (float)PW / 58.0f;

	    return distance;


}


int main(){

	PIN_Set();
	Timer2_InIt();

	ura2_tx_init();

	float current_distance=0;
	char buffer[50];


	while(1){
		current_distance=Read_Distance();

		sprintf(buffer, "Distance: %.2f cm \r\n" ,current_distance);

		uart_writ_string(buffer);

		Delay_InIt(800000);

	}
}


