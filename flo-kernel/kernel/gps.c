#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/namei.h>
#include <linux/gps.h>

static DEFINE_RWLOCK(gps_loc_lock);
static struct gps_location *__k_loc;
static struct gps_location *temp;
unsigned long long __timestamp;
static struct timeval *time;

unsigned long long __get_timestamp(void)
{
	/*TODO: need to add locking here and also avoid dead lock since
	* two resources are exposed*/
	do_gettimeofday(time);
		
	return time->tv_sec;
}

struct gps_location *__get_gps_location(void)
{
	read_lock(&gps_loc_lock);
	temp->latitude = __k_loc->latitude;
	temp->longitude = __k_loc->longitude;
	temp->accuracy = __k_loc->accuracy;
	read_unlock(&gps_loc_lock);
	return temp;
}


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

	ret = init_k_loc();

	if (ret != 1)
		return ret;

	k_loc = kmalloc(sizeof(struct gps_location), GFP_KERNEL);
	if (k_loc == NULL)
		return -ENOMEM;

	if (copy_from_user(k_loc, loc, sizeof(struct gps_location))) {
		pr_err("set_gps_location: copy_from_user failed.\n");
		return -EFAULT;
	}

	write_lock(&gps_loc_lock);
	__k_loc->latitude = k_loc->latitude;
	__k_loc->longitude = k_loc->longitude;
	__k_loc->accuracy = k_loc->accuracy;
	write_unlock(&gps_loc_lock);
	kfree(k_loc);

	return 0;
}


SYSCALL_DEFINE2(get_gps_location,
		const char __user *, pathname,
		struct gps_location __user *, loc)
{
	struct gps_location *k_loc = NULL;
	char *k_path = NULL;
	struct path *path = NULL;
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
		return -EINVAL;
	}

	k_path = kmalloc(path_len, GFP_KERNEL);
	if (k_path == NULL)
		return -ENOMEM;

	if (copy_from_user(k_path, pathname, path_len)) {
		pr_err("get_gps_location: copy_from_user failed for path.\n");
		return -EFAULT;
	}

	pr_err("get_gps_location: pathname: %s, path_len: %d.\n",
			k_path, path_len);
	
	ret = user_path(k_path, path);
	if (ret || path == NULL)
		return -EINVAL;
	ret = get_location_info(path->dentry->d_inode, k_loc);
	
	if (copy_to_user(loc, k_loc, sizeof(struct gps_location))) {
		pr_err("get_gps_location: copy_to_user failed for loc.\n");
		return -EFAULT;
	}
	return ret;
}
