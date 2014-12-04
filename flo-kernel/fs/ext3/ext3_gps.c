#include <linux/fs.h>
#include <linux/gps.h>
#include "ext3_gps.h"
#include "ext3.h"

int ext3_set_gps_location(struct inode *f_inode)
{
	/*TODO:locking*/
	struct ext3_inode_info *ei = EXT3_I(f_inode);
        struct gps_location *location_data = __get_gps_location();
        ei->i_latitude = cpu_to_le64(*((__u64 *)&location_data->latitude));/*cpu_to from fs.h*/
	ei->i_longitude = cpu_to_le64(*((__u64 *)&location_data->longitude));
	ei->i_accuracy = cpu_to_le32(*((__u32 *)&location_data->accuracy));
	//f_inode->i_coord_age = TODO: function implementation missing 
        return 0;
	/*TODO:locking*/

}

int ext3_get_gps_location(struct inode *f_inode, struct gps_location *location_data)
{
	__u64 lat,lon;
	__u32 accuracy;
	struct ext3_inode_info *ei;
	/*TODO:locking*/
	if (location_data == NULL || f_inode == NULL)
		return -1;
	ei = EXT3_I(f_inode);

	lat = le64_to_cpu(ei->i_latitude);
	lon = le64_to_cpu(ei->i_longitude);
	accuracy = le32_to_cpu(ei->i_latitude);

	location_data->latitude = *(double *)&lat;
	location_data->longitude = *(double *)&lon;
	location_data->accuracy = *(float *)&accuracy;
        return 0;
	/*TODO:locking*/
}

