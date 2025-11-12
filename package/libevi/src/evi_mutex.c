#include "evi_mutex.h"
#include <pthread.h>
static pthread_mutex_t g_evi_mtx = PTHREAD_MUTEX_INITIALIZER;
void evi_lock(void){ pthread_mutex_lock(&g_evi_mtx); }
void evi_unlock(void){ pthread_mutex_unlock(&g_evi_mtx); }
