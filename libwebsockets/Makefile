# Copyright (C) 2010 Commotion
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
include $(TOPDIR)/rules.mk

PKG_NAME:=libwebsockets
PKG_RELEASE:=1
PKG_VERSION:=1.2.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=git://git.libwebsockets.org/libwebsockets
PKG_SOURCE_VERSION:=f162492cf389d9680ecacfd951921607e0899679
PKG_SOURCE_SUBDIR:=libwebsockets
PKG_SOURCE_PROTO:=git

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/libwebsockets
  SECTION:=commotion-apps
  CATEGORY:=Commotion-apps
  TITLE:=C Websockets Server Library
  DEPENDS:= +zlib +libopenssl
  URL:=http://git.libwebsockets.org/libwebsockets
endef

define Build/Configure
	$(call Build/Configure/Default,--with-linux-headers=$(LINUX_DIR) --datadir=/usr/share)
endef

define Package/libwebsockets/description
  C Websockets Server Library, this package only installs the library and a test application.
endef


define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/lib/libwebsockets.h $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/lib//libwebsockets.{a,so*} $(1)/usr/lib/	
endef

define Package/libwebsockets/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/share/libwebsockets-test-server
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin/libwebsockets-test-server $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lib/libwebsockets.{a,so*} $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/test-server/test.html $(1)/usr/share/libwebsockets-test-server
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/test-server/favicon.ico $(1)/usr/share/libwebsockets-test-server
	
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lib/libwebsockets.h $(1)/usr/include
	


endef

$(eval $(call BuildPackage,libwebsockets))
