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

//static DEFINE_RWLOCK(gps_loc_lock);
DEFINE_SPINLOCK(gps_loc_lock);

struct gps_location __k_loc;
unsigned long long __timestamp;
static struct timeval time;

unsigned long long __get_timestamp(void)
{
	/*TODO: need to add locking here and also avoid dead lock since
	* two resources are exposed*/
	
	do_gettimeofday(&time);
	pr_err("\n get_timestamp\n");	
	return time.tv_sec;
}

/*
struct gps_location *__get_gps_location(void)
{
	read_lock(&gps_loc_lock);
	temp.latitude = __k_loc.latitude;
	temp.longitude = __k_loc.longitude;
	temp.accuracy = __k_loc.accuracy;
	pr_err("\n get_gps_loc_gpsc_lat: %llx", *(__u64 *)&temp.latitude);
	pr_err("\n get_gps_loc_gpsc_lon: %llx", *(__u64 *)&temp.longitude);
	pr_err("\n get_gps_loc_gpsc_accu: %x", *(__u32 *)&temp.accuracy);
	read_unlock(&gps_loc_lock);
	pr_err("\nafter read_unlcok get_gps_loc\n");
	return &temp;
}
*/

//TODO: Remove
/*
static int init_k_loc(void)
{
	if (__k_loc == NULL) {
		__k_loc = kmalloc(sizeof(struct gps_location), GFP_KERNEL);
		if (__k_loc == NULL) {
			pr_err("init_k_loc: couldn't allocate __k_loc\n");
			return -ENOMEM;
		}
	}

	if (temp == NULL) {
		temp = kmalloc(sizeof(struct gps_location), GFP_KERNEL);
		if (temp == NULL) {
			pr_err("init_k_loc: couldn't allocate temp\n");
			return -ENOMEM;
		}
	}
	
	if (time == NULL) {
		time =  kmalloc(sizeof(struct timeval), GFP_KERNEL);
                if (time == NULL) {
                        pr_err("init_k_loc: couldn't allocate time\n");
                        return -ENOMEM;
                }
	}
	return 1;
}
*/

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, loc)
{
	struct gps_location *k_loc = NULL;
	int uid;
	int ret;

	read_lock(&tasklist_lock);
	if (current == NULL || current->real_cred == NULL) {
		pr_err("set_gps_location: current task is invalid\n");
		read_unlock(&tasklist_lock);
		return -EFAULT;
	}

	uid = current->real_cred->uid;
	read_unlock(&tasklist_lock);

	if (uid != 0) {
		pr_err("set_gps_location: called by non-root user !!\n");
		return -EACCES;
	}

	if (loc == NULL) {
		pr_err("set_gps_location: loc is NULL.\n");
		return -EINVAL;
	}

	//for debugging
	/*
	ret = init_k_loc();

	if (ret != 1)
		return ret;
	*/

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
	spin_unlock(&gps_loc_lock);
	kfree(k_loc);

	return 0;
}


SYSCALL_DEFINE2(get_gps_location,
		const char __user *, pathname,
		struct gps_location __user *, loc)
{
	struct gps_location *k_loc = NULL;
//	char *k_path = NULL;
	struct path st_path;
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

/*	k_path = kmalloc(path_len, GFP_KERNEL);
	if (k_path == NULL)
		return -ENOMEM;

	if (copy_from_user(k_path, pathname, path_len)) {
		pr_err("get_gps_location: copy_from_user failed for path.\n");
		return -EFAULT;
	}

	pr_err("get_gps_location: pathname: %s, path_len: %d.\n",
			k_path, path_len);*/
	
	ret = user_path(pathname, &st_path);
	if (ret) {
		pr_err("\nerror occured while trying to get path %d", ret);
		kfree(k_loc);
		return -EINVAL;
	}
	ret = get_location_info(st_path.dentry->d_inode, k_loc);
	
	pr_err("\n get_gps_loc_gps_SYSCALL_lat: %llx", *(__u64 *)&k_loc->latitude);
	pr_err("\n get_gps_loc_gps_SYSCALL_lon: %llx", *(__u64 *)&k_loc->longitude);
	pr_err("\n get_gps_loc_gps_SYSCALL_accu: %x", *(__u32 *)&k_loc->accuracy);
	if (copy_to_user(loc, k_loc, sizeof(struct gps_location))) {
		pr_err("get_gps_location: copy_to_user failed for loc.\n");
		kfree(k_loc);
		return -EFAULT;
	}
	kfree(k_loc);
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
