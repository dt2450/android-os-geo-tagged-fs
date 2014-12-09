#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/namei.h>
#include <linux/gps.h>
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

	if (inode_permission(inode, MAY_READ)) {
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
