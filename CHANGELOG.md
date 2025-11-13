# CHANGELOG

## v0.9.1
- 導入附件 libevi-0.9.0 為基線；新增 evi-python2c

## v0.9.2
- 新增 evi-azurectl（C 版骨架）

## v0.9.4
- evi-azurectl: 強化 usb_set_port() 讀取（≥40B/4行）

## v0.9.5 — 2025-10-09
- evi-azurectl: 一問一答機制，送指令前 `tcflush(TCIOFLUSH)`，結束後 `tcflush(TCIFLUSH)`；TX/RX 緩衝清零再用。

## v0.9.6 — 2025-10-09
- evi-azurectl: 新增 `usb_query_status()` 解析 4 行各 10 bytes（`CHx: ON \r\n`/`CHx: OFF\r\n`），並整合 `getUsbDongle` 輸出。

## v0.9.7 — 2025-10-09
- `getUsbDongle` 改為與 azureIoT.py 一致：輸出 `usbdongle,<usbdongle>,<b0,b1,b2,b3>`（完全來自 UCI）。
- `setDevOn/Off` 與 azureIoT.py 一致：回傳 `devon/devoff,<valStr>`；成功時更新 `system.@system[0].dongleport` 位元並 `uci commit system`；接受 16 進位或十進位的 `<valStr>`。

## 0.9.9 — 2025-10-14
- Default UCI pins set to RS=18, RW=17, EN=37, D0..D7=45,46,0,1,2,3,4,5
- LCD init Function Set changed to 0x30 (twice)
- Added Python shim: package/libevi/python/displayCaptionFunc.py

## 0.9.9 instructionDef fix — 2025-10-14
- displayCaptionFunc shim now supports raw opcodes via lcdsvc: `write_instructions(v)` → `INSTR 0xNN`.
- Added docs: docs/CheckResult_instructionDef.md, docs/ApiSampleDoc_instructionDef.md.

## 0.9.10 — 2025-10-16
- Integrated Azure IoT C SDK glue:
  - package/azureiotd (daemon, MQTT X.509, C2D -> Unix socket broadcast)
  - package/libevi-azure (client C library + Python wrapper)
- Keeps instructionDef fix & displayCaptionFunc shim.

## v0.9.12b2 — 2025-10-31
- OpenSSL 1.0.x 相容、修正 JSON 字串、加入診斷輸出。
- X.509：MQTT 3.1.1 over TLS 連線並發佈 QoS1 測試訊息。
# Azure IoT Daemon 更新日誌

## 版本 0.9.13 (2024-11-13)
### 修復
- **重大修復**: 解決ELF文件頭損壞問題
- **編譯優化**: 使用靜態連結避免庫依賴問題
- **穩定性**: 修復在MIPS平台上的段錯誤問題

### 變更
- 從動態連結改為靜態連結
- 文件大小: 12KB → 283KB (完整靜態版本)
- 移除有問題的strip工具處理

## 版本 0.9.12d (2024-11-12)
### 功能
- 添加TLS/SNI支持
- 改進證書處理
- 優化連接穩定性

## 版本 0.9.12c (2024-11-12)
### 修復
- 修復內存泄漏問題
- 改進錯誤處理

## 版本 0.9.12b (2024-11-12)
### 功能
- 添加DPS (Device Provisioning Service) 支持
- 改進配置管理

## 版本 0.9.12a (2024-11-12)
### 初始版本
- 基礎Azure IoT Hub連接
- 遙測數據發送
- UCI配置支持

## 版本 0.9.13a (2024-11-13)

### 新功能
- ✅ DPS 證書管理系統 (dps_certificate.c/h)
- ✅ Modbus RTU 通信支持 (modbus_rtu.c/h)  
- ✅ 靜態鏈接編譯選項
- ✅ 增強測試程序 (enhanced_main.c)

### 優化
- 靜態鏈接二進制文件從 501KB 優化到 71K
- 去除調試信息，減少存儲空間
- 保持與原有功能的向後兼容

### 測試驗證
- 靜態編譯測試通過
- 二進制文件可在 MIPS 設備獨立運行
