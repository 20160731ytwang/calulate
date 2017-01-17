#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0xf8e3dbd2, "struct_module" },
	{ 0x5a34a45c, "__kmalloc" },
	{ 0xf9a482f9, "msleep" },
	{ 0x429328d9, "_spin_lock" },
	{ 0x7ad5eec9, "schedule_work" },
	{ 0x1d26aa98, "sprintf" },
	{ 0x147a4e12, "sysfs_remove_group" },
	{ 0x7d11c268, "jiffies" },
	{ 0xda4008e6, "cond_resched" },
	{ 0x48836560, "__kzalloc" },
	{ 0x9b5d2f6f, "misc_register" },
	{ 0xc659d5a, "del_timer_sync" },
	{ 0xde0bdcff, "memset" },
	{ 0xc16fe12d, "__memcpy" },
	{ 0xdd132261, "printk" },
	{ 0x94ba1f8e, "sysfs_create_group" },
	{ 0xbe499d81, "copy_to_user" },
	{ 0x521445b, "list_del" },
	{ 0xf3b39202, "mod_timer" },
	{ 0xb3d2bf12, "find_first_zero_bit" },
	{ 0xaf9229c9, "put_device" },
	{ 0x2cc5eafc, "dev_driver_string" },
	{ 0x5d407156, "get_device" },
	{ 0x19cacd0, "init_waitqueue_head" },
	{ 0xa5c89579, "init_timer" },
	{ 0x37a0cba, "kfree" },
	{ 0x801678, "flush_scheduled_work" },
	{ 0x46d33fe1, "list_add" },
	{ 0x7bb4de17, "scnprintf" },
	{ 0x1042cbb5, "__up_wakeup" },
	{ 0x7a7ef853, "__down_failed" },
	{ 0x945bc6a7, "copy_from_user" },
	{ 0x928c3f70, "misc_deregister" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "204E0830761B73040E0A4C2");
