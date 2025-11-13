#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dps_certificate.h"
#include <sys/stat.h>

// 獲取序列號
char* dps_get_serial_number(void) {
    static char serial_no[64] = {0};
    FILE *fp = popen("uci -q get system.@system[0].regid", "r");
    
    if (fp) {
        if (fgets(serial_no, sizeof(serial_no), fp)) {
            // 去除換行符
            serial_no[strcspn(serial_no, "\n")] = 0;
        }
        pclose(fp);
    }
    
    // 如果沒有獲取到序列號，返回空字符串
    if (strlen(serial_no) == 0 || strcmp(serial_no, "-1") == 0) {
        return NULL;
    }
    
    return serial_no;
}

// 檢查證書是否存在
int dps_check_certificates_exist(const char *serial_no) {
    if (!serial_no) return 0;
    
    char key_path[256], full_chain_path[256];
    struct stat st;
    
    snprintf(key_path, sizeof(key_path), "/IoT/openssl/private/%s.key.pem", serial_no);
    snprintf(full_chain_path, sizeof(full_chain_path), "/IoT/openssl/certs/%s-full-chain.cert.pem", serial_no);
    
    return (stat(key_path, &st) == 0 && stat(full_chain_path, &st) == 0);
}

// 生成證書
int dps_generate_certificates(dps_cert_manager_t *cert_mgr) {
    if (!cert_mgr) return -1;
    
    char *serial_no = dps_get_serial_number();
    if (!serial_no) {
        printf("錯誤: 無法獲取序列號\n");
        return -1;
    }
    
    strcpy(cert_mgr->serial_no, serial_no);
    
    // 設置文件路徑
    snprintf(cert_mgr->key_path, sizeof(cert_mgr->key_path), 
             "/IoT/openssl/private/%s.key.pem", serial_no);
    snprintf(cert_mgr->cert_path, sizeof(cert_mgr->cert_path), 
             "/IoT/openssl/certs/%s.cert.pem", serial_no);
    snprintf(cert_mgr->full_chain_path, sizeof(cert_mgr->full_chain_path), 
             "/IoT/openssl/certs/%s-full-chain.cert.pem", serial_no);
    
    // 檢查證書是否已存在
    if (dps_check_certificates_exist(serial_no)) {
        printf("證書已存在，跳過生成: %s\n", serial_no);
        system("uci set system.@system[0].AzureKeyChange=0");
        system("uci commit system");
        return 0;
    }
    
    printf("開始生成證書 for %s\n", serial_no);
    
    // 設置系統狀態
    system("uci set system.@system[0].NetLink=0");
    system("uci set system.@system[0].AzureBindingStatus=0");
    system("uci set system.@system[0].AzureKeyChange=1");
    system("uci set system.@system[0].MqttBindingStatus=0");
    system("uci commit system");
    
    // 創建目錄結構
    system("cd /IoT/openssl && mkdir -m 755 -p csr newcerts 2>/dev/null");
    system("cd /IoT/openssl && rm -f index.txt && touch index.txt 2>/dev/null");
    
    char command[1024];
    int result;
    
    // 生成私鑰
    printf("生成私鑰...\n");
    snprintf(command, sizeof(command),
             "openssl genrsa -out /IoT/openssl/private/%s.key.pem 4096 2>/dev/null", serial_no);
    result = system(command);
    
    if (result != 0) {
        printf("生成私鑰失敗\n");
        return -1;
    }
    
    // 生成證書請求
    printf("生成證書請求...\n");
    snprintf(command, sizeof(command),
             "openssl req -config /IoT/openssl/openssl_device_intermediate_ca.cnf "
             "-key /IoT/openssl/private/%s.key.pem -subj \"/CN=%s\" -new -sha256 "
             "-out /IoT/openssl/csr/%s.csr.pem 2>/dev/null", serial_no, serial_no, serial_no);
    result = system(command);
    
    if (result != 0) {
        printf("生成證書請求失敗\n");
        return -1;
    }
    
    // 簽發證書
    printf("簽發證書...\n");
    snprintf(command, sizeof(command),
             "openssl ca -batch -config /IoT/openssl/openssl_device_intermediate_ca.cnf "
             "-passin pass:fev0000$ -extensions usr_cert -days 10950 -notext -md sha256 "
             "-in /IoT/openssl/csr/%s.csr.pem -out /IoT/openssl/certs/%s.cert.pem 2>/dev/null", 
             serial_no, serial_no);
    result = system(command);
    
    if (result != 0) {
        printf("簽發證書失敗\n");
        return -1;
    }
    
    // 驗證證書並創建完整鏈
    printf("驗證證書...\n");
    snprintf(command, sizeof(command),
             "openssl x509 -noout -in /IoT/openssl/certs/%s.cert.pem > /dev/null 2>&1", 
             serial_no);
    result = system(command);
    
    if (result == 0) {
        // 創建完整證書鏈
        snprintf(command, sizeof(command),
                 "cat /IoT/openssl/certs/%s.cert.pem "
                 "/IoT/openssl/certs/fev_charger_iot.intermediate.cert.pem "
                 "/IoT/openssl/certs/fev_charger_iot.root.ca.cert.pem > "
                 "/IoT/openssl/certs/%s-full-chain.cert.pem 2>/dev/null", 
                 serial_no, serial_no);
        system(command);
        
        printf("證書生成成功: %s\n", serial_no);
        
        // 更新系統狀態
        system("uci set system.@system[0].AzureKeyChange=0");
        system("uci commit system");
    } else {
        printf("證書驗證失敗，執行系統重置\n");
        system("sleep 1 && firstboot -y && reboot now");
        return -1;
    }
    
    system("sync");
    return 0;
}
