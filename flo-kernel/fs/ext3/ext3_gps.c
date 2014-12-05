#include <linux/fs.h>
#include <linux/gps.h>
#include "ext3_gps.h"
#include "ext3.h"

int ext3_set_gps_location(struct inode *f_inode)
{
	struct ext3_inode_info *ei = EXT3_I(f_inode);
        struct gps_location *location_data = __get_gps_location();
	unsigned long long temp = __get_timestamp();
	write_lock(&ei->inode_gps_lock); 
	ei->i_latitude = cpu_to_le64(*((__u64 *)&location_data->latitude));/*cpu_to from fs.h*/
	ei->i_longitude = cpu_to_le64(*((__u64 *)&location_data->longitude));
	ei->i_accuracy = cpu_to_le32(*((__u32 *)&location_data->accuracy));
	ei->i_coord_age = cpu_to_le32((__u32)temp);
	write_unlock(&ei->inode_gps_lock);
        return 1;

}

int ext3_get_gps_location(struct inode *f_inode, struct gps_location *location_data)
{
	__u64 lat,lon;
	__u32 accuracy;
	struct ext3_inode_info *ei;
	if (location_data == NULL || f_inode == NULL)
		return -EINVAL;
	ei = EXT3_I(f_inode);
	read_lock(&ei->inode_gps_lock);
	lat = le64_to_cpu(ei->i_latitude);
	lon = le64_to_cpu(ei->i_longitude);
	accuracy = le32_to_cpu(ei->i_latitude);
	read_unlock(&ei->inode_gps_lock);
	location_data->latitude = *(double *)&lat;
	location_data->longitude = *(double *)&lon;
	location_data->accuracy = *(float *)&accuracy;
        return 1;
}

