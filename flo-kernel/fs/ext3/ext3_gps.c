#include <linux/fs.h>
#include <linux/gps.h>
#include "ext3_gps.h"
#include "ext3.h"

int ext3_set_gps_location(struct inode *f_inode)
{
	struct ext3_inode_info *ei;
	unsigned long long temp;
	struct timespec t;

	ei = EXT3_I(f_inode);

	spin_lock(&gps_loc_lock);
	spin_lock(&ei->inode_gps_lock);
	ei->i_latitude = cpu_to_le64(*((u64 *)&__k_loc.latitude));
	ei->i_longitude = cpu_to_le64(*((u64 *)&__k_loc.longitude));
	ei->i_accuracy = cpu_to_le32(*((u32 *)&__k_loc.accuracy));

	if (__timestamp < ~0) {
		t = CURRENT_TIME_SEC;
		temp = (t.tv_sec - __timestamp);
	} else {
		temp = ~0;
	}
	ei->i_coord_age = cpu_to_le32((u32)temp);
	spin_unlock(&ei->inode_gps_lock);
	spin_unlock(&gps_loc_lock);
	return 0;
}

int ext3_get_gps_location(struct inode *f_inode,
		struct gps_location *location_data)
{
	u64 lat, lon;
	u32 accuracy;
	unsigned long long time;
	struct ext3_inode_info *ei;

	if (location_data == NULL || f_inode == NULL)
		return -EINVAL;

	ei = EXT3_I(f_inode);

	spin_lock(&ei->inode_gps_lock);

	lat = le64_to_cpu(ei->i_latitude);
	lon = le64_to_cpu(ei->i_longitude);
	accuracy = le32_to_cpu(ei->i_accuracy);
	time = le32_to_cpu(ei->i_coord_age);

	spin_unlock(&ei->inode_gps_lock);

	location_data->latitude = *(double *)&lat;
	location_data->longitude = *(double *)&lon;
	location_data->accuracy = *(float *)&accuracy;
	return time;
}
