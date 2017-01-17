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
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0xf8e3dbd2, "struct_module" },
	{ 0xf30740f9, "pnp_unregister_driver" },
	{ 0x521445b, "list_del" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x436c2179, "iowrite32" },
	{ 0xf0ed0ced, "pnp_register_driver" },
	{ 0x478021be, "driver_unregister" },
	{ 0x385f219d, "platform_device_unregister" },
	{ 0x92872675, "platform_device_register_simple" },
	{ 0x4c5069e2, "driver_register" },
	{ 0x5611e4ec, "param_get_bool" },
	{ 0xdc76cbcb, "param_set_bool" },
	{ 0x62eb2329, "platform_bus_type" },
	{ 0x5869dc9a, "tcm_pm_resume" },
	{ 0xbfa1d720, "tcm_pm_suspend" },
	{ 0x6373426, "tcm_remove_hardware" },
	{ 0xedc03953, "iounmap" },
	{ 0x5368dcb4, "tcm_startup" },
	{ 0x42cd924b, "tcm_get_timeouts" },
	{ 0x46d33fe1, "list_add" },
	{ 0x429328d9, "_spin_lock" },
	{ 0xe484e35f, "ioread32" },
	{ 0x9eac042a, "__ioremap" },
	{ 0x43aa175e, "tcm_register_hardware" },
	{ 0x30ffbad3, "tcm_store_cancel" },
	{ 0xf6871acc, "tcm_show_caps" },
	{ 0xf9579297, "tcm_show_temp_deactivated" },
	{ 0xe3158831, "tcm_show_owned" },
	{ 0x5e30bb6d, "tcm_show_active" },
	{ 0xaff9f7f0, "tcm_show_enabled" },
	{ 0x2993028, "tcm_show_pcrs" },
	{ 0x8cf27087, "tcm_show_pubek" },
	{ 0x2ce9ee65, "tcm_release" },
	{ 0xba2173c8, "tcm_open" },
	{ 0x3d614c46, "tcm_write" },
	{ 0xe7d3e18c, "tcm_read" },
	{ 0x6f428a89, "no_llseek" },
	{ 0xbeb165b, "tcm_calc_ordinal_duration" },
	{ 0xdd132261, "printk" },
	{ 0x2cc5eafc, "dev_driver_string" },
	{ 0xf9a482f9, "msleep" },
	{ 0x7d11c268, "jiffies" },
	{ 0x727c4f3, "iowrite8" },
	{ 0xf10de535, "ioread8" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=tcm";


MODULE_INFO(srcversion, "34BAC5AA38E99A9CD4FE353");
