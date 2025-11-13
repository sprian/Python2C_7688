#include "safe_str.h"
#include <string.h>

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

size_t strlcpy_compat(char *dst, const char *src, size_t dstsz){
    if(!dst || dstsz==0) return 0;
    if(!src){ dst[0]='\0'; return 0; }
    size_t n = strlen(src);
    size_t c = (dstsz>0)? MIN(n, dstsz-1) : 0;
    if(c) memcpy(dst, src, c);
    dst[c] = '\0';
    return n;
}

static void chop_at_first(char *s, const char *cutset){
    if(!s || !cutset) return;
    char *p = strpbrk(s, cutset);
    if(p) *p = '\0';
}

void safe_hostname_only(const char *src, char *dst, size_t dstsz){
    if(!dst || dstsz==0){ return; }
    dst[0] = '\0';
    if(!src){ return; }

    char tmp[512];
    strlcpy_compat(tmp, src, sizeof(tmp));
    const char *s = tmp;

    // drop scheme
    const char *scheme = strstr(s, "://");
    if(scheme) s = scheme + 3;

    // copy to dst
    strlcpy_compat(dst, s, dstsz);

    // remove path / query / fragment
    chop_at_first(dst, "/?#");

    // remove :port
    char *colon = strchr(dst, ':');
    if(colon) *colon = '\0';
}

void safe_remove_file_ext(const char *src, char *dst, size_t dstsz){
    if(!dst || dstsz==0){ return; }
    dst[0] = '\0';
    if(!src){ return; }
    char tmp[512];
    strlcpy_compat(tmp, src, sizeof(tmp));

    // Keep only the basename portion
    const char *base = strrchr(tmp, '/');
    base = base ? base+1 : tmp;

    // copy and strip extension
    strlcpy_compat(dst, base, dstsz);
    char *dot = strrchr(dst, '.');
    if(dot) *dot = '\0';
}
