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
	DEPENDS:=+libnids
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

#define Build/Compile
#	$(MAKE) -C $(PKG_BUILD_DIR)
#	-$(TARGET_CONFIGURE_OPTS) \
#		-CC="$(TARGET_CC)" LD="$(TARGET_CC)" CFLAGS="-c $(TARGET_CFLAGS)"
#		LDFLAGS= \
#		PCAP_H="$(TARGET_CPPFLAGS)" \
#		NIDS_H="$(TARGET_CPPFLAGS) -I$(STAGING_DIR)/usr/lib/libnet-1.0.x/include" \
#		NIDS_L="$(TARGET_LDFLAGS) -L$(STAGING_DIR)/usr/lib/libnet-1.0.x/lib -lnids -lnet -lpcap" libnids
#endef

define Package/ww_engine/install
	$(INSTALL_DIR) $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ww_engine $(1)/bin/ww_engine
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
