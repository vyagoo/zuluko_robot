#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/touchscreen-gt9xx
  SUBMENU:=$(INPUT_MODULES_MENU)
  TITLE:=touchscreen gt9xx support
  DEPENDS:=+kmod-input-core
  FILES:=$(LINUX_DIR)/drivers/input/touchscreen/gt9xx/gt9xx_ts.ko
  AUTOLOAD:=$(call AutoProbe,gt9xx_ts)
endef

define KernelPackage/touchscreen-gt9xx/description
 Kernel modules for support touchscreen gt9xx
endef

$(eval $(call KernelPackage,touchscreen-gt9xx))

define KernelPackage/gsensor-da380
  SUBMENU:=$(INPUT_MODULES_MENU)
  TITLE:=gsensor da380 support
  DEPENDS:=+kmod-input-core
  FILES:=$(LINUX_DIR)/drivers/hwmon/da380/da380.ko
  AUTOLOAD:=$(call AutoLoad,60,da380)
endef

define KernelPackage/gsensor-da380/description
 Kernel modules for support da380 gsensor
endef

$(eval $(call KernelPackage,gsensor-da380))