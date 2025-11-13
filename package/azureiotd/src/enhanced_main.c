#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "dps_certificate.h"
#include "modbus_rtu.h"

// 全局變量
static int g_running = 1;

// 信號處理函數
void signal_handler(int sig) {
    printf("收到信號 %d，準備退出...\n", sig);
    g_running = 0;
}

// 測試 DPS 證書功能
void test_dps_certificate(void) {
    printf("\n=== 測試 DPS 證書功能 ===\n");
    
    dps_cert_manager_t cert_mgr;
    memset(&cert_mgr, 0, sizeof(cert_mgr));
    
    // 獲取序列號
    char *serial_no = dps_get_serial_number();
    if (serial_no) {
        printf("設備序列號: %s\n", serial_no);
        
        // 檢查證書是否存在
        if (dps_check_certificates_exist(serial_no)) {
            printf("證書已存在\n");
        } else {
            printf("證書不存在，需要生成\n");
            
            // 生成證書
            if (dps_generate_certificates(&cert_mgr) == 0) {
                printf("證書生成成功\n");
            } else {
                printf("證書生成失敗\n");
            }
        }
    } else {
        printf("無法獲取序列號\n");
    }
}

// 測試 Modbus 功能
void test_modbus_rtu(void) {
    printf("\n=== 測試 Modbus RTU 功能 ===\n");
    
    modbus_rtu_t modbus;
    memset(&modbus, 0, sizeof(modbus));
    
    // 初始化 Modbus
    if (modbus_rtu_init(&modbus, "/dev/ttyS0", 115200) == 0) {
        printf("Modbus 初始化成功\n");
        
        // 測試讀取寄存器
        unsigned char buffer[32];
        if (modbus_read_register(&modbus, 0x3000, 4, buffer) == 0) {
            printf("讀取寄存器成功: ");
            for (int i = 0; i < 4; i++) {
                printf("%02X ", buffer[i]);
            }
            printf("\n");
        }
        
        // 測試寫入寄存器
        modbus_write_register(&modbus, 0x5000, 0x1234);
        
        // 清理
        modbus_rtu_cleanup(&modbus);
    } else {
        printf("Modbus 初始化失敗\n");
    }
}

int main(int argc, char *argv[]) {
    printf("=== Azure IoT Daemon 增強版測試程序 ===\n");
    printf("使用靜態連結編譯\n\n");
    
    // 註冊信號處理
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // 測試各個模塊
    test_dps_certificate();
    test_modbus_rtu();
    
    printf("\n=== 測試完成 ===\n");
    //printf程序運行中... 按 Ctrl+C 退出\n");
    printf("Progeam running ... Press Ctrl+C to exit\n");
    
    // 主循環
    while (g_running) {
        sleep(1);
    }
    
    printf("程序正常退出\n");
    return 0;
}
