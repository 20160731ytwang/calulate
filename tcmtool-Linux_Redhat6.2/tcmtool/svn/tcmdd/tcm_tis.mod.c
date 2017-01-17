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
	{ 0x22813daa, "struct_module" },
	{ 0x259e2e95, "tcm_store_cancel" },
	{ 0x1cdcc0f, "tcm_show_caps_1_2" },
	{ 0x9dd7221, "tcm_show_temp_deactivated" },
	{ 0xc689951b, "tcm_show_owned" },
	{ 0x3d0d10e7, "tcm_show_active" },
	{ 0xe8cb865f, "tcm_show_enabled" },
	{ 0xb1917895, "tcm_show_pcrs" },
	{ 0x2c0a9a38, "tcm_show_pubek" },
	{ 0xe295be6a, "tcm_release" },
	{ 0x4c52f955, "tcm_open" },
	{ 0xa0fccdab, "tcm_write" },
	{ 0x66b31f5b, "tcm_read" },
	{ 0xcd6376e2, "no_llseek" },
	{ 0xae9e8ead, "platform_bus_type" },
	{ 0xfbf92453, "param_get_bool" },
	{ 0xa925899a, "param_set_bool" },
	{ 0xe691b4c1, "tcm_pm_suspend" },
	{ 0xd12b3e5c, "tcm_calc_ordinal_duration" },
	{ 0x23b6985e, "pnp_register_driver" },
	{ 0x76cd3c4c, "platform_device_register_simple" },
	{ 0xb465f5a, "driver_register" },
	{ 0x731e4382, "pnp_get_resource" },
	{ 0x49f34132, "tcm_startup" },
	{ 0x4cff545f, "tcm_get_timeouts" },
	{ 0x343a1a8, "__list_add" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x42c8de35, "ioremap_nocache" },
	{ 0xbde55ed9, "tcm_register_hardware" },
	{ 0x9ccb2622, "finish_wait" },
	{ 0xd62c833f, "schedule_timeout" },
	{ 0x33d92f9a, "prepare_to_wait" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0xd6d20588, "per_cpu__current_task" },
	{ 0xf9a482f9, "msleep" },
	{ 0x7d11c268, "jiffies" },
	{ 0x91d0c2ba, "pnp_unregister_driver" },
	{ 0xaa1f715b, "driver_unregister" },
	{ 0xc01ab00e, "platform_device_unregister" },
	{ 0x323a823b, "tcm_remove_hardware" },
	{ 0x521445b, "list_del" },
	{ 0xedc03953, "iounmap" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x436c2179, "iowrite32" },
	{ 0xe484e35f, "ioread32" },
	{ 0x973873ab, "_spin_lock" },
	{ 0x727c4f3, "iowrite8" },
	{ 0xf10de535, "ioread8" },
	{ 0x952c5afb, "tcm_pm_resume" },
	{ 0xb72397d5, "printk" },
	{ 0x920e963b, "dev_driver_string" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=tcm";


MODULE_INFO(srcversion, "87A0D39F657C47165B1B331");
