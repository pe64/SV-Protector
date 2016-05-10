#include <linux/ftrace.h>
#include <linux/kprobes.h>
#include <asm/unistd.h>
#include <linux/notifier.h>
#include <linux/profile.h>
#include <linux/threads.h>
#include <linux/cpumask.h>

/* comment the following line to shut me up */
#define INTERCEPT_DEBUG

#ifdef INTERCEPT_DEBUG
#define dbgprint(format,args...) \
	printk("intercept: function:%s-L%d: "format, __FUNCTION__, __LINE__, ##args);
#else
#define dbgprint(format,args...) do {} while(0);
#endif

static int sv_task_exit_notify(struct notifier_block *self, unsigned long val, void *data)
{
	struct task_struct *task;

	task = data;
	printk("file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__func__);
	printk("next = %p\n",self->next);
	printk("task = %s\n",task->comm);
	return 2;
}

struct notifier_block s_sv_nb = {
	.notifier_call = sv_task_exit_notify,
	.priority = 1,
	
};

int  process_init(void)
{
	int err;
	printk("file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__func__);
	err = profile_event_register(PROFILE_TASK_EXIT, &s_sv_nb);
	if(IS_ERR(ERR_PTR(err))){
		printk("file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__func__);
		return err;
	}
	printk("cpu = %d\n",num_online_cpus());
	return 0;
}

void process_fini(void)
{
	printk("file:%s,line:%d,func:%s\n",__FILE__,__LINE__,__func__);
	profile_event_unregister(PROFILE_TASK_EXIT, &s_sv_nb);
}

