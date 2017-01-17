#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x22813daa, "struct_module" },
	{ 0xf9a482f9, "msleep" },
	{ 0xc288f8ce, "malloc_sizes" },
	{ 0x973873ab, "_spin_lock" },
	{ 0x8949858b, "schedule_work" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0x6835cd6e, "sysfs_remove_group" },
	{ 0x7d11c268, "jiffies" },
	{ 0x343a1a8, "__list_add" },
	{ 0x2433290d, "misc_register" },
	{ 0x949b88e1, "del_timer_sync" },
	{ 0xb72397d5, "printk" },
	{ 0xde091edb, "sysfs_create_group" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0x521445b, "list_del" },
	{ 0x748caf40, "down" },
	{ 0x6b5a2d1, "mod_timer" },
	{ 0xf11543ff, "find_first_zero_bit" },
	{ 0xd8b08694, "kmem_cache_alloc" },
	{ 0xb6b108f6, "put_device" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x920e963b, "dev_driver_string" },
	{ 0xefaf524e, "get_device" },
	{ 0xa40adf6d, "init_timer" },
	{ 0x37a0cba, "kfree" },
	{ 0x2e60bace, "memcpy" },
	{ 0x801678, "flush_scheduled_work" },
	{ 0x3f1899f1, "up" },
	{ 0x374ed073, "scnprintf" },
	{ 0xf2a644fb, "copy_from_user" },
	{ 0xf520dd7a, "misc_deregister" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "04E1968B355500EF73A3B6D");
