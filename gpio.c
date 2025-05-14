#include "stdint.h"
#include "TM4C123GH6PM.h"



void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;          // Enable clock for Port F
    while ((SYSCTL_PRGPIO_R & 0x20) == 0); // Wait until Port F is ready

    GPIO_PORTF_DIR_R |= 0x06;           // Set PF1 as output
    GPIO_PORTF_DEN_R |= 0x06;           // Digital enable PF1
    GPIO_PORTF_AFSEL_R &= ~0x06;        // Disable alternate function
    GPIO_PORTF_AMSEL_R &= ~0x06;        // Disable analog mode
    GPIO_PORTF_PCTL_R &= ~0x0000FF00;   // Clear PCTL for PF1
		GPIO_PORTF_DATA_R &= ~0x06;
}

void Delay_Ms(uint32_t ms) {
    uint32_t i;
    while (ms > 0) {
        i = 13333; // ~1ms delay for 16MHz clock
        while (i > 0) {
            i--;
        }
        ms--;
    }
}
void buzz (void)
{
			GPIO_PORTF_DATA_R |= 0x02; 
			Delay_Ms(250);

			GPIO_PORTF_DATA_R &= ~0x02; 
}

void led (void)
{
			GPIO_PORTF_DATA_R |= 0x04; 
			Delay_Ms(250);

			GPIO_PORTF_DATA_R &= ~0x04; 
}

