#include "stdint.h"
#include "TM4C123GH6PM.h"
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LEN 100
char gps_buffer[MAX_LINE_LEN];
int gps_index = 0;

char latest_lat[15] = "";
char latest_lon[15] = "";
char latest_ns = 'N';
char latest_ew = 'E';

char parseGPRMC(char *line) {
    char *token;
    int field = 0;

    token = strtok(line, ",");
    while (token != NULL) {
        field++;
        switch (field) {
            case 4: strncpy(latest_lat, token, sizeof(latest_lat)); break;
            case 5: latest_ns = token[0]; break;
            case 6: strncpy(latest_lon, token, sizeof(latest_lon)); break;
            case 7: latest_ew = token[0]; break;
        }
        token = strtok(NULL, ",");
    }
    return 1;
}

char processGPSChar(char c) {
    if (c == '$') gps_index = 0;

    if (gps_index < MAX_LINE_LEN - 1) {
        gps_buffer[gps_index++] = c;
        gps_buffer[gps_index] = '\0';
    }

    if (c == '\n') {
				gps_index = 0;
        if (strstr(gps_buffer, "$GPRMC")) {
            return parseGPRMC(gps_buffer);
        }
        
    }
		return 0;
}

double raw;
int deg;
double minn;
double dec;

double convertToDecimal(char *coord, char dir) {
		if (coord == '\0') return 0;
    raw = atof(coord);
    deg = (dir == 'N' || dir == 'S') ? (int)(raw / 100) : (int)(raw / 100);
    minn = raw - (deg * 100);
    dec = deg + (minn / 60.0);
    if (dir == 'S' || dir == 'W') dec *= -1;
    return dec;
}
