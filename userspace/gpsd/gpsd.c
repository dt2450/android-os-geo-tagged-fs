#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/syscall.h>

#include "gpsd.h"

//#define __NR_set_gps_location   378
//#define __NR_get_gps_location   379


struct gps_location *get_file_data()
{
	struct gps_location *loc = NULL;

	printf("gpsd: reading GPS data from file.\n");
	//read from GPS_LOCATION_FILE here
	loc = (struct gps_location *) malloc(sizeof(struct gps_location));
	loc->latitude = 111;
	loc->longitude = 222;
	loc->accuracy = 333;

	return loc;
}


int main(int argc, char *argv[])
{
	struct gps_location *loc = NULL;
	pid_t pid, sid;
	int ret;


	pid = fork();
	if (pid < 0) {
		perror("gpsd: could not fork.\n");
		exit(EXIT_FAILURE);
	}

	if (pid > 0)
		exit(EXIT_SUCCESS);

	sid = setsid();
	if (sid < 0) {
		perror("gpsd: could not create new session.\n");
		exit(EXIT_FAILURE);
	}

	//close(STDIN_FILENO);
	//close(STDOUT_FILENO);
	//close(STDERR_FILENO);

	printf("Begin reading GPS data ...\n\n");
	while(1) {
		loc = get_file_data();
		if (loc == NULL) {
			perror("gpsd: could not read GPS data.\n");
			exit(EXIT_FAILURE);
		}

		ret = set_gps_location(loc);
		if (ret != 0) {
			perror("gpsd: couldn't set GPS data in kernel.\n");
			exit(EXIT_FAILURE);
		}

		if (usleep(1000 * 1000) == -1) {
			perror("gpsd: couldn't sleep.\n");
			exit(EXIT_FAILURE);
		}
	}


	return 0;
}

