#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xd9726f80, "module_layout" },
	{ 0x46dce43, "device_destroy" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x64b60eb0, "class_destroy" },
	{ 0xb7ad68f3, "device_create" },
	{ 0xa946dcde, "__class_create" },
	{ 0x4102c7cb, "__register_chrdev" },
	{ 0x1e20e5b6, "filp_close" },
	{ 0x69af1880, "current_task" },
	{ 0x974db752, "kernel_write" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0xe96d683c, "find_vma" },
	{ 0x72d79d83, "pgdir_shift" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0x1d19f77b, "physical_mask" },
	{ 0x50d4de4, "pv_ops" },
	{ 0xf7cbd1dd, "filp_open" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x56470118, "__warn_printk" },
	{ 0xe271efb4, "try_module_get" },
	{ 0x50f6845f, "module_put" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "8E3437709356E16B0463A80");
