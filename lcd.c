
#include "stdint.h"
#include "TM4C123GH6PM.h"
#define LCD_ADDR 0x27  // or 0x3F

// LCD control bits
#define EN 0x04  // Enable bit
#define RW 0x02  // Read/Write bit
#define RS 0x01  // Register select bit
volatile uint32_t k ;
void DelayMs(uint32_t ms)
{
    for( k = 0; k < ms * 1600; k++);
}
void I2C_Init(void)
{
    // Configure I2C pins (PB2-SCL, PB3-SDA)
		SYSCTL_RCGCGPIO_R |= 0x02;       // Enable Port B
    SYSCTL_RCGCI2C_R |= 0x01;        // Enable I2C0
    DelayMs(100);                     // Wait for clock stabilization
    GPIO_PORTB_AFSEL_R |= 0x0C;           // Enable alternate function
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFF00FF) | 0x00003300;
    GPIO_PORTB_DEN_R |= 0x0C;             // Digital enable
    GPIO_PORTB_ODR_R |= 0x08;             // Open-drain on SDA (PB3)

    // Initialize I2C0
    I2C0_MCR_R = 0x0010;             // Master mode
    I2C0_MTPR_R = 0x07;              // 100kHz at 16MHz
    while(I2C0_MCR_R & 0x01);        // Wait for initialization
}

void LCD_WriteNibble(uint8_t nibble, uint8_t mode)
{
    uint8_t data = nibble | mode | 0x08;  // Backlight on
    uint8_t data_en = data | EN;
    uint8_t data_no_en = data & ~EN;
    
    // Send enable pulse
    while(I2C0_MCS_R & 0x01);        // Wait until not busy
    I2C0_MSA_R = (LCD_ADDR << 1);
    I2C0_MDR_R = data_en;
    I2C0_MCS_R = 0x07;               // Start, Run, Stop
    while(I2C0_MCS_R & 0x40);        // Wait for completion
    
    DelayMs(1);
    
    // Turn off enable
    while(I2C0_MCS_R & 0x01);
    I2C0_MSA_R = (LCD_ADDR << 1);
    I2C0_MDR_R = data_no_en;
    I2C0_MCS_R = 0x07;
    while(I2C0_MCS_R & 0x40);
    
    DelayMs(1);
}

void LCD_WriteByte(uint8_t byte, uint8_t mode)
{
    // Send upper nibble
    LCD_WriteNibble(byte & 0xF0, mode);
    // Send lower nibble
    LCD_WriteNibble((byte << 4) & 0xF0, mode);
}

void LCD_Init(void)
{
    DelayMs(50);  // Power-up delay
    
    // Special initialization sequence for 4-bit mode
    LCD_WriteNibble(0x30, 0);  // Try to set 8-bit mode
    DelayMs(5);
    LCD_WriteNibble(0x30, 0);
    DelayMs(1);
    LCD_WriteNibble(0x30, 0);
    DelayMs(1);
    LCD_WriteNibble(0x20, 0);  // Switch to 4-bit mode
    
    // Now we can use 4-bit commands
    LCD_WriteByte(0x28, 0);    // 4-bit, 2-line, 5x8 font
    LCD_WriteByte(0x0C, 0);    // Display on, cursor off
    LCD_WriteByte(0x06, 0);    // Entry mode: increment, no shift
    LCD_WriteByte(0x01, 0);    // Clear display
    DelayMs(2);                // Clear display needs extra time
}

void LCD_WriteString(char *str)
{
    while(*str) {
        LCD_WriteByte(*str++, 1);  // RS=1 for data
    }
}

