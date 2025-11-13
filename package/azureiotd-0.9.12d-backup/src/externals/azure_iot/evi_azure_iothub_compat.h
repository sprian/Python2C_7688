#pragma once
/* EVI compat for IoTHub *_Ex API differences across SDK versions.
 * In IoTHub SDK builds, ALWAYS macro-map *_Ex -> legacy name.
 * In UMQTT-only builds, provide a harmless stub. */
#if defined(EVI_USE_IOTHUB_SDK)
  #ifndef IoTHubDeviceClient_LL_SetDeviceMethodCallback_Ex
  #define IoTHubDeviceClient_LL_SetDeviceMethodCallback_Ex IoTHubDeviceClient_LL_SetDeviceMethodCallback
  #endif
#else
  static inline int IoTHubDeviceClient_LL_SetDeviceMethodCallback_Ex(void* h, void* cb, void* ctx)
  { (void)h; (void)cb; (void)ctx; return 0; }
#endif
