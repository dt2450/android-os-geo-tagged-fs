#ifndef __GPS_H
#define __GPS_H

//struct gps_location {
//	double latitude;
//	double longitude;
	//float  accuracy;  /* in meters */
//	double  accuracy;  /* in meters */
//};

struct gps_location *__get_gps_location(void);
unsigned long long __get_timestamp(void);

#endif /* __GPS_H */

