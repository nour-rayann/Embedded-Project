#include "stdint.h"
void LCD_WriteByte(uint8_t byte, uint8_t mode);
void LCD_WriteString(char *str);
void I2C_Init(void);
void LCD_Init(void);
