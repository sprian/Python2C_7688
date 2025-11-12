#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int get_mac(const char *ifname, char *out, size_t outlen)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "ifconfig %s | grep HWaddr | awk '{print $5}'", ifname);
    FILE *f = popen(cmd, "r");
    if(!f) return -1;
    if(!fgets(out, (int)outlen, f))
    {
        pclose(f);
        return -1;
    }
    size_t n = strlen(out);
    while(n > 0 && (out[n - 1] == '\n' || out[n - 1] == '\r')) out[--n] = 0;
    pclose(f);
    return 0;
}
int get_wifi_signal(char *out, size_t outlen)
{
    char ssid[128] = {0};
    FILE *f = popen("uci get wireless.sta.ssid", "r");
    if(!f) return -1;
    if(!fgets(ssid, sizeof(ssid), f))
    {
        pclose(f);
        return -1;
    }
    pclose(f);
    size_t n = strlen(ssid);
    while(n > 0 && (ssid[n - 1] == '\n' || ssid[n - 1] == '\r')) ssid[--n] = 0;
    system("iwpriv ra0 set SiteSurvey=1");
    sleep(3);
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "iwpriv ra0 get_site_survey|grep -w %s|awk '{print $5}'", ssid);
    f = popen(cmd, "r");
    if(!f) return -1;
    if(!fgets(out, (int)outlen, f))
    {
        pclose(f);
        return -1;
    }
    n = strlen(out);
    while(n > 0 && (out[n - 1] == '\n' || out[n - 1] == '\r')) out[--n] = 0;
    pclose(f);
    return 0;
}
