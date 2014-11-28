#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>

#include <linux/gps.h>

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, loc)
{
	struct gps_location *k_loc = NULL;
	int uid;

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

	k_loc = kmalloc(sizeof(struct gps_location), GFP_KERNEL);
	if (k_loc == NULL) {
		pr_err("set_gps_location: couldn't allocate k_loc\n");
		return -ENOMEM;
	}

	if (copy_from_user(k_loc, loc, sizeof(struct gps_location))) {
		pr_err("set_gps_location: copy_from_user failed.\n");
		return -EFAULT;
	}
/*
	//unable to do double and float related operations!!!

	pr_err("set_gps_location: lat: %d, long: %d, acc: %d\n",
			(int) k_loc->latitude,
			(int) k_loc->longitude,
			(int) k_loc->accuracy);
*/
	return 0;
}


SYSCALL_DEFINE2(get_gps_location,
		const char __user *, pathname,
		struct gps_location __user *, loc)
{
	struct gps_location *k_loc = NULL;
	char *k_path = NULL;
	int path_len = 0;

	if (loc == NULL) {
		pr_err("get_gps_location: loc is NULL.\n");
		return -EINVAL;
	}

	if (pathname == NULL) {
		pr_err("get_gps_location: pathname is NULL.\n");
		return -EINVAL;
	}

	k_loc = kmalloc(sizeof(struct gps_location), GFP_KERNEL);
	if (k_loc == NULL) {
		pr_err("get_gps_location: couldn't allocate k_loc\n");
		return -ENOMEM;
	}

	if (copy_from_user(k_loc, loc, sizeof(struct gps_location))) {
		pr_err("get_gps_location: copy_from_user failed for loc.\n");
		return -EFAULT;
	}


	//need to check for null-terminated string
	path_len = strlen(pathname);
	if (path_len <= 0) {
		pr_err("get_gps_location: pathname is invalid\n");
		return -EINVAL;
	}

	k_path = kmalloc(path_len, GFP_KERNEL);
	if (k_path == NULL) {
		pr_err("get_gps_location: couldn't allocate k_path.\n");
		return -ENOMEM;
	}

	if (copy_from_user(k_path, pathname, path_len)) {
		pr_err("get_gps_location: copy_from_user failed for path.\n");
	}


	pr_err("get_gps_location: pathname: %s, path_len: %d.\n",
			k_path, path_len);


	/*
	   pr_err("set_gps_location: lat: %d, long: %d, acc: %d\n",
	   (int) k_loc->latitude,
	   (int) k_loc->longitude,
	   (int) k_loc->accuracy);
	 */
	return 0;
}
