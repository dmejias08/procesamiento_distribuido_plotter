#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

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
	{ 0xc5843c7c, "usb_serial_generic_open" },
	{ 0xbb9908b0, "usb_serial_deregister_drivers" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x35ae4dd3, "usb_serial_register_drivers" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xb3a762f, "usb_serial_generic_close" },
	{ 0xb940ca68, "_dev_info" },
	{ 0x2fa5cadd, "module_layout" },
};

MODULE_INFO(depends, "usbserial");

MODULE_ALIAS("usb:v2341p0043d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2341p8036d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "2373F9D07B7DE5A07F16CC5");
