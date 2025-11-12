include $(TOPDIR)/rules.mk

PKG_NAME:=libevi-azure
PKG_RELEASE:=2
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/libevi-azure
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Thin client library for azureiotd Unix socket
  DEPENDS:=+libpthread
endef

define Package/libevi-azure/description
 Client-side C library that talks to azureiotd via /var/run/azureiotd.sock and exposes simple APIs.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -fPIC -shared -o $(PKG_BUILD_DIR)/libevi_azure.so $(PKG_BUILD_DIR)/libevi_azure.c $(TARGET_LDFLAGS)
endef

define Package/libevi-azure/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libevi_azure.so $(1)/usr/lib/

	$(INSTALL_DIR) $(1)/usr/lib/python
	$(INSTALL_DATA) ./python/evi_azure.py $(1)/usr/lib/python/
endef

$(eval $(call BuildPackage,libevi-azure))
