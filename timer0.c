#include "tm4c123gh6pm.h"
#include <stdint.h>

int start_time = 0;
int stop_time;
int ticks_elapsed;
int seconds;
	
// Initialize Timer0A in 32-bit down mode, no interrupts
void Timer0A_Init(void) {
    SYSCTL_RCGCTIMER_R |= 0x01;      // Enable Timer0 clock
    while ((SYSCTL_PRTIMER_R & 0x01) == 0); // Wait

    TIMER0_CTL_R &= ~0x01;           // Disable Timer0A
    TIMER0_CFG_R = 0x00;             // 32-bit mode
    TIMER0_TAMR_R = 0x02;            // Periodic mode, count down
    TIMER0_TAILR_R = 0xFFFFFFFF;     // Max start value
    TIMER0_TAPR_R = 0;               // No prescaler
    TIMER0_ICR_R = 0x1;              // Clear timeout flag
}

// Start Timer0A (store initial value)
void Timer0A_Start(void) {
    TIMER0_TAV_R = 0xFFFFFFFF;       // Reset counter
    TIMER0_CTL_R |= 0x01;            // Enable Timer0A
    start_time = TIMER0_TAV_R;
}

// Stop Timer0A and return elapsed time in seconds
int Timer0A_Stop(void) {
    TIMER0_CTL_R &= ~0x01;           // Disable Timer0A
    stop_time = TIMER0_TAV_R;
    ticks_elapsed = start_time - stop_time;
    seconds = ticks_elapsed / 16000000;  // Assuming 16 MHz clock
    return seconds;
}

