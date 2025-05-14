
#include "stdint.h"
#include "TM4C123GH6PM.h"

char stat;
volatile int count;

void EnableInterrupts(void) {
    __asm("    CPSIE I");
}
void Timer0A_Init(void) {
    SYSCTL_RCGCTIMER_R |= 0x01;
    TIMER0_CTL_R = 0;
    TIMER0_CFG_R = 0x00;
    TIMER0_TAMR_R = 0x02;
    TIMER0_TAILR_R = 16000000 - 1;  // 1 sec interval (assuming 16 MHz)
    TIMER0_TAPR_R = 0;
    TIMER0_ICR_R = 0x01;
    TIMER0_IMR_R = 0x01;
    NVIC_EN0_R |= 1 << 19;
    TIMER0_CTL_R |= 0x01;
}
void TIMER0A_Handler(void) {
    TIMER0_ICR_R = 0x01;
		if (stat == 0) count++;

    
}
