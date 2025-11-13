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
