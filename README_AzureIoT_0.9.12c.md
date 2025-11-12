# Azure IoT Daemon for AI7688 (OpenWrt 15.05.1)

## 版本: 0.9.12c

### 功能特性
- Azure IoT Hub 连接支持 (SAS Token & X.509 证书)
- 设备配置管理 (UCI 配置系统)
- TLS/SSL 安全连接
- SNI (Server Name Indication) 支持
- 自动重连和错误恢复
- 安全的字符串处理 (safe_str 模块)

### 编译说明
```bash
# 在 OpenWrt SDK 中编译
make package/feeds/packages/azureiotd/compile V=s
```
### 配置示例
```bash
config azureiotd 'main'
    option enable '1'
    option host 'your-hub.azure-devices.net'
    option device_id 'myDevice'
    option auth 'x509'
    option x509_cert '/etc/ssl/certs/device.crt'
    option x509_key '/etc/ssl/private/device.key'
    option keepalive '60'
```

### 新版本改进
更安全的字符串处理

改进的错误诊断

版本信息追踪

代码结构优化
