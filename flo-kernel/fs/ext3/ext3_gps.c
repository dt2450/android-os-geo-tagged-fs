#include <linux/fs.h>
#include <linux/gps.h>
#include "ext3_gps.h"
int ext3_set_gps_location(struct inode *f_inode)
{
	/*TODO:locking*/
        struct gps_location *location_data = __get_gps_location();
        f_inode->i_latitude = cpu_to_le64(*((__u64 *)&location_data->latitude));/*cpu_to from fs.h*/
	f_inode->i_longitude = cpu_to_le64(*((__u64 *)&location_data->longitude));
	f_inode->i_accuracy = cpu_to_le32(*((__u32 *)&location_data->accuracy));
	//f_inode->i_coord_age = TODO: function implementation missing 
        return 0;
	/*TODO:locking*/

}

int ext3_get_gps_location(struct inode *f_inode, struct gps_location *location_data)
{
	__u64 lat,lon;
	__u32 accuracy;
	/*TODO:locking*/
	if (location_data == NULL || f_inode == NULL)
		return -1;
	lat = le64_to_cpu(f_inode->i_latitude);
	lon = le64_to_cpu(f_inode->i_latitude);
	accuracy = le32_to_cpu(f_inode->i_latitude);

//        &location_data->latitude = (double *)&(le64_to_cpu(f_inode->i_latitude));
//        &location_data->longitude = (double *)&(le64_to_cpu(f_inode->i_latitude));
//        &location_data->accuracy = (float *)&(le32_to_cpu(f_inode->i_latitude));
        return 0;
	/*TODO:locking*/
}

