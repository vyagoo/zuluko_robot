include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/kernel.mk

PKG_NAME:=testdemo
PKG_VERSION:=1
PKG_RELEASE:=1

PKG_BUILD_DIR := $(COMPILE_DIR)/$(PKG_NAME)

include $(BUILD_DIR)/package.mk

define Package/testdemo
  SECTION:=utils
  CATEGORY:=Vego
  TITLE:=CedarX2.6 recorder utils for allwinner
  DEPENDS:= +standby +libcutils +libutils +media-server +wifiswitch

endef

define Package/testdemo/description
	CedarX2.6 recorder
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -r ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/ \
		ARCH="$(TARGET_ARCH)" \
		AR="$(TARGET_AR)" \
		CC="$(TARGET_CC)" \
		CXX="$(TARGET_CXX)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/testdemo/install
	$(INSTALL_DIR) $(1)/usr/bin/opencv
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/testdemo					 $(1)/usr/bin/opencv/
#	$(CP) mono_robot_vacuum_controller $(1)/usr/bin/opencv/
	$(CP) zuluko.init $(1)/etc/init.d/zuluko
endef

$(eval $(call BuildPackage,testdemo))
