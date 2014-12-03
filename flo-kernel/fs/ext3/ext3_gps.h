#ifndef __EXT3_GPS_H
#define __EXT3_GPS_iH
#include <linux/fs.h>

int ext3_set_gps_location(struct inode *);
int ext3_get_gps_location(struct inode *, struct gps_location *);
#endif
