#pragma once
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
const char* evi_caption_version(void);
void evi_caption_initial(void);
void evi_caption_system_loading(void);
void evi_caption_info1(const char* mcuFw, const char* socFw);
void evi_caption_info2(const char* sn, const char* prodName);
void evi_caption_info3(const char* meter, const char* maxCurrent);
void evi_caption_info4(const char* meter2, const char* maxCurrent2);
void evi_caption_standby(int delaytime);
void evi_caption_preparing(void);
void evi_caption_authWaiting(int sec);
void evi_caption_plugInWaiting(int sec);
void evi_caption_serverConn(void);
void evi_caption_serverConnFailed(void);
void evi_caption_serverConnLost(void);
void evi_caption_getMValue(const char* item,const char* value);
void evi_caption_dispComplete(const char* item,const char* value);
#ifdef __cplusplus
}
#endif
