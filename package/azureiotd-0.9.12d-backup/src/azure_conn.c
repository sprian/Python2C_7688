#ifndef AZUREIOTD_VERSION
#define AZUREIOTD_VERSION "0.9.12c"
#endif
#include "azure_conn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <openssl/pem.h>

static int diag_enabled(void){ const char* s=getenv("AZUREIOTD_DIAG"); return (s&&*s=='1'); }
static void dlog(const char* t){ if(diag_enabled()){ fprintf(stderr,"[azure_conn][diag] %s\n",t); fflush(stderr);} }

static char hex_digit(int v){ return "0123456789ABCDEF"[v & 0xF]; }
static char* url_encode(const char* s){
    size_t n=0; const unsigned char* p;
    for(p=(const unsigned char*)s; *p; ++p){
        if(('A'<=*p && *p<='Z')||('a'<=*p && *p<='z')||('0'<=*p && *p<='9')||*p=='-'||*p=='_'||*p=='.'||*p=='~') n+=1;
        else n+=3;
    }
    char* out=(char*)malloc(n+1); if(!out) return NULL;
    char* w=out;
    for(p=(const unsigned char*)s; *p; ++p){
        if(('A'<=*p && *p<='Z')||('a'<=*p && *p<='z')||('0'<=*p && *p<='9')||*p=='-'||*p=='_'||*p=='.'||*p=='~') *w++=*p;
        else { *w++='%'; *w++=hex_digit(*p>>4); *w++=hex_digit(*p&0xF); }
    }
    *w=0; return out;
}

static unsigned char* b64_decode(const char* in, size_t* outlen){
    BIO *b64=BIO_new(BIO_f_base64()), *bio=BIO_new_mem_buf(in,-1);
    if(!b64||!bio){ if(b64)BIO_free(b64); if(bio)BIO_free(bio); return NULL; }
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_push(b64, bio);
    size_t buflen = strlen(in)*3/4+4;
    unsigned char* buf=(unsigned char*)malloc(buflen);
    if(!buf){ BIO_free_all(bio); return NULL; }
    int n = BIO_read(bio, buf, (int)buflen);
    BIO_free_all(bio);
    if(n<=0){ free(buf); return NULL; }
    *outlen=(size_t)n; return buf;
}

static char* b64_encode(const unsigned char* data, size_t len){
    BIO *b64=BIO_new(BIO_f_base64()), *bio=BIO_new(BIO_s_mem());
    if(!b64||!bio){ if(b64)BIO_free(b64); if(bio)BIO_free(bio); return NULL; }
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    b64 = BIO_push(b64, bio);
    BIO_write(b64, data, (int)len);
    (void)BIO_flush(b64);
    BUF_MEM* mem; char* out=NULL;
    BIO_get_mem_ptr(b64, &mem);
    out=(char*)malloc(mem->length+1);
    if(out){ memcpy(out, mem->data, mem->length); out[mem->length]=0; }
    BIO_free_all(b64);
    return out;
}

char* azure_sas_build_token(const char* host,const char* device_id,const char* b64_key,int ttl_secs){
    if(!host||!*host||!device_id||!*device_id||!b64_key||!*b64_key) return NULL;
    time_t now=time(NULL);
    long se=(long)(now + (ttl_secs>0?ttl_secs:3600));
    char res_uri_raw[512]; snprintf(res_uri_raw,sizeof(res_uri_raw),"%s/devices/%s",host,device_id);
    char* sr=url_encode(res_uri_raw); if(!sr) return NULL;
    char string_to_sign[1024]; snprintf(string_to_sign,sizeof(string_to_sign),"%s\n%ld",sr,se);
    size_t keylen=0; unsigned char* key=b64_decode(b64_key,&keylen); if(!key){ free(sr); return NULL; }
    unsigned char digest[EVP_MAX_MD_SIZE]; unsigned int dlen=0;
    HMAC(EVP_sha256(), key, (int)keylen, (const unsigned char*)string_to_sign, strlen(string_to_sign), digest, &dlen);
    free(key);
    char* b64sig=b64_encode(digest,dlen); if(!b64sig){ free(sr); return NULL; }
    char* sig=url_encode(b64sig); free(b64sig); if(!sig){ free(sr); return NULL; }
    char* token=(char*)malloc(1024);
    if(token){ snprintf(token,1024,"SharedAccessSignature sr=%s&sig=%s&se=%ld",sr,sig,se); }
    free(sr); free(sig); return token;
}

/* MQTT helpers */
static unsigned char* mqtt_write_str(unsigned char* w,const char* s){
    size_t len=strlen(s);
    *w++=(unsigned char)((len>>8)&0xFF);
    *w++=(unsigned char)(len&0xFF);
    memcpy(w,s,len);
    return w+len;
}

static int tcp_connect(const char* host,const char* port){
    struct addrinfo hints; struct addrinfo* res=NULL; struct addrinfo* rp;
    int s=-1; int ret;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC; hints.ai_socktype=SOCK_STREAM; hints.ai_protocol=IPPROTO_TCP;
    dlog("getaddrinfo");
    ret=getaddrinfo(host,port,&hints,&res);
    if(ret!=0) return -1;
    for(rp=res; rp; rp=rp->ai_next){
        s=socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
        if(s<0) continue;
        if(connect(s,rp->ai_addr,rp->ai_addrlen)==0) break;
        close(s); s=-1;
    }
    freeaddrinfo(res);
    return s;
}

/* OpenSSL 1.0.x / 1.1.x compatibility */
static SSL_CTX* create_ssl_ctx(void){
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
    const SSL_METHOD* method = TLS_client_method();
    return SSL_CTX_new(method);
#else
    const SSL_METHOD* method;
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
    method = SSLv23_client_method();
    return SSL_CTX_new(method);
#endif
}

static SSL* tls_wrap_socket(int sock,const char* sni,const char* ca_file,const char* client_cert_path,const char* client_key_path){
    SSL_CTX* ctx=NULL; SSL* ssl=NULL;
    dlog("create_ssl_ctx");
    ctx=create_ssl_ctx(); if(!ctx) return NULL;
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    if(ca_file && *ca_file){
        dlog("load_verify_locations");
        if(SSL_CTX_load_verify_locations(ctx, ca_file, NULL) != 1){
            SSL_CTX_set_default_verify_paths(ctx);
        }
    }else{
        SSL_CTX_set_default_verify_paths(ctx);
    }
    if(client_cert_path && *client_cert_path){
        dlog("use_certificate_file");
        if(SSL_CTX_use_certificate_file(ctx, client_cert_path, SSL_FILETYPE_PEM) != 1){
            SSL_CTX_free(ctx); return NULL;
        }
    }
    if(client_key_path && *client_key_path){
        dlog("use_privatekey_file");
        if(SSL_CTX_use_PrivateKey_file(ctx, client_key_path, SSL_FILETYPE_PEM) != 1){
            SSL_CTX_free(ctx); return NULL;
        }
    }
    dlog("SSL_new");
    ssl=SSL_new(ctx); if(!ssl){ SSL_CTX_free(ctx); return NULL; }
#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
    if(sni && *sni){ SSL_set_tlsext_host_name(ssl, sni); }
#endif
    dlog("SSL_set_fd"); SSL_set_fd(ssl, sock);
    dlog("SSL_connect"); if(SSL_connect(ssl)!=1){ SSL_free(ssl); SSL_CTX_free(ctx); return NULL; }
    return ssl;
}

static int mqtt_read_exact(SSL* ssl,unsigned char* buf,int want){
    int got=0,n;
    while(got<want){
        n=SSL_read(ssl,buf+got,want-got);
        if(n<=0) return -1;
        got+=n;
    }
    return got;
}

static int mqtt_connect_x509(SSL* ssl,const evi_azure_cfg_t* cfg){
    unsigned char pkt[1024]; unsigned char* w=pkt; int rc; char username[512];
    *w++=0x10; w++; w=mqtt_write_str(w,"MQTT"); *w++=4; *w++=0xC2;
    *w++=(unsigned char)((cfg->keepalive>>8)&0xFF); *w++=(unsigned char)(cfg->keepalive&0xFF);
    w=mqtt_write_str(w,cfg->device_id);
    snprintf(username,sizeof(username),"%s/%s/?api-version=2021-04-12",cfg->host,cfg->device_id);
    w=mqtt_write_str(w,username);
    pkt[1]=(unsigned char)(w-(pkt+2));
    dlog("SSL_write CONNECT"); rc=SSL_write(ssl,pkt,(int)(w-pkt)); if(rc<=0) return -1;
    unsigned char hdr[4]; dlog("SSL_read CONNACK");
    if(mqtt_read_exact(ssl,hdr,4)<0) return -2;
    if((hdr[0]&0xF0)!=0x20) return -3;
    if(hdr[1]!=0x02) return -4;
    if(hdr[3]!=0x00) return -5;
    return 0;
}

static int mqtt_publish_qos1(SSL* ssl,const char* topic,const unsigned char* payload,size_t payload_len,unsigned short packet_id){
    unsigned char hdr=0x32; unsigned char vb[1024]; unsigned char* w=vb; int rc; unsigned char rem[4]; int rem_len=0; size_t val;
    w=mqtt_write_str(w,topic);
    *w++=(unsigned char)((packet_id>>8)&0xFF); *w++=(unsigned char)(packet_id&0xFF);
    memcpy(w,payload,payload_len); w+=payload_len;
    val=(size_t)(w-vb);
    do{ unsigned char b=(unsigned char)(val%128); val/=128; if(val>0) b|=0x80; rem[rem_len++]=b; }while(val>0);
    dlog("SSL_write PUBLISH hdr"); rc=SSL_write(ssl,&hdr,1); if(rc<=0) return -1;
    dlog("SSL_write PUBLISH rem"); rc=SSL_write(ssl,rem,rem_len); if(rc<=0) return -2;
    dlog("SSL_write PUBLISH body"); rc=SSL_write(ssl,vb,(int)(w-vb)); if(rc<=0) return -3;
    unsigned char a[4]; dlog("SSL_read PUBACK");
    if(mqtt_read_exact(ssl,a,2)<0) return -4;
    if((a[0]&0xF0)!=0x40) return -5;
    return 0;
}

static int mqtt_disconnect(SSL* ssl){
    unsigned char pkt[2]; pkt[0]=0xE0; pkt[1]=0x00;
    dlog("SSL_write DISCONNECT"); int rc=SSL_write(ssl,pkt,2);
    return (rc==2)?0:-1;
}

int azure_conn_init(evi_azure_runtime_t* rt,const evi_azure_cfg_t* cfg){ (void)cfg; if(!rt) return -1; memset(rt,0,sizeof(*rt)); return 0; }

int azure_conn_run(evi_azure_runtime_t* rt,const evi_azure_cfg_t* cfg){
    (void)rt; if(!cfg) return -1;
    const char* sni=(cfg->sni_host[0]?cfg->sni_host:cfg->host);
    int sock=-1; SSL* ssl=NULL; int rc; char topic[256]; char json[256];
    printf("[azure_conn] host=%s device_id=%s auth=%s keepalive=%d\n",
        cfg->host,cfg->device_id,(cfg->auth==EVI_AUTH_X509?"x509":"sas"),cfg->keepalive);
    printf("[azure_conn] ca_file=%s sni_host=%s\n", cfg->ca_file, sni);

    if(cfg->auth==EVI_AUTH_SAS){
        char* token=azure_sas_build_token(cfg->host,cfg->device_id,cfg->sas_key,cfg->sas_ttl>0?cfg->sas_ttl:3600);
        if(!token){ fprintf(stderr,"[azure_conn] SAS token build failed.\n"); return -2; }
        printf("[azure_conn] (preview) username: %s/%s/?api-version=2021-04-12\n", cfg->host,cfg->device_id);
        printf("[azure_conn] (preview) password: %s\n", token);
        free(token);
        fprintf(stderr,"[azure_conn] SAS 實連線將在後續版本提供。\n");
        return 0;
    }

    dlog("tcp_connect"); sock=tcp_connect(cfg->host,"8883");
    if(sock<0){ fprintf(stderr,"[azure_conn] TCP 連線失敗。\n"); goto cleanup; }
    dlog("tls_wrap_socket"); ssl=tls_wrap_socket(sock,sni,cfg->ca_file,cfg->x509_cert,cfg->x509_key);
    if(!ssl){ fprintf(stderr,"[azure_conn] TLS 連線/驗證失敗。\n"); goto cleanup; }
    dlog("mqtt_connect_x509"); rc=mqtt_connect_x509(ssl,cfg);
    if(rc!=0){ fprintf(stderr,"[azure_conn] MQTT CONNECT 失敗 rc=%d\n",rc); goto cleanup; }

    snprintf(topic,sizeof(topic),"devices/%s/messages/events/",cfg->device_id);
    snprintf(json,sizeof(json),"{\"ts\":%ld,\"msg\":\"hello from azureiotd "" AZUREIOTD_VERSION ""\"}", (long)time(NULL));
    dlog("mqtt_publish_qos1"); rc=mqtt_publish_qos1(ssl,topic,(const unsigned char*)json,strlen(json),1);
    if(rc!=0){ fprintf(stderr,"[azure_conn] PUBLISH 失敗 rc=%d\n",rc); goto cleanup; }
    printf("[azure_conn] 發佈成功：topic=%s payload=%s\n", topic, json);

    dlog("mqtt_disconnect"); mqtt_disconnect(ssl);
    dlog("SSL_shutdown"); SSL_shutdown(ssl);
    shutdown(sock,2); close(sock); return 0;
cleanup:
    if(ssl){ SSL_shutdown(ssl); }
    if(sock>=0){ shutdown(sock,2); close(sock); }
    return -1;
}
