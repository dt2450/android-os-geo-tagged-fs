#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "file_loc.h"


int main(int argc, char *argv[])
{
	char *path_name = "/data/misc/hmwk6/test";
	struct gps_location *loc = NULL;
	int age = 0;

	if (argc == 2)
		path_name = argv[1];
	else
		printf("No file name passed!!\n");

	printf("reading location info for: %s\n", path_name);
	loc = (struct gps_location *) malloc(sizeof(struct gps_location));
	age = syscall(__NR_get_gps_location, path_name, loc);
	if (age < 0) {
		printf("Failed with error: %s\n", strerror(errno));
		return -1;
	}
	printf("Age: %d\n", age);
	printf("Lat: %f\n", loc->latitude);
	printf("Long: %f\n", loc->longitude);
	printf("Accuracy: %f\n", loc->accuracy);

	printf("http://maps.googleapis.com/maps/api/staticmap?center=%f,%f",
			loc->latitude, loc->longitude);
	printf("&zoom=14&size=600x600&markers=color:blue%%7Clabel:S%%7C%f,%f\n",
			loc->latitude, loc->longitude);

	return 0;
}
