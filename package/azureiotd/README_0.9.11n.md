# azureiotd 0.9.11n (OpenWrt 15.05.1 / MT7688)

- Conditional IoTHub/DPS linking based on libs in `CONFIG_AZUREIOTD_EXT_AZURE_LIBDIR`.
- UMQTT-only fallback when libs are absent: build still succeeds.
- TLS/SNI/Cert compile-time fallbacks via Config.in; runtime logs for field diagnostics.
- Includes procd init and default UCI config.

## Build
```
make menuconfig
# Network -> azureiotd -> Configuration
# set External Azure IoT C SDK lib dir (if you built azure-iot-sdk-c)

make package/azureiotd/clean V=s
make package/azureiotd/compile V=s
```
