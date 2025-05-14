#include <math.h>
#include <stdlib.h>

#define accc 1000000
double lats[]={0,30.064630, 30.064675, 30.064882, 30.064905, 30.064335, 30.064080, 30.063840};
double lons[]={0,31.280160,31.280225,31.280300, 31.280225, 31.280365, 31.280460, 31.280500};

double min;
int min_indx;
int j;
	
double get_Distance(double latt, double lonn,double lat, double lon){
double x= abs(lat-latt);
double y= abs(lon-lonn);
	
return sqrt(x*x+y*y);
}
int compare(double lat, double lon){
	if (lat == 0 && lon == 0) return 0;
	min = get_Distance(lats[1]*accc,lons[1]*accc,lat,lon);
	min_indx=1;
	for(j = 2;j<8;j++){
	double temp= get_Distance(lats[j]*accc,lons[j]*accc,lat,lon);
		
	if(temp<min){
	min=temp;
	min_indx=j;
	}
	
	}
	return min_indx;
}

