#include "stdint.h"
#include "TM4C123GH6PM.h"
#include <stdio.h>
#include "uart1.h"
#include "gps.h"
#include "comp.h"
#include "lcd.h"
#include "timer.h"
#include "gpio.h"
#include "uart0.h"
#include "timer0.h"
#include <String.h>

#define acc 1000000

char gps_data_ready;

volatile int stat;
volatile int min_ind;
volatile double lat;
volatile double lon;
volatile int targ;

int s;
int h;
int m;
char b;
extern char latest_lat[15];
extern char latest_lon[15];
extern char latest_ns ;
extern char latest_ew ;
char* place[]={"No Fix","Workshops", "Mosque", "Gate 6", "Library", "Hall A & B","Architecture", "Hall C & D"};
char buff[100];
char cnt[100];

int main(void) {
		

		gps_data_ready = 0;
		stat = 0;
		min_ind = 0;
		latest_ns = 'N';
		latest_ew = 'E';
    UART1_Init();
    I2C_Init();
    LCD_Init();
		UART2_Init();
		PortF_Init();
		Timer0A_Init();
		b = UART2_ReadChar();
		targ = b - '0' ;
		Timer0A_Start();
		

    while (1) {
				char c = UART1_ReadChar();
        gps_data_ready = processGPSChar(c);
				
				
				if (gps_data_ready) {
					
					
					lat = convertToDecimal(latest_lat, latest_ns);
					lon = convertToDecimal(latest_lon, latest_ew);
					if ( ((int)(lat /10)) != 3 ) 
						{min_ind = 0;
							LCD_WriteByte(0x01, 0);
							LCD_WriteByte(0x80, 0);  
							LCD_WriteString(place[min_ind]);
					}
					else{
						min_ind = compare(lat*acc,lon*acc);
						led();
					}
				
					if (stat != min_ind){ 
							LCD_WriteByte(0x01, 0);
							LCD_WriteByte(0x80, 0);  
							LCD_WriteString(place[min_ind]);
							buzz();
						
							if (min_ind == targ) 
							{
								s = Timer0A_Stop();
								h = s/3600;
								s = s % 3600;
								m = s/60;
								s = s % 60;
								sprintf(cnt,"%dH:%dM:%dS",h,m,s);
								LCD_WriteByte(0xC0, 0);
								LCD_WriteString(cnt);
								s = 0;
								targ = -1;

						}
						
						
					}
					stat = min_ind;

					gps_data_ready = 0;
					UART2_SendString(place[min_ind]);
					sprintf(buff,"\n%f \n%f \n--------------- \n",lat,lon);
					UART2_SendString(buff);
					
				}
				
				
    }
		
}
