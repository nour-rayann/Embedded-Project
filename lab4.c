#include "io.h"

void RGBLED_Init(void)
	
{
	SYSCTL_RCGCGPIO_R |= 0x20;
	
	while((SYSCTL_PRGPIO_R & 0x20) == 0);
	
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= 0x0E;
	
	GPIO_PORTF_AMSEL_R &= ~0x0E;
	GPIO_PORTF_PCTL_R &= ~0xFFF0;
	GPIO_PORTF_AFSEL_R &= ~0x0E;
	GPIO_PORTF_DIR_R |= 0x0E;
	GPIO_PORTF_DEN_R |= 0x0E;
	GPIO_PORTF_DATA_R &= ~0x0E;
	
}

void SW1_Init(void)
	
{
	SYSCTL_RCGCGPIO_R |= 0x20;
	
	while((SYSCTL_PRGPIO_R & 0x20) == 0);
	
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= 0x10;
	
	GPIO_PORTF_AMSEL_R &= ~0x10;
	GPIO_PORTF_PCTL_R &= ~0xF0000;
	GPIO_PORTF_AFSEL_R &= ~0x10;
	GPIO_PORTF_DIR_R &= ~0x10;
	GPIO_PORTF_DEN_R |= 0x10;
	GPIO_PORTF_PUR_R |= 0x10;
	
}

unsigned char SW1_Input(void)
{
	
	return GPIO_PORTF_DATA_R & 0x10;
	
	
}

void RGB_led(unsigned char data)
{
	GPIO_PORTF_DATA_R &= ~0x0E;
	GPIO_PORTF_DATA_R |= data;
	
}


unsigned char but;
unsigned char led = 0x02;


int main()
{
	SW1_Init();
	RGBLED_Init();
	
	
	while (1) 
	{
		if (led == 0x10) led = 0x02;
		but = SW1_Input();
		if (but != 0x10)
		{
			RGB_led(led);
			led = led << 1;
			for (int i = 0; i <1000000;i++);
		}
		
		
		
		
	}
	
	
	
}


