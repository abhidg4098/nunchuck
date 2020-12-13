#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xedf9fcaf, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x2d5fa01f, "i2c_master_send" },
	{ 0x6fbed5b4, "device_remove_file" },
	{ 0x81474d67, "kmalloc_caches" },
	{ 0x5a34a45c, "__kmalloc" },
	{ 0xb279da12, "pv_lock_ops" },
	{ 0xf67a6595, "i2c_del_driver" },
	{ 0x3bc8f6a4, "i2c_transfer" },
	{ 0x4f00501b, "no_llseek" },
	{ 0x973873ab, "_spin_lock" },
	{ 0x105e2727, "__tracepoint_kmalloc" },
	{ 0x691b8d3d, "device_destroy" },
	{ 0xda7dd7bc, "queue_work" },
	{ 0xb8e0d489, "__register_chrdev" },
	{ 0x5a2191ac, "i2c_put_adapter" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xb8e7ce2c, "__put_user_8" },
	{ 0xea147363, "printk" },
	{ 0x36a4b0e5, "i2c_verify_client" },
	{ 0x3656bf5a, "lock_kernel" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0xbe499d81, "copy_to_user" },
	{ 0xb4390f9a, "mcount" },
	{ 0x13db8ac, "destroy_workqueue" },
	{ 0x44abb265, "device_create" },
	{ 0x46f66e6b, "__create_workqueue_key" },
	{ 0x97acb59b, "i2c_register_driver" },
	{ 0xe6e8f340, "device_create_file" },
	{ 0xb1f975aa, "unlock_kernel" },
	{ 0xbdc5daf4, "kmem_cache_alloc" },
	{ 0x668058fc, "i2c_smbus_xfer" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x1a2188d7, "i2c_master_recv" },
	{ 0x8db89810, "device_for_each_child" },
	{ 0x37a0cba, "kfree" },
	{ 0xf6c5f396, "class_destroy" },
	{ 0x9edbecae, "snprintf" },
	{ 0x4648d83, "i2c_get_adapter" },
	{ 0x733e4a90, "__class_create" },
	{ 0x945bc6a7, "copy_from_user" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "79A0883E42D8D464F2E2610");
