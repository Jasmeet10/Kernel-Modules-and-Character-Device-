#include <linux/kernel.h>
#include <linux/device.h> 
#include <linux/fs.h>
#include <linux/uaccess.h>	
#include <linux/slab.h>	
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/module.h>

#define BUF_LEN 100

static int major;
static char kbuffer[20000] = {0};            

static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static struct task_struct *task;

static struct file_operations fops = {
  .read = device_read,
};

static struct miscdevice mydev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "process_list",
    .fops = &fops,
};

char * get_task_state(long state)
{
    switch (state)
    {
    case TASK_RUNNING:
        return "TASK_RUNNING";
        break;
    case TASK_INTERRUPTIBLE:
            return "TASK_INTERRUPTIBLE";
            break;
     case TASK_UNINTERRUPTIBLE:
            return "TASK_UNINTERRUPTIBLE";
            break;
    case TASK_STOPPED:
            return "TASK_STOPPED";
            break;
    case TASK_TRACED:
            return "TASK_TRACED";
            break;
    case EXIT_DEAD:
            return "EXIT_DEAD";
            break;
    case EXIT_ZOMBIE:
            return "EXIT_ZOMBIE";
            break;
    case TASK_WAKEKILL:
            return "TASK_WAKEKILL";
            break;
    case TASK_WAKING:
            return "TASK_WAKING";
            break;
    case TASK_PARKED:
        return "TASK_PARKED";
        break;
    case TASK_NOLOAD:
            return "TASK_NOLOAD";
            break;
     case TASK_NEW:
            return "TASK_NEW";
            break;
    case TASK_IDLE:
            return "TASK_IDLE";
            break;
    case EXIT_TRACE:
            return "EXIT_TRACE";
            break;
    case TASK_DEAD:
            return "TASK_DEAD";
            break;
    case TASK_STATE_MAX:
            return "TASK_STATE_MAX";
            break; 
    default:
        return "OTHER";
        break;
    }
}

static int __init my_init(void)
{
    printk(KERN_INFO "Device is Register");
    major = misc_register(&mydev);
    if (major < 0){
        printk(KERN_ALERT "Registering char device failed with %d\n", major);
	  return major;
    }
    return 0;
}

static void __exit my_exit(void)
{
    misc_deregister(&mydev);
    printk(KERN_ALERT "Hope you have a nice day, Good Bye!");
}

static ssize_t device_read(struct file *filp,char *buffer,size_t length,loff_t * offset){

    int error_count = 0;

	if(error_count < 0)
	{
		return error_count;
	}
    for( task = &init_task; (task = next_task(task)) != &init_task; ) {
        sprintf(kbuffer + strlen(kbuffer), "PID=%d  PPID=%d  CPU=%d  STATE=%s\n", task->pid, task->real_parent->pid, task_cpu(task), get_task_state(task->state));
        error_count = copy_to_user(buffer, kbuffer, strlen(kbuffer));
	}
	return error_count;
}

MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
