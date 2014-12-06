#ifndef __GPS_H
#define __GPS_H

extern struct gps_location __k_loc;
extern spinlock_t gps_loc_lock;
unsigned long long __get_timestamp(void);

#endif /* __GPS_H */

