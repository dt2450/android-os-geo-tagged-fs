#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>

#define __NR_set_gps_location	378
#define __NR_get_gps_location	379


struct gps_location {
	double latitude;
	double longitude;
	float  accuracy;  /* in meters */
};

int main(void)
{
	struct gps_location *loc;
	int ret;

	loc = (struct gps_location *) malloc(sizeof(struct gps_location));
	loc->latitude = 111;
	loc->longitude = 222;
	loc->accuracy = 333;

	ret = syscall(__NR_set_gps_location, loc);
	printf("Ret: %d\n", ret);

	ret = syscall(__NR_get_gps_location, "/root", loc);
	printf("Ret: %d\n", ret);

	return 0;

}

