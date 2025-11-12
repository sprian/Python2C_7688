#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int read_file_line(const char* path, char* out, size_t outlen){
    FILE* f = fopen(path, "r");
    if(!f) return -1;
    if(!fgets(out, (int)outlen, f)){ fclose(f); return -1; }
    size_t n = strlen(out);
    while(n>0 && (out[n-1]=='\n' || out[n-1]=='\r')) out[--n]=0;
    fclose(f); return 0;
}
int write_file(const char* path, const char* s){
    FILE* f = fopen(path, "w");
    if(!f) return -1;
    int rc = fputs(s, f) < 0 ? -1 : 0;
    fclose(f); return rc;
}
