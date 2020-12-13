/*
  Asynchronous i2c character device
*/

#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include <linux/moduleparam.h>
#include<linux/slab.h>
#include<linux/workqueue.h>

//for universal distribution, to prevent kernel warning
MODULE_LICENSE("Dual BSD/GPL");

#define QUEUE_NAME "work_queue"
struct workqueue_struct *async_queue;

struct async_t
{
  struct work_struct async_work;
  int status;
};

struct async_t *p_instance1;

//##################################
void work_routine(void)
{
  printk("inside the routine \n");
  return;
}

static int __init init_i2c_async(void)
{
  struct work_struct launch_work;
  int ret_val = -1;
  p_instance = NULL;
  async_queue = create_workqueue(QUEUE_NAME);
  p_instance = kmalloc(sizeof(struct async_t), GFP_KERNEL);
  
  if((NULL == async_queue) || (NULL == p_instance))
  {
   printk(KERN_ALERT "Could not initialize %s \n", (async_queue == NULL) ? "async_queue" : "p_instance");

   if(p_instance)
    kfree(p_instance);

   return -ENOMEM;
  }
  
  launch_work = container_of(p_instance, struct async_t, async_work);
  
  INIT_WORK(launch_work, routine, NULL);
  
  ret_val = queue_work(async_queue, &launch_work);
  
  if(ret_val != 0)
   {
     printk(KERN_ALERT "Could not load the work to the queue \n");
     return -ENOENT;
   }

  printk("Module i2c_dev_async is successfully loaded \n");
  return 0;
}

void __exit release_i2c_async(void)
{
 flush_workqueue(async_queue);
 destroy_workqueue(async_queue);
 
 kfree(p_instance);
 printk("Removing the module i2c_dev_async \n");
 return;
}

module_init(init_i2c_async);
module_exit(release_i2c_async);
