#
# Copyright (C) 2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/target.mk

PKG_NAME := ubus-server
PKG_VERSION := 0.26.0
PKG_RELEASE := $(PKG_SOURCE_VERSION)
PKG_USE_MIPS16 := 0

include $(INCLUDE_DIR)/package.mk

# 使用cmake编译
include $(INCLUDE_DIR)/cmake.mk

define Package/$(PKG_NAME)/Default
	SECTION := sxx
	CATEGORY := Sxx
	TITLE := $(PKG_NAME)
	DEPENDS := +libstdcpp +libubus +libubox
endef

define Package/$(PKG_NAME)
	$(call Package/$(PKG_NAME)/Default)
	TITLE += $(PKG_NAME)
	DEPENDS := +libstdcpp +libubus +libubox
endef

define Package/$(PKG_NAME)/description
	$(call Package/$(PKG_NAME)/description/Default)
	mqtt server
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	#$(CP) -r src/ $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(call Build/Compile/Default)
endef

# $(call Build/Compile/Default, TARGET=OPENWRT ENDIAN=$(ENDIANNESS)),)

define Package/$(PKG_NAME)/Default/install
	true
endef

define Package/ubus-server/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin/
	#$(INSTALL_DIR) $(1)/etc/init.d
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
