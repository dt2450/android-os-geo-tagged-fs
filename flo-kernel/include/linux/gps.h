#ifndef __GPS_H
#define __GPS_H

extern struct gps_location __k_loc;
extern spinlock_t gps_loc_lock;
extern unsigned long long __timestamp;

#endif /* __GPS_H */

