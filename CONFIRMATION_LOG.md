# CONFIRMATION_LOG — 逐檔確認紀錄

- 版本：v0.9.7 （2025-10-09）
- 確認檔名與狀態：
  - `package/evi-azurectl/src/main.c` — **已修正**（UCI 對齊 python）
  - `package/evi-azurectl/src/usb_dongle.c` — 一問一答、RX/TX 清空、查詢累積讀取 ≥40B/4行（沿用 v0.9.5 邏輯）
- 回傳值與 UCI 行為與 **azureIoT.py** 一致：
  - `setPingSettings`：`pingsettings,<val>` ✅
  - `setDevOn/Off`：`devon/devoff,<valStr>`；並更新 `dongleport` ✅
  - `getUsbDongle`：`usbdongle,<usbdongle>,<b0,b1,b2,b3>`（UCI） ✅
