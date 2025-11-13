include $(TOPDIR)/rules.mk

PKG_NAME:=azureiotd
PKG_VERSION:=0.9.12c
CFLAGS += -DAZUREIOTD_VERSION=\"0.9.12c\"
PKG_RELEASE:=1

PKG_LICENSE:=MIT
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

AZ_SDK_DIR := /home/ai7688/ai7688/py2c/azureProcess/azure-iot-sdk-c
AZ_BUILD_DIR := $(AZ_SDK_DIR)/build-mt7688

include $(INCLUDE_DIR)/package.mk

define Package/azureiotd
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Azure IoT daemon (C) with conditional IoTHub/DPS
  DEPENDS:=+libopenssl +libpthread +librt +libuci +libuuid +libcurl +zlib
endef

define Package/azureiotd/description
 Azure IoT Hub client daemon for MT7688 (OpenWrt 15.05.1) with conditional IoTHub/DPS linking and TLS/SNI toggles.
endef

define Package/azureiotd/config
  source "$(SOURCE)/Config.in"
endef

PKG_CONFIG_DEPENDS := \
  CONFIG_AZUREIOTD_WITH_IOTHUB \
  CONFIG_AZUREIOTD_DPS_REQUIRED \
  CONFIG_AZUREIOTD_EXT_AZURE_LIBDIR \
  CONFIG_AZUREIOTD_TLS_MIN_TLS12 \
  CONFIG_AZUREIOTD_CIPHER_WHITELIST \
  CONFIG_AZUREIOTD_TLS_VER_POLICY_WHITELIST \
  CONFIG_AZUREIOTD_TLS_VER_WHITELIST \
  CONFIG_AZUREIOTD_TLS_VER_POLICY_BLACKLIST \
  CONFIG_AZUREIOTD_TLS_VER_BLACKLIST \
  CONFIG_AZUREIOTD_SNI_ENABLE \
  CONFIG_AZUREIOTD_SNI_HOST \
  CONFIG_AZUREIOTD_CA_FILE \
  CONFIG_AZUREIOTD_CLIENT_CERT \
  CONFIG_AZUREIOTD_CLIENT_KEY \
  CONFIG_AZUREIOTD_CLIENT_KEY_PASS \
  CONFIG_AZUREIOTD_LOG_TLS_RUNTIME

define Build/Prepare
	$(call Build/Prepare/Default)
	mkdir -p $(PKG_BUILD_DIR)/src/externals/azure_iot
	$(CP) ./src/* $(PKG_BUILD_DIR)/src/
endef

######## EVI 0.9.11n conditional detection ########
AZSDK_DIR := $(call qstrip,$(CONFIG_AZUREIOTD_EXT_AZURE_LIBDIR))

# Detect IoTHub & DPS libs under external build dir
HAVE_IOTHUB=0
HAVE_DPS=0
ifneq ($(strip $(AZSDK_DIR)),)
  IOTHUB_LIB_A := $(firstword $(wildcard $(AZSDK_DIR)/iothub_client/libiothub_client.a $(AZSDK_DIR)/iothub_client/libiothub_client.so))
  IOTHUB_MQTT_A := $(firstword $(wildcard $(AZSDK_DIR)/iothub_client/libiothub_client_mqtt_transport.a $(AZSDK_DIR)/iothub_client/libiothub_client_mqtt_transport.so))
  ifneq ($(strip $(IOTHUB_LIB_A)$(IOTHUB_MQTT_A)),)
    HAVE_IOTHUB=1
  endif
  ifeq ($(CONFIG_AZUREIOTD_DPS_REQUIRED),y)
    PROV_AUTH_A := $(firstword $(wildcard $(AZSDK_DIR)/provisioning_client/libprov_auth_client.a $(AZSDK_DIR)/provisioning_client/libprov_auth_client.so))
    HSM_SEC_A   := $(firstword $(wildcard $(AZSDK_DIR)/provisioning_client/libhsm_security_client.a $(AZSDK_DIR)/provisioning_client/libhsm_security_client.so))
    ifneq ($(strip $(PROV_AUTH_A)$(HSM_SEC_A)),)
      HAVE_DPS=1
    endif
  endif
  # Add -L paths for external build
  TARGET_LDFLAGS += $(foreach d,iothub_client c-utility umqtt uhttp provisioning_client deps/parson,-L$(AZSDK_DIR)/$(d))
endif

# Detect parson/uhttp variants (prefer external dir, then staging)
PARSON_LIB := parson
ifneq ($(wildcard $(AZSDK_DIR)/deps/parson/libparson-c.*),)
  PARSON_LIB := parson-c
endif
ifneq ($(wildcard $(STAGING_DIR)/usr/lib/libparson-c.*),)
  PARSON_LIB := parson-c
endif

UHTTP_LIB := uhttp
ifneq ($(wildcard $(AZSDK_DIR)/uhttp/libuhttpc.*),)
  UHTTP_LIB := uhttpc
endif
ifneq ($(wildcard $(STAGING_DIR)/usr/lib/libuhttpc.*),)
  UHTTP_LIB := uhttpc
endif

TARGET_CPPFLAGS += -I$(PKG_BUILD_DIR)/src -I$(PKG_BUILD_DIR)/src/externals/azure_iot

# Compile-time TLS/SNI/cert fallbacks from menuconfig
ifeq ($(CONFIG_AZUREIOTD_TLS_MIN_TLS12),y)
  TARGET_CFLAGS += -DEVI_TLS_MIN_VER_TLS12=1
endif
ifneq ($(call qstrip,$(CONFIG_AZUREIOTD_CIPHER_WHITELIST)),"")
  TARGET_CFLAGS += -DEVI_TLS_CIPHER_WHITELIST=\"$(call qstrip,$(CONFIG_AZUREIOTD_CIPHER_WHITELIST))\"
endif
ifeq ($(CONFIG_AZUREIOTD_TLS_VER_POLICY_WHITELIST),y)
  TARGET_CFLAGS += -DEVI_TLS_VERSION_POLICY_WHITELIST=1 -DEVI_TLS_VERSION_LIST=\"$(call qstrip,$(CONFIG_AZUREIOTD_TLS_VER_WHITELIST))\"
endif
ifeq ($(CONFIG_AZUREIOTD_TLS_VER_POLICY_BLACKLIST),y)
  TARGET_CFLAGS += -DEVI_TLS_VERSION_POLICY_BLACKLIST=1 -DEVI_TLS_VERSION_LIST=\"$(call qstrip,$(CONFIG_AZUREIOTD_TLS_VER_BLACKLIST))\"
endif
ifeq ($(CONFIG_AZUREIOTD_SNI_ENABLE),y)
  TARGET_CFLAGS += -DEVI_SNI_ENABLE=1
endif
ifneq ($(call qstrip,$(CONFIG_AZUREIOTD_SNI_HOST)),"")
  TARGET_CFLAGS += -DEVI_SNI_HOST=\"$(call qstrip,$(CONFIG_AZUREIOTD_SNI_HOST))\"
endif
ifneq ($(call qstrip,$(CONFIG_AZUREIOTD_CA_FILE)),"")
  TARGET_CFLAGS += -DEVI_CA_FILE=\"$(call qstrip,$(CONFIG_AZUREIOTD_CA_FILE))\"
endif
ifneq ($(call qstrip,$(CONFIG_AZUREIOTD_CLIENT_CERT)),"")
  TARGET_CFLAGS += -DEVI_CLIENT_CERT=\"$(call qstrip,$(CONFIG_AZUREIOTD_CLIENT_CERT))\"
endif
ifneq ($(call qstrip,$(CONFIG_AZUREIOTD_CLIENT_KEY)),"")
  TARGET_CFLAGS += -DEVI_CLIENT_KEY=\"$(call qstrip,$(CONFIG_AZUREIOTD_CLIENT_KEY))\"
endif
ifneq ($(call qstrip,$(CONFIG_AZUREIOTD_CLIENT_KEY_PASS)),"")
  TARGET_CFLAGS += -DEVI_CLIENT_KEY_PASS=\"$(call qstrip,$(CONFIG_AZUREIOTD_CLIENT_KEY_PASS))\"
endif
ifeq ($(CONFIG_AZUREIOTD_LOG_TLS_RUNTIME),y)
  TARGET_CFLAGS += -DEVI_LOG_TLS_RUNTIME=1
endif

LIB_DIRS := \
    -L$(AZ_BUILD_DIR)/iothub_client \
    -L$(AZ_BUILD_DIR)/provisioning_client \
    -L$(AZ_BUILD_DIR)/deps/parson \
    -L$(AZ_BUILD_DIR)/umqtt \
    -L$(AZ_BUILD_DIR)/deps/uhttp \
    -L$(AZ_BUILD_DIR)/c-utility \
    -L$(SDK_DIR)/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/lib

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(LIB_DIRS) \
		-o $(PKG_BUILD_DIR)/azureiotd \
		$(PKG_BUILD_DIR)/src/azureiotd.c \
		$(PKG_BUILD_DIR)/src/azure_conn.c \
		$(if $(filter 1,$(HAVE_IOTHUB)),-DEVI_USE_IOTHUB_SDK=1,) \
		-Wl,--start-group \
		$(if $(filter 1,$(HAVE_IOTHUB)),-liothub_client -liothub_client_mqtt_transport,) \
		$(if $(filter 1,$(HAVE_DPS)),-lprov_auth_client -lhsm_security_client,) \
		-l$(UHTTP_LIB) -l$(PARSON_LIB) -lumqtt -laziotsharedutil -lssl -lcrypto \
		-luuid -lm -lcurl -lz -lpthread -lrt -luci \
		-Wl,--end-group
endef

define Package/azureiotd/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/azureiotd $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/azureiotd.init $(1)/etc/init.d/azureiotd
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/azureiotd.config $(1)/etc/config/azureiotd
endef

$(eval $(call BuildPackage,azureiotd))

DEPENDS += +libuci +libopenssl