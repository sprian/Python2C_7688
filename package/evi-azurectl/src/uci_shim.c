#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int uci_get(const char* key, char* out, size_t outlen){
    char cmd[256]; snprintf(cmd, sizeof(cmd), "uci -q get %s 2>/dev/null", key);
    FILE* f = popen(cmd, "r"); if(!f) return -1;
    if(!fgets(out, (int)outlen, f)){ pclose(f); return -1; }
    size_t n = strlen(out); while(n>0 && (out[n-1]=='\n' || out[n-1]=='\r')) out[--n]=0;
    pclose(f); return 0;
}
int uci_set(const char* key, const char* val){
    char cmd[512]; snprintf(cmd, sizeof(cmd), "uci set %s=%s", key, val);
    return system(cmd);
}
int uci_commit(const char* pkg){
    char cmd[256]; snprintf(cmd, sizeof(cmd), "uci commit %s", pkg);
    return system(cmd);
}
