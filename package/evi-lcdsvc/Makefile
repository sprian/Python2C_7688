include $(TOPDIR)/rules.mk
PKG_NAME:=evi-lcdsvc
PKG_VERSION:=0.1.0
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/evi-lcdsvc
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=EVI LCD service (daemon + CLI, with arbitration)
  DEPENDS:=
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/src
	$(CP) ./src/* $(PKG_BUILD_DIR)/src/
endef

TARGET_CFLAGS += -Os -pipe -fdata-sections -ffunction-sections -std=gnu99 -Wall -Wextra
TARGET_LDFLAGS += -Wl,--gc-sections

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src \		CC="$(TARGET_CC)" \		CFLAGS="$(TARGET_CFLAGS)" \		LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/evi-lcdsvc/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/lcdctl $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/lcdsvc $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/evi-lcdsvc.init $(1)/etc/init.d/evi-lcdsvc
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/lcd $(1)/etc/config/lcd
endef

$(eval $(call BuildPackage,evi-lcdsvc))
