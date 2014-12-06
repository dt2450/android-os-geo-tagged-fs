#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/syscall.h>

#include "gpsd.h"


int main(int argc, char *argv[])
{
	char *path_name = "/data/misc/hmwk6/test";
        struct gps_location *loc = NULL;
	int age = 0;
	if (argc == 2) {
		path_name = argv[1];	
	} else {
		printf("\n No file name passed!!");
	}
	printf("reading location info for: %s", path_name);
	loc = (struct gps_location *) malloc(sizeof(struct gps_location));
       	age = syscall(__NR_get_gps_location, path_name, loc);
	printf("\nAGE::::%d",age);
	printf("\nLat:::%f", loc->latitude);
	printf("\nLong:::%f", loc->longitude);
	printf("\nAccuracy:::%f", loc->accuracy);
	return 0;
}
