#ifndef __GPS_H
#define __GPS_H

struct gps_location {
	double latitude;
	double longitude;
	//float  accuracy;  /* in meters */
	double  accuracy;  /* in meters */
};

#endif /* __GPS_H */

