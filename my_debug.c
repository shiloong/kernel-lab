// SPDX-License-Identifier: GPL-2.0
/**
 * My trivial kernel module for debugging
 *
 * Copyright (C) 2017 Shiloong Zhang <zhangshile@gmail.com>
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/string.h>

#include <linux/kobject.h>
#include <linux/sysfs.h>

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define MY_KNOB_NAME	"my_knob"

static int my_knob = 42;

static ssize_t my_knob_show(struct kobject *kobj,
		       struct kobj_attribute *attr, char *buf)
{
	ssize_t count;

	count = sprintf(buf, "%d\n", my_knob);

	return count;
}

static ssize_t my_knob_store(struct kobject *kobj,
		       struct kobj_attribute *attr,
		       const char *buf, size_t count)
{
	sscanf(buf, "%d", &my_knob);

	return count;
}

static struct kobj_attribute my_attr = __ATTR_RW(my_knob);

static int my_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%d\n", my_knob);
	return 0;
}

static int my_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, my_proc_show, NULL);
}

static const struct file_operations my_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= my_proc_open,
	.read		= seq_read,
	.llseek 	= seq_lseek,
	.release	= single_release,
};

static int __init my_init(void)
{
	int ret = 0;
	static struct proc_dir_entry *my_proc;

	ret = sysfs_create_file(kernel_kobj, &my_attr.attr);
	if (ret) {
		pr_err("MY_DEBUG: sysfs_create_file failed with ret=%d\n",
		       ret);
		return -ENOMEM;
	}

	my_proc = proc_create(MY_KNOB_NAME, 0, NULL, &my_proc_fops);
	if (!my_proc) {
		pr_err("MY_DEBUG: proc_create failed\n");
		return -ENOMEM;
	}

	pr_info("MY_DEBUG: hello, kernel\n");

	return ret;
}

static void __exit my_exit(void)
{
	sysfs_remove_file(kernel_kobj, &my_attr.attr);
	remove_proc_entry(MY_KNOB_NAME, NULL);

	pr_info("MY_DEBUG: bye, kernel!\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Shiloong zhangshile@gmail.com");
MODULE_DESCRIPTION("Shiloong's kernel module for debugging");
