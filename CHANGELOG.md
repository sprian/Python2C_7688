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
