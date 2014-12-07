#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/namei.h>
#include <linux/gps.h>
//to be removed
#include "../fs/ext3/ext3.h"

DEFINE_SPINLOCK(gps_loc_lock);

struct gps_location __k_loc;
unsigned long long __timestamp = ~0;

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, loc)
{
	struct gps_location *k_loc = NULL;
	struct timespec time;

	if (!capable(CAP_SYS_ADMIN)) {
		pr_err("set_gps_location: called by non-admin user !!\n");
		return -EPERM;
	}

	if (loc == NULL) {
		pr_err("set_gps_location: loc is NULL.\n");
		return -EINVAL;
	}

	k_loc = kmalloc(sizeof(struct gps_location), GFP_KERNEL);
	if (k_loc == NULL)
		return -ENOMEM;

	if (copy_from_user(k_loc, loc, sizeof(struct gps_location))) {
		pr_err("set_gps_location: copy_from_user failed.\n");
		kfree(k_loc);
		return -EFAULT;
	}

	spin_lock(&gps_loc_lock);
	__k_loc.latitude = k_loc->latitude;
	__k_loc.longitude = k_loc->longitude;
	__k_loc.accuracy = k_loc->accuracy;
	time = CURRENT_TIME_SEC;
	__timestamp = time.tv_sec;
	spin_unlock(&gps_loc_lock);
	kfree(k_loc);

	return 0;
}


SYSCALL_DEFINE2(get_gps_location,
		const char __user *, pathname,
		struct gps_location __user *, loc)
{
	struct gps_location *k_loc = NULL;
	struct path st_path;
	struct inode *inode = NULL;
	struct ext3_inode_info *ei;
	int path_len = 0;
	int ret = 0;

	if (loc == NULL) {
		pr_err("get_gps_location: loc is NULL.\n");
		return -EINVAL;
	}

	if (pathname == NULL) {
		pr_err("get_gps_location: pathname is NULL.\n");
		return -EINVAL;
	}

	k_loc = kmalloc(sizeof(struct gps_location), GFP_KERNEL);
	if (k_loc == NULL)
		return -ENOMEM;


	path_len = strlen(pathname) + 1;
	if (path_len <= 0) {
		pr_err("get_gps_location: pathname is invalid\n");
		kfree(k_loc);
		return -EINVAL;
	}

	ret = user_path(pathname, &st_path);
	if (ret) {
		pr_err("\nerror occured while trying to get path %d", ret);
		kfree(k_loc);
		return -EINVAL;
	}

	inode = st_path.dentry->d_inode;

	if (inode->i_uid != current_uid() && inode->i_uid != current_euid()){
		path_put(&st_path);
		kfree(k_loc);
		return -EPERM;
	}

	ei = EXT3_I(inode);
	if (ei->i_coord_age == ~0) {
		pr_err("get_gps_location: No GPS data in inode\n");
		path_put(&st_path);
		kfree(k_loc);
		return -ENODEV;
	}
	ret = get_location_info(inode, k_loc);

	pr_err("\n get_gps_loc_gps_SYSCALL_lat: %llx", *(__u64 *)&k_loc->latitude);
	pr_err("\n get_gps_loc_gps_SYSCALL_lon: %llx", *(__u64 *)&k_loc->longitude);
	pr_err("\n get_gps_loc_gps_SYSCALL_accu: %x", *(__u32 *)&k_loc->accuracy);

	if (copy_to_user(loc, k_loc, sizeof(struct gps_location))) {
		pr_err("get_gps_location: copy_to_user failed for loc.\n");
		kfree(k_loc);
		path_put(&st_path);
		return -EFAULT;
	}
	kfree(k_loc);
	path_put(&st_path);
	return ret;
}

SYSCALL_DEFINE1(get_xxx, unsigned int, addr)
{
	struct ext3_inode_info *ei = NULL;

	spin_lock(&gps_loc_lock);
	pr_err("\n get_xxx_lat: %llx", *(__u64 *)&__k_loc.latitude);
	pr_err("\n get_xxx_lon: %llx", *(__u64
				*)&__k_loc.longitude);
	pr_err("\n get_xxx_accu: %x", *(__u32
				*)&__k_loc.accuracy);
	spin_unlock(&gps_loc_lock);

	if (addr) {
		ei = (struct ext3_inode_info *) addr;
		
		pr_err("Pointer is: 0x%x\n", (unsigned int)ei);
		spin_lock(&ei->inode_gps_lock);

		pr_err("\n 1. ext3_set_gps_location_lat::: %llx", ei->i_latitude);
		pr_err("\n 1. ext3_set_gps_location_lon::: %llx", ei->i_longitude);
		pr_err("\n 1. ext3_set_gps_location_accu::: %x", ei->i_accuracy);
		pr_err("\n 1. ext3_set_gps_location_age::: %x", ei->i_coord_age);
		pr_err("\n ext3_inode_info = 0x%x\n", (unsigned int)ei);
		spin_unlock(&ei->inode_gps_lock);
	}

	return 0;
}
