#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int uci_get(const char*, char*, size_t);
int uci_set(const char*, const char*);
int uci_commit(const char*);
int usb_set_port(const char* act, int port);

// helpers
static int parse_hex_u8(const char* s){
    if (!s) return -1; int v=0; if (sscanf(s, "%x", &v)!=1) return -1;
    return (v<0 || v>0xFF) ? -1 : v;
}
static int uci_get_int(const char* key){
    char buf[64]={0}; if (uci_get(key, buf, sizeof(buf)) != 0) return -1;
    int v=-1; sscanf(buf, "%d", &v); return v;
}
static void uci_set_int(const char* key, int v){
    char tmp[64]; snprintf(tmp, sizeof(tmp), "%d", v); uci_set(key, tmp);
}
static void format_dongleport_bits(int mask, char* out, size_t outlen){
    int b0 = (mask & (1<<0)) ? 1:0, b1 = (mask & (1<<1)) ? 1:0;
    int b2 = (mask & (1<<2)) ? 1:0, b3 = (mask & (1<<3)) ? 1:0;
    snprintf(out, outlen, "%d,%d,%d,%d", b0,b1,b2,b3);
}

static void print_usage(const char* p){
    fprintf(stderr, "Usage: %s --get <CmdName> | --set <CmdName>,<val>\n", p);
}

int main(int argc, char** argv){
    if (argc < 3){ print_usage(argv[0]); return 1; }

    int is_get = !strcmp(argv[1],"--get");
    int is_set = !strcmp(argv[1],"--set");
    if (!is_get && !is_set){ print_usage(argv[0]); return 1; }

    if (is_get){
        const char* cmd = argv[2];

        if (!strcmp(cmd, "getUsbDongle")){
            // usbdongle,<usbdongle>,<b0,b1,b2,b3>  ← 來自 UCI
            int ud = uci_get_int("system.@system[0].usbdongle");
            int dp = uci_get_int("system.@system[0].dongleport");
            char bits[32]; bits[0]=0;
            if (dp >= 0) format_dongleport_bits(dp, bits, sizeof(bits));

            if (ud < 0){
                printf("usbdongle,0,-1,-1,-1,-1\n");
            } else if (dp >= 0){
                printf("usbdongle,%d,%s\n", ud, bits);
            } else {
                printf("usbdongle,%d,-1,-1,-1,-1\n", ud);
            }
        } else {
            printf("unknown\n");
        }

    } else {
        char* pair = argv[2]; char* comma = strchr(pair, ',');
        if (!comma){ printf("Wrong settings\n"); return 0; }
        *comma = 0; const char* key = pair; const char* valStr = comma+1;

        if (!strncmp(key, "setDev", 6)){
            int port_hex = parse_hex_u8(valStr);
            int port = (port_hex >= 1) ? port_hex : atoi(valStr); // 支援 "01" 或 "1"
            if (port < 1 || port > 4){ printf("Wrong settings\n"); return 0; }

            const char* act = (!strcmp(key+6,"On")) ? "On" : "Off";
            int rc = usb_set_port(act, port);
            if (rc == 0){
                int mask = uci_get_int("system.@system[0].dongleport");
                if (mask < 0) mask = 0;
                int bit = 1 << (port - 1);
                if (!strcmp(act,"On")) mask |= bit; else mask &= ~bit;
                uci_set_int("system.@system[0].dongleport", mask);
                uci_commit("system");
            }
            // 與 python 相同的回傳格式（小寫）
            printf("%s,%s\n", (!strcmp(key+6,"On")) ? "devon" : "devoff", valStr);

        } else if (!strcmp(key, "setPingSettings")){
            uci_set("system.@system[0].pingsettings", valStr);
            uci_commit("system");
            printf("pingsettings,%s\n", valStr);

        } else {
            printf("Wrong settings\n");
        }
    }
    return 0;
}
