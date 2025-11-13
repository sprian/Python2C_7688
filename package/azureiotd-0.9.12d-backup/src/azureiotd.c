#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "safe_str.h"
#include <sys/stat.h>
#include <uci.h>
#include "azure_conn.h"

static int diag_enabled(void){ const char* s=getenv("AZUREIOTD_DIAG"); return (s&&*s=='1'); }
static void diag(const char* t){ if(diag_enabled()){ fprintf(stderr,"[azureiotd][diag] %s\n",t); fflush(stderr);} }
static void diag_path(const char* title,const char* path){
    if(!diag_enabled()) return;
    struct stat st; int ok=(path&&*path&&access(path,R_OK)==0&&stat(path,&st)==0);
    fprintf(stderr,"[azureiotd][diag] %s: %s %s\n", title, path?path:"(null)", ok?"[OK]":"[MISSING]"); fflush(stderr);
}

static int uci_get_str(struct uci_context* ctx, const char* opt, char* out, size_t outlen){
    struct uci_ptr p; memset(&p,0,sizeof(p));
    if(uci_lookup_ptr(ctx,&p,(char*)opt,1)!=UCI_OK) return -1;
    if(!(p.flags&UCI_LOOKUP_COMPLETE)||!p.o||p.o->type!=UCI_TYPE_STRING) return -2;
    snprintf(out,outlen,"%s",p.o->v.string); return 0;
}
static int uci_get_int(struct uci_context* ctx, const char* opt, int defv){
    struct uci_ptr p; memset(&p,0,sizeof(p));
    if(uci_lookup_ptr(ctx,&p,(char*)opt,1)!=UCI_OK) return defv;
    if(!(p.flags&UCI_LOOKUP_COMPLETE)||!p.o) return defv;
    if(p.o->type==UCI_TYPE_STRING && p.o->v.string && *p.o->v.string) return atoi(p.o->v.string);
    return defv;
}

int main(int argc,char**argv){
    (void)argc; (void)argv;
    diag("start");
    struct uci_context* ctx=uci_alloc_context();
    evi_azure_cfg_t cfg; memset(&cfg,0,sizeof(cfg));
    evi_azure_runtime_t rt;

    diag("uci_load");
    if(ctx){
        uci_get_str(ctx,"azureiotd.main.host",cfg.host,sizeof(cfg.host));
        uci_get_str(ctx,"azureiotd.main.device_id",cfg.device_id,sizeof(cfg.device_id));
        { char auth[16]="sas"; uci_get_str(ctx,"azureiotd.main.auth",auth,sizeof(auth));
          cfg.auth = (!strcmp(auth,"x509")?EVI_AUTH_X509:EVI_AUTH_SAS); }
        uci_get_str(ctx,"azureiotd.main.sas_key",cfg.sas_key,sizeof(cfg.sas_key));
        cfg.sas_ttl = uci_get_int(ctx,"azureiotd.main.sas_ttl",3600);
        uci_get_str(ctx,"azureiotd.main.x509_cert",cfg.x509_cert,sizeof(cfg.x509_cert));
        uci_get_str(ctx,"azureiotd.main.x509_key", cfg.x509_key, sizeof(cfg.x509_key));
        uci_get_str(ctx,"azureiotd.main.x509_key_pass", cfg.x509_key_pass, sizeof(cfg.x509_key_pass));
        uci_get_str(ctx,"azureiotd.main.sni_host",cfg.sni_host,sizeof(cfg.sni_host));
        // Normalize host and SNI host safely (strip scheme, path and :port if present)
        if (cfg.host[0]) {
            char host_only[256];
            safe_hostname_only(cfg.host, host_only, sizeof(host_only));
            if (host_only[0]) {
                // overwrite cfg.host with the normalized hostname
                strlcpy_compat(cfg.host, host_only, sizeof(cfg.host));
            }
        }
        if (cfg.sni_host[0]) {
            char sni_only[256];
            safe_hostname_only(cfg.sni_host, sni_only, sizeof(sni_only));
            if (sni_only[0]) {
                strlcpy_compat(cfg.sni_host, sni_only, sizeof(cfg.sni_host));
            }
        }

        uci_get_str(ctx,"azureiotd.main.ca_file", cfg.ca_file, sizeof(cfg.ca_file));
        cfg.keepalive = uci_get_int(ctx,"azureiotd.main.keepalive",60);
        uci_free_context(ctx);
    }
    diag("uci_done");

    if(!cfg.host[0]||!cfg.device_id[0]){
        fprintf(stderr,"[azureiotd] 缺少 host 或 device_id，請編輯 /etc/config/azureiotd\n"); return 2;
    }
    if(cfg.auth==EVI_AUTH_X509){
        diag_path("x509_cert",cfg.x509_cert);
        diag_path("x509_key", cfg.x509_key);
        diag_path("ca_file",  cfg.ca_file);
    }

    diag("conn_init");
    if(azure_conn_init(&rt,&cfg)!=0){ fprintf(stderr,"[azureiotd] azure_conn_init 失敗\n"); return 3; }
    diag("conn_run");
    return azure_conn_run(&rt,&cfg);
}
