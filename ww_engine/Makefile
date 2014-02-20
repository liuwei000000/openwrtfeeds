# 
# Copyright (C) 2014 hiwifitw
#
# create by liuweiky@qq.com
#
include $(TOPDIR)/rules.mk

PKG_NAME:=ww_engine
PKG_VERSION:=0.1
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ww_engine
	SECTION:=Detection Engine
	CATEGORY:=Network
	DEPENDS:=+libnids +libstdcpp
	TITLE:=ww_engine
	MAINTAINER:=liuwei
endef

define Package/ww_engine/description
	7 leyer dpi
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/ww_engine/install
	$(INSTALL_DIR) $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ww_engine $(1)/bin/ww_engine
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
