include $(TOPDIR)/rules.mk

PKG_NAME:=libevi
PKG_VERSION:=0.9.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/libevi
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=EVI Unified Library (thread-safe, minimal heap, Python/C)
  DEPENDS:=+libpthread +libc
endef

define Package/libevi/description
Unified EVI library (LCM captions + HAL for MT7688) with thread safety and minimal heap.
Installs libevi.so and a Python ctypes wrapper (python2.7) for OpenWrt 15.05.1.
Also provides compatibility symlink libevi_caption.so.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/src $(PKG_BUILD_DIR)/include $(PKG_BUILD_DIR)/python
	$(CP) ./src/* $(PKG_BUILD_DIR)/src/
	$(CP) ./include/* $(PKG_BUILD_DIR)/include/
	$(CP) ./python/evi.py $(PKG_BUILD_DIR)/python/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -Os -fPIC -std=gnu99 -Wall -Wextra -pthread -I$(PKG_BUILD_DIR)/include -c $(PKG_BUILD_DIR)/src/evi_version.c -o $(PKG_BUILD_DIR)/src/evi_version.o
	$(TARGET_CC) $(TARGET_CFLAGS) -Os -fPIC -std=gnu99 -Wall -Wextra -pthread -I$(PKG_BUILD_DIR)/include -c $(PKG_BUILD_DIR)/src/evi_mutex.c -o $(PKG_BUILD_DIR)/src/evi_mutex.o
	$(TARGET_CC) $(TARGET_CFLAGS) -Os -fPIC -std=gnu99 -Wall -Wextra -pthread -I$(PKG_BUILD_DIR)/include -c $(PKG_BUILD_DIR)/src/evi_caption.c -o $(PKG_BUILD_DIR)/src/evi_caption.o
	$(TARGET_CC) $(TARGET_CFLAGS) -Os -fPIC -std=gnu99 -Wall -Wextra -pthread -I$(PKG_BUILD_DIR)/include -c $(PKG_BUILD_DIR)/src/evi_charge.c -o $(PKG_BUILD_DIR)/src/evi_charge.o
	$(TARGET_CC) $(TARGET_CFLAGS) -Os -fPIC -std=gnu99 -Wall -Wextra -pthread -I$(PKG_BUILD_DIR)/include -c $(PKG_BUILD_DIR)/src/evi_charge_router.c -o $(PKG_BUILD_DIR)/src/evi_charge_router.o
	$(TARGET_CC) $(TARGET_CFLAGS) -Os -fPIC -std=gnu99 -Wall -Wextra -pthread -I$(PKG_BUILD_DIR)/include -c $(PKG_BUILD_DIR)/src/evi_hal_mt7688_gpio.c -o $(PKG_BUILD_DIR)/src/evi_hal_mt7688_gpio.o
	$(TARGET_CC) $(TARGET_CFLAGS) -Os -fPIC -std=gnu99 -Wall -Wextra -pthread -c $(PKG_BUILD_DIR)/src/gpio_sysfs.c -o $(PKG_BUILD_DIR)/src/gpio_sysfs.o
	$(TARGET_CC) $(TARGET_LDFLAGS) -shared -pthread -o $(PKG_BUILD_DIR)/src/libevi.so \
		$(PKG_BUILD_DIR)/src/evi_version.o $(PKG_BUILD_DIR)/src/evi_mutex.o \
		$(PKG_BUILD_DIR)/src/evi_caption.o $(PKG_BUILD_DIR)/src/evi_charge.o $(PKG_BUILD_DIR)/src/evi_charge_router.o \
		$(PKG_BUILD_DIR)/src/evi_hal_mt7688_gpio.o $(PKG_BUILD_DIR)/src/gpio_sysfs.o
	ln -sf libevi.so $(PKG_BUILD_DIR)/src/libevi_caption.so
	$(TARGET_CC) $(TARGET_CFLAGS) -Os -std=gnu99 -Wall -Wextra -o $(PKG_BUILD_DIR)/src/initlcm $(PKG_BUILD_DIR)/src/initlcm.c $(PKG_BUILD_DIR)/src/gpio_sysfs.o
endef

define Package/libevi/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/libevi.so $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/libevi_caption.so $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/python2.7/site-packages
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/python/evi.py $(1)/usr/lib/python2.7/site-packages/
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/initlcm $(1)/usr/bin/
endef

$(eval $(call BuildPackage,libevi))
