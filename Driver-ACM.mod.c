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
	{ 0x54b1fac6, "__ubsan_handle_load_invalid_value" },
	{ 0x45f56f9f, "usb_unlink_urb" },
	{ 0x69acdf38, "memcpy" },
	{ 0xa99497b1, "usb_autopm_get_interface_async" },
	{ 0xa0f0ad18, "usb_anchor_urb" },
	{ 0xfe4fc7fc, "usb_autopm_get_interface" },
	{ 0xe73e0769, "usb_control_msg" },
	{ 0x48476967, "usb_autopm_put_interface" },
	{ 0x5b5bb31b, "__dynamic_dev_dbg" },
	{ 0xbd394d8, "tty_termios_baud_rate" },
	{ 0xf72a65ea, "tty_get_char_size" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x200fbf5f, "cdc_parse_cdc_header" },
	{ 0x78ffcd7a, "usb_ifnum_to_if" },
	{ 0xb88db70c, "kmalloc_caches" },
	{ 0x4454730e, "kmalloc_trace" },
	{ 0xe6341857, "tty_port_init" },
	{ 0x19af8caa, "usb_get_intf" },
	{ 0xb8f11603, "idr_alloc" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0xb8d6ea64, "usb_alloc_coherent" },
	{ 0x2ec9681e, "usb_alloc_urb" },
	{ 0x9fe19262, "usb_free_urb" },
	{ 0x2421d970, "usb_free_coherent" },
	{ 0xa9df55d9, "device_create_file" },
	{ 0x9060e2be, "usb_driver_claim_interface" },
	{ 0x91b60e7a, "tty_port_register_device" },
	{ 0xb940ca68, "_dev_info" },
	{ 0xefc0278d, "device_remove_file" },
	{ 0x1bcb7020, "usb_clear_halt" },
	{ 0x6b269da1, "usb_driver_release_interface" },
	{ 0x6ebe366f, "ktime_get_mono_fast_ns" },
	{ 0xc8dcc62a, "krealloc" },
	{ 0x673f9564, "tty_port_tty_hangup" },
	{ 0x2251af21, "tty_ldisc_ref" },
	{ 0x6383b27c, "__x86_indirect_thunk_rdx" },
	{ 0x1b8a560a, "tty_ldisc_deref" },
	{ 0xe2964344, "__wake_up" },
	{ 0xfc84ced7, "__tty_insert_flip_char" },
	{ 0x1e77b97b, "tty_flip_buffer_push" },
	{ 0xa648e561, "__ubsan_handle_shift_out_of_bounds" },
	{ 0x20978fb9, "idr_find" },
	{ 0x1f1a9622, "tty_standard_install" },
	{ 0x296695f, "refcount_warn_saturate" },
	{ 0xaad8c7d6, "default_wake_function" },
	{ 0xb6a5193f, "pcpu_hot" },
	{ 0x4afb2238, "add_wait_queue" },
	{ 0x1000e51, "schedule" },
	{ 0x37110088, "remove_wait_queue" },
	{ 0x2d3385d3, "system_wq" },
	{ 0xb2fcb56d, "queue_delayed_work_on" },
	{ 0xe88b33, "tty_insert_flip_string_fixed_flag" },
	{ 0x23c9fcc2, "usb_poison_urb" },
	{ 0x5a03d771, "tty_port_tty_get" },
	{ 0xef7db010, "tty_vhangup" },
	{ 0x63367036, "tty_kref_put" },
	{ 0x9fa7184a, "cancel_delayed_work_sync" },
	{ 0x696d5547, "tty_unregister_device" },
	{ 0x8e5d9ea4, "usb_autopm_get_interface_no_resume" },
	{ 0x58a3832d, "usb_kill_urb" },
	{ 0x6a7d5eda, "tty_port_tty_wakeup" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x84cd9e22, "usb_submit_urb" },
	{ 0x5981fc21, "_dev_err" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0x1545fa8b, "usb_autopm_put_interface_async" },
	{ 0x8427cc7b, "_raw_spin_lock_irq" },
	{ 0x4b750f53, "_raw_spin_unlock_irq" },
	{ 0x97c92eba, "usb_unpoison_urb" },
	{ 0x113e6925, "usb_get_from_anchor" },
	{ 0xf542ac60, "tty_port_put" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x8b6dad90, "__tty_alloc_driver" },
	{ 0x67b27ec1, "tty_std_termios" },
	{ 0x58a5e007, "tty_register_driver" },
	{ 0x4523577f, "usb_register_driver" },
	{ 0x122c3a7e, "_printk" },
	{ 0x93be9d5a, "tty_unregister_driver" },
	{ 0x1a46e82b, "tty_driver_kref_put" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x7665a95b, "idr_remove" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x980532af, "usb_put_intf" },
	{ 0x37a0cba, "kfree" },
	{ 0x14321055, "tty_port_hangup" },
	{ 0xd9f88298, "tty_port_close" },
	{ 0x98a930b6, "tty_port_open" },
	{ 0x74bc459a, "usb_deregister" },
	{ 0x8e17b3ae, "idr_destroy" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0x2fa5cadd, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:v2341p0043d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2341p8036d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "B051443D477AFDFFE289F0E");
