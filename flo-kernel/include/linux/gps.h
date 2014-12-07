#ifndef __GPS_H
#define __GPS_H

extern struct gps_location __k_loc;
extern spinlock_t gps_loc_lock;
unsigned long long get_age(void);

#endif /* __GPS_H */

