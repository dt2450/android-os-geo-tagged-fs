#include <linux/fs.h>
#include <linux/gps.h>
#include "ext3_gps.h"
#include "ext3.h"

int ext3_set_gps_location(struct inode *f_inode)
{
	struct ext3_inode_info *ei = EXT3_I(f_inode);
	struct gps_location *location_data = __get_gps_location();
	unsigned long long temp = __get_timestamp();
	pr_err("\next3_set_gps_loc before write_lock");
	write_lock(&ei->inode_gps_lock);
	pr_err("\n ext3_set_gps_location::: setting the gps_location");
	ei->i_latitude = cpu_to_le64(*((__le64 *)&location_data->latitude));
	ei->i_longitude = cpu_to_le64(*((__le64 *)&location_data->longitude));
	ei->i_accuracy = cpu_to_le32(*((__le32 *)&location_data->accuracy));
	ei->i_coord_age = cpu_to_le32((__le32)temp);
	pr_err("\n ext3_set_gps_location_lat::: %x", ei->i_latitude);
	pr_err("\n ext3_set_gps_location_lon::: %x", ei->i_longitude);
	pr_err("\n ext3_set_gps_location_accu::: %x", ei->i_accuracy);
	pr_err("\n ext3_set_gps_location_age::: %x", ei->i_coord_age);
	write_unlock(&ei->inode_gps_lock);
	return 0;
}

int ext3_get_gps_location(struct inode *f_inode,
		struct gps_location *location_data)
{
	__le64 lat, lon;
	__le32 accuracy;
	unsigned long long time;
	struct ext3_inode_info *ei;

	if (location_data == NULL || f_inode == NULL)
		return -EINVAL;
	ei = EXT3_I(f_inode);

	read_lock(&ei->inode_gps_lock);
	lat = le64_to_cpu(ei->i_latitude);
	lon = le64_to_cpu(ei->i_longitude);
	accuracy = le32_to_cpu(ei->i_latitude);
	time = le32_to_cpu(ei->i_coord_age);
	pr_err("\n ext3_get_gps_location_lat::: %x", lat);
	pr_err("\n ext3_get_gps_location_lon::: %x", lon);
	pr_err("\n ext3_get_gps_location_accu::: %x",accuracy);
	pr_err("\n ext3_get_gps_location_age::: %x", time);
	read_unlock(&ei->inode_gps_lock);
	location_data->latitude = *(double *)&lat;
	location_data->longitude = *(double *)&lon;
	location_data->accuracy = *(float *)&accuracy;
	return time;
}
