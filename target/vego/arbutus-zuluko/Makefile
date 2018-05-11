#
# Copyright (C) 2013-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=arbutus-zuluko
BOARDNAME:=arbutus-zuluko
FEATURES:=fpu usb ext4 display rtc
CPU_TYPE:=cortex-a7
CPU_SUBTYPE:=neon

KERNEL_PATCHVER:=3.4
KERNELNAME:=zImage

include $(BUILD_DIR)/target.mk

DEFAULT_PACKAGES +=

$(eval $(call BuildTarget))
