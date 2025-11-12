include $(TOPDIR)/rules.mk
PKG_NAME:=evi-azurectl
PKG_VERSION:=0.9.7
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/evi-azurectl
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=EVI Azure control — C port of azureIoT.py
  DEPENDS:=+libpthread
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

define Package/evi-azurectl/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/evi-azurectl $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/evi-azurectl.init $(1)/etc/init.d/evi-azurectl
endef

$(eval $(call BuildPackage,evi-azurectl))
