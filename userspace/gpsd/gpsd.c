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
	FILE* f = NULL;
	char line[128];
	int line_num = 0;
	double lat = -1, lon = -1;
	float acc = -1;
	int ret;

	loc = (struct gps_location *) malloc(sizeof(struct gps_location));
	if (loc == NULL) {
		perror("gpsd: couldn't allocate memory for location struct.");
		return NULL;
	}

	//printf("gpsd: reading GPS data from file.\n");
	f = fopen(GPS_LOCATION_FILE, "r");
	if (f == NULL) {
		perror("gpsd: couldn't open GPS file for reading.");
		return NULL;
	}

	while (fgets(line, sizeof(line), f) != NULL) {
		switch (line_num) {
		case 0:	lat = strtod(line, NULL);
			break;
		case 1:	lon = strtod(line, NULL);
			break;
		case 2:	acc = strtof(line, NULL);
			break;
		default:printf("gpsd: unexpected GPS file format\n");
			return NULL;
		}
		line_num++;
	}

	ret = fclose(f);
	if (ret != 0) {
		perror("gpsd: couldn't close GPS location file.");
		return NULL;
	}

	if (lat == -1 || lon == -1 || acc == -1) {
		printf("gpsd: error reading GPS file value(s).\n");
		return NULL;
	} else {
		printf("gpsd: lat: %f, lon: %f, acc: %f\n",
				lat, lon, acc);
	}

	loc->latitude = lat * 1000;
	loc->longitude = lon * 1000;
	loc->accuracy = acc * 1000;

	return loc;
}


int main(int argc, char *argv[])
{
	struct gps_location *loc = NULL;
	pid_t pid, sid;
	int ret;


	pid = fork();
	if (pid < 0) {
		perror("gpsd: could not fork.");
		exit(EXIT_FAILURE);
	}

	if (pid > 0)
		exit(EXIT_SUCCESS);

	sid = setsid();
	if (sid < 0) {
		perror("gpsd: could not create new session.");
		exit(EXIT_FAILURE);
	}

	//close(STDIN_FILENO);
	//close(STDOUT_FILENO);
	//close(STDERR_FILENO);

	printf("Begin reading GPS data ...\n\n");
	while(1) {
		loc = get_file_data();
		if (loc == NULL) {
			printf("gpsd: could not read GPS data.\n");
			exit(EXIT_FAILURE);
		}

		ret = set_gps_location(loc);
		if (ret != 0) {
			perror("gpsd: couldn't set GPS data in kernel.");
			exit(EXIT_FAILURE);
		}
		free(loc);

		if (usleep(1000 * 1000) == -1) {
			perror("gpsd: couldn't sleep.");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}

