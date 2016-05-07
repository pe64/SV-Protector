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
	{ 0x41086e, "module_layout" },
	{ 0x7d0bea0b, "per_cpu__current_task" },
	{ 0x3841ab01, "unregister_kprobe" },
	{ 0xdc714560, "register_kprobe" },
	{ 0x973873ab, "_spin_lock" },
	{ 0xd9a9bb30, "getname" },
	{ 0xb72397d5, "printk" },
	{ 0x1e6d26a8, "strstr" },
	{ 0x2b4c47df, "putname" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 7,
	.rhel_release = 572,
};
