#include <linux/fs.h>
#include <linux/gps.h>
#include "ext3_gps.h"
#include "ext3.h"

int ext3_set_gps_location(struct inode *f_inode)
{
	struct ext3_inode_info *ei = EXT3_I(f_inode);

	unsigned long long temp = __get_timestamp();
	pr_err("\next3_set_gps_loc before write_lock");
	spin_lock(&gps_loc_lock);
	spin_lock(&ei->inode_gps_lock);
	pr_err("\n ext3_set_gps_location::: setting the gps_location");
	ei->i_latitude = cpu_to_le64(*((u64 *)&__k_loc.latitude));
	ei->i_longitude = cpu_to_le64(*((u64 *)&__k_loc.longitude));
	ei->i_accuracy = cpu_to_le32(*((u32 *)&__k_loc.accuracy));
	ei->i_coord_age = cpu_to_le32((u32)temp);
	pr_err("\n ext3_set_gps_location_lat::: %llx", ei->i_latitude);
	pr_err("\n ext3_set_gps_location_lon::: %llx", ei->i_longitude);
	pr_err("\n ext3_set_gps_location_accu::: %x", ei->i_accuracy);
	pr_err("\n ext3_set_gps_location_age::: %x", ei->i_coord_age);
	pr_err("\n ext3_inode_info = 0x%x\n", (unsigned int)ei);
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
	accuracy = le32_to_cpu(ei->i_latitude);
	time = le32_to_cpu(ei->i_coord_age);

	pr_err("\n 1. ext3_set_gps_location_lat::: %llx", ei->i_latitude);
	pr_err("\n 1. ext3_set_gps_location_lon::: %llx", ei->i_longitude);
	pr_err("\n 1. ext3_set_gps_location_accu::: %x", ei->i_accuracy);
	pr_err("\n 1. ext3_set_gps_location_age::: %x", ei->i_coord_age);
	pr_err("\n ext3_inode_info = 0x%x\n", (unsigned int)ei);

	pr_err("\n 2.ext3_get_gps_location_lat::: %llx", lat);
	pr_err("\n 2.ext3_get_gps_location_lon::: %llx", lon);
	pr_err("\n 2.ext3_get_gps_location_accu::: %x", accuracy);
	pr_err("\n 2.ext3_get_gps_location_age::: %llx", time);

	spin_unlock(&ei->inode_gps_lock);

	location_data->latitude = *(double *)&lat;
	location_data->longitude = *(double *)&lon;
	location_data->accuracy = *(float *)&accuracy;
	return time;
}
