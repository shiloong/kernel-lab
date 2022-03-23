#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/ratelimit.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

static int fun = 42;
static struct task_struct *ftsk;

static int ftsk_fn(void *ignore)
{
	while (!kthread_should_stop()) {
		pr_warn_ratelimited("[ - ] FEINENG: flooding?\n");
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(HZ/10);
	}

	return 0;
}

static ssize_t fun_show(struct kobject *kobj,
			    struct kobj_attribute *attr, char *buf)
{
	ssize_t count;

	count = sprintf(buf, "Have fun: %d\n", fun);

	return count;
}

static ssize_t fun_store(struct kobject *kobj,
			     struct kobj_attribute *attr,
			     const char *buf, size_t count)
{
	sscanf(buf, "%d", &fun);

	switch (fun) {
	case 7:
		if (ftsk) {
			pr_info("FEINENG: kthread 'kfeineng' "
				"already running\n");
			break;
		}

		ftsk = kthread_run(ftsk_fn, NULL, "kfeineng");
		if (IS_ERR(ftsk)) {
			pr_err("[ - ] ERROR: cannot create kthread\n");
			ftsk = NULL;
		}

		break;
	case 0:
		if (ftsk) {
			kthread_stop(ftsk);
			ftsk = NULL;
		}

		break;
	default:
		pr_warn("FEINENG: unknown command, do nothing\n");
	}

	return count;
}

static struct kobj_attribute fun_attr = __ATTR_RW(fun);

static int __init fun_init(void)
{
	int ret = 0;

	pr_info("[ - ] FEINENG: Let's go...\n");

	ret = sysfs_create_file(kernel_kobj, &fun_attr.attr);
	if (ret) {
		pr_err("FEINENG: sysfs_create_file failed with ret=%d\n", ret);
		return -ENOMEM;
	}

	return 0;
}

static void __exit fun_exit(void)
{
	if (ftsk) {
		kthread_stop(ftsk);
		ftsk = NULL;
	}

	sysfs_remove_file(kernel_kobj, &fun_attr.attr);

	pr_info("[ - ] FEINENG: Have fun, bye!\n");
}

module_init(fun_init);
module_exit(fun_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Feineng");
MODULE_DESCRIPTION("Just for fun!");
MODULE_VERSION("0.1");
