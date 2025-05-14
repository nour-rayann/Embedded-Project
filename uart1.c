#include "stdint.h"
#include "TM4C123GH6PM.h"


void UART1_Init(void) {
    SYSCTL_RCGCUART_R |= 0x02;
    SYSCTL_RCGCGPIO_R |= 0x02;
    while (!(SYSCTL_PRGPIO_R & 0x02));
    GPIO_PORTB_AFSEL_R |= 0x03;
    GPIO_PORTB_PCTL_R |= 0x00000011;
    GPIO_PORTB_DEN_R |= 0x03;
    GPIO_PORTB_AMSEL_R &= ~0x03;
    UART1_CTL_R &= ~0x01;
    UART1_IBRD_R = 104;  // 9600 baud
    UART1_FBRD_R = 11;
    UART1_LCRH_R = 0x60;
    UART1_CTL_R |= 0x301;
}

char UART1_ReadChar(void) {
    while (UART1_FR_R & 0x10);
    return UART1_DR_R & 0xFF;
}
