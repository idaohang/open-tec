include $(TOPDIR)/rules.mk

PKG_NAME:=sirfdump
PKG_VERSION:=0.2-22
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/littlesavage-sirfdump-45251e0
PKG_SOURCE:=littlesavage-sirfdump-v0.2-22-g45251e0.zip
PKG_SOURCE_URL:=https://github.com/littlesavage/sirfdump/zipball/45251e0e37f39975526ec5d112508e9e94879ddb
PKG_MD5SUM:=80a74b3dc6bedd53e3462ea465dd6441
PKG_CAT:=unzip

include $(INCLUDE_DIR)/package.mk

define Package/sirfdump
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Sirf binary logger
  URL:=https://github.com/littlesavage/sirfdump
endef

define Package/sirfdump/description
	Sirf binary logger
endef

define Package/sirfdump/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sirfdump $(1)/usr/bin/
endef

$(eval $(call BuildPackage,sirfdump))
