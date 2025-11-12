# 對照結果 — azureIoT.py vs C 版 v0.9.7
> 版本日期：2025-10-09

此文件比對 **C 版 v0.9.7** 與 **azureIoT.py** 在「**回傳值**」與「**UCI 存取**」上的一致性。

---

## 1) 指令 / 輸出 對照

| 功能 | azureIoT.py 行為 | C v0.9.7 行為 | 判定 | 備註 |
|---|---|---|---|---|
| `--set setDevOn,<valStr>` | 送 A0 指令後（成功）**回傳** `devon,<valStr>`；允許 **16 進位**或**十進位**輸入；**更新 UCI** `system.@system[0].dongleport` 對應位元並 `uci commit system` | 與 python 一致：`devon,<valStr>`；接受 16/10 進位；成功時更新 `dongleport` 位元並 `uci commit system` | ✅ 一致 | 回傳沿用呼叫字串 |
| `--set setDevOff,<valStr>` | 同上，**回傳** `devoff,<valStr>`；成功時**清除** `dongleport` 位元並 `uci commit system` | 與 python 一致：`devoff,<valStr>`；成功時清除位元並 `commit` | ✅ 一致 | — |
| `--get getUsbDongle` | **僅讀 UCI** 狀態，不做即時 USB 查詢；輸出 `usbdongle,<usbdongle>,<b0,b1,b2,b3>`；若 `dongleport<0` → `-1,-1,-1,-1` | 與 python 一致：讀 `usbdongle`/`dongleport` 組字串；`dongleport<0` → `-1,-1,-1,-1` | ✅ 一致 | `b0..b3` 為 bit0~bit3 |
| `--set setPingSettings,<val>` | `uci set system.@system[0].pingsettings=<val>` → `uci commit system`；**回傳** `pingsettings,<val>` | 與 python 一致：同樣 `uci set` + `commit`；回傳 `pingsettings,<val>` | ✅ 一致 | — |

---

## 2) UCI 欄位 對照

| UCI Key | azureIoT.py | C v0.9.7 | 判定 |
|---|---|---|---|
| `system.@system[0].dongleport` | 維護各路位元；`getUsbDongle` 依此轉 `b0..b3` | `setDevOn/Off` 成功即時更新；`getUsbDongle` 讀此欄位 | ✅ |
| `system.@system[0].usbdongle` | 由偵測流程維護；`getUsbDongle` 僅讀取 | 本程式不改寫；`getUsbDongle` 僅讀取 | ✅ |
| `system.@system[0].pingsettings` | 以 `uci set` + `commit` 設定 | 同上 | ✅ |

---

## 3) 錯誤處理 / 邊界
- 不合法指令或格式 → 回傳 `Wrong settings`（兩邊一致）。
- `<valStr>` 支援 **16 進位**（如 `01`）與 **十進位**（如 `1`）；**回傳沿用原字串**。
- `getUsbDongle` 為 **UCI 驅動**（不即時查詢 USB），兩邊一致。

## 4) 測試建議（快速驗證）
```sh
# 讀取狀態（兩邊一致的輸出形態）
/usr/bin/evi-azurectl --get getUsbDongle

# 開 CH1（16 進位）；確認 UCI bit0 置 1
/usr/bin/evi-azurectl --set setDevOn,01
uci get system.@system[0].dongleport

# 關 CH1（十進位）；確認 UCI bit0 清 0
/usr/bin/evi-azurectl --set setDevOff,1
uci get system.@system[0].dongleport

# 設定 Ping；確認 UCI
/usr/bin/evi-azurectl --set setPingSettings,8
uci get system.@system[0].pingsettings
---

## 5) 結論
目前 **v0.9.7** 在回傳值與 UCI 存取上 **與 azureIoT.py 一致**，無需修正。

