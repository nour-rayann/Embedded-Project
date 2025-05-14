#include "stdint.h"
#include "TM4C123GH6PM.h"

void UART2_Init(void) {
    SYSCTL_RCGCUART_R |= 0x04;     // Enable UART2 clock
    SYSCTL_RCGCGPIO_R |= 0x08;     // Enable Port D clock
    while ((SYSCTL_PRGPIO_R & 0x08) == 0); // Wait until ready

    // Unlock PD7 (special pin)
    GPIO_PORTD_LOCK_R = 0x4C4F434B;
    GPIO_PORTD_CR_R |= 0xC0;       // Allow changes to PD6, PD7

    GPIO_PORTD_AFSEL_R |= 0xC0;    // Enable alternate function on PD6, PD7
    GPIO_PORTD_PCTL_R &= ~0xFF000000;
    GPIO_PORTD_PCTL_R |= 0x11000000; // PD6 = U2RX, PD7 = U2TX
    GPIO_PORTD_DEN_R |= 0xC0;      // Enable digital on PD6, PD7
    GPIO_PORTD_AMSEL_R &= ~0xC0;   // Disable analog on PD6, PD7

    UART2_CTL_R &= ~0x01;          // Disable UART2
    UART2_IBRD_R = 104;            // IBRD = int(16,000,000 / (16 * 9600))
    UART2_FBRD_R = 11;             // FBRD = int(0.1667 * 64 + 0.5)
    UART2_LCRH_R = 0x60;           // 8-bit, no parity, 1-stop bit
    UART2_CTL_R = 0x301;           // Enable UART2, TXE, RXE
}
void UART2_SendChar(char c) {
    while ((UART2_FR_R & 0x20) != 0); // Wait if TX FIFO full
    UART2_DR_R = c;
}

void UART2_SendString(char *str) {
    while (*str) {
        UART2_SendChar(*str++);
    }
}
char UART2_ReadChar(void) {
    while (UART2_FR_R & 0x10);
		return UART2_DR_R & 0xFF;
}