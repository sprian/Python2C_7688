include $(TOPDIR)/rules.mk
PKG_NAME:=evi-python2c
PKG_VERSION:=0.9.1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
include $(INCLUDE_DIR)/package.mk
define Package/evi-python2c
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=EVI Python→C harness (event loop & tests)
  DEPENDS:=+libpthread +libevi
endef
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/src
	$(CP) ./src/* $(PKG_BUILD_DIR)/src/
endef
TARGET_CFLAGS += -Os -pipe -fdata-sections -ffunction-sections -std=gnu99 -Wall -Wextra
TARGET_LDFLAGS += -Wl,--gc-sections
define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)"
endef
define Package/evi-python2c/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/evi-python2c $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/evi-python2c.init $(1)/etc/init.d/evi-python2c
endef
$(eval $(call BuildPackage,evi-python2c))
