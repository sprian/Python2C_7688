include $(TOPDIR)/rules.mk
PKG_NAME:=evi-lcdclient
PKG_VERSION:=0.1.0
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/evi-lcdclient
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=EVI LCD client library (for lcdsvc) + Python wrapper
  DEPENDS:=
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/src
	$(CP) ./src/* $(PKG_BUILD_DIR)/src/
endef

TARGET_CFLAGS += -Os -pipe -fdata-sections -ffunction-sections -std=gnu99 -Wall -Wextra -fPIC
TARGET_LDFLAGS += -Wl,--gc-sections -shared

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src \		CC="$(TARGET_CC)" \		CFLAGS="$(TARGET_CFLAGS)" \		LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/evi-lcdclient/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/libevi_lcd.so $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DATA) ./src/evi_lcd.h $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib/python2.7/site-packages
	$(INSTALL_DATA) ./python/evi_lcd.py $(1)/usr/lib/python2.7/site-packages/
	$(INSTALL_DIR) $(1)/usr/lib/python3.8/site-packages
	$(INSTALL_DATA) ./python/evi_lcd.py $(1)/usr/lib/python3.8/site-packages/
endef

$(eval $(call BuildPackage,evi-lcdclient))