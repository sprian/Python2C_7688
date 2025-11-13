{{toc}}

# Azure IoT Daemon 0.9.13

## 簡介
Azure IoT守護進程，用於MT7688設備連接Azure IoT Hub。

## 版本特色
- ✅ 靜態連結，無外部依賴
- ✅ 修復ELF文件頭損壞問題  
- ✅ 支持TLS/SNI
- ✅ 支持設備配置服務(DPS)
- ✅ UCI配置系統集成

## 文件說明
- `azureiotd-0.9.13-mipsel-static` - 主執行文件 (靜態連結)
- `azureiotd-0.9.13-mipsel-static-backup` - 備份執行文件
- `src/` - 源代碼目錄
- `files/` - 配置和初始化文件

## 安裝說明
1. 將執行文件複製到 `/usr/sbin/azureiotd`
2. 設置執行權限: `chmod +x /usr/sbin/azureiotd`
3. 複製配置文件到 `/etc/config/azureiotd`
4. 啟動服務: `/etc/init.d/azureiotd start`

## 配置示例
```bash
config azureiotd
    option enabled '1'
    option hostname 'global.azure-devices.net'
    option device_id 'your-device-id'
    option auth_method 'sas'
```
## 故障排除
如果遇到段錯誤，請使用靜態連結版本。
詳見: SOLUTION_REPORT.md

## 版本 0.9.13a 更新

### 新增核心功能

#### 1. DPS 證書管理
- 自動設備證書生成
- 證書狀態檢查和驗證
- Azure DPS 集成支持

#### 2. Modbus RTU 通信
- 標準 Modbus RTU 協議
- 支持讀取/寫入寄存器
- 可擴展功能碼支持

#### 3. 靜態鏈接編譯
- 提供 `-static` 編譯選項
- 二進制文件獨立運行（無外部依賴）
- 文件大小：71K（優化後）

### 快速測試

```bash
# 編譯測試程序
cd package/azureiotd/src
mipsel-openwrt-linux-gcc -static -std=c99 -Os -o test_enhanced \
    dps_certificate.c modbus_rtu.c enhanced_main.c \
    -lm -lpthread

# 優化大小
mipsel-openwrt-linux-strip test_enhanced

# 部署到設備
scp test_enhanced root@192.168.1.1:/tmp/
ssh root@192.168.1.1 "/tmp/test_enhanced"
```

### 文件結構
```text
src/
├── dps_certificate.c/h    # DPS 證書管理
├── modbus_rtu.c/h        # Modbus RTU 通信  
├── enhanced_main.c       # 增強測試程序
├── azureiotd.c          # 原有主程序
└── [其他原有文件]
```

