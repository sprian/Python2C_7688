# 外部程式呼叫範例對照 — azureIoT.py vs evi-azurectl v0.9.7
> 版本日期：2025-10-09

目標：在相同的外部呼叫介面下，示範 **Python 版（azureIoT.py）** 與 **C 版（evi-azurectl v0.9.7）** 的**呼叫方式**與**輸出格式**，便於替換測試。

---

## 共同呼叫規則
- 讀取：`--get <Command>`
- 設定：`--set <Command>,<value>`
- 範例：`--set setDevOn,01`（`01` 可為**16 進位**；亦可用 `1` 為**十進位**）

---

## 指令對照表

| 功能 | azureIoT.py（Python） | evi-azurectl v0.9.7（C） | 典型輸出（兩邊一致） | UCI 作用 |
|---|---|---|---|---|
| 讀 USB 狀態 | `python /usr/bin/azureIoT.py --get getUsbDongle` | `/usr/bin/evi-azurectl --get getUsbDongle` | `usbdongle,<ud>,<b0,b1,b2,b3>`<br>例：`usbdongle,1,1,0,0,1` | 讀 `system.@system[0].usbdongle`、`system.@system[0].dongleport` |
| 開某一路 | `python /usr/bin/azureIoT.py --set setDevOn,01` | `/usr/bin/evi-azurectl --set setDevOn,01` | `devon,01` | 成功則設置 `dongleport` 指定位元並 `uci commit system` |
| 關某一路 | `python /usr/bin/azureIoT.py --set setDevOff,01` | `/usr/bin/evi-azurectl --set setDevOff,01` | `devoff,01` | 成功則清除 `dongleport` 指定位元並 `uci commit system` |
| 設定 Ping | `python /usr/bin/azureIoT.py --set setPingSettings,8` | `/usr/bin/evi-azurectl --set setPingSettings,8` | `pingsettings,8` | `uci set system.@system[0].pingsettings=8` → `uci commit system` |

> 註：`<ud>` 代表 UCI `usbdongle`（0/1）；`<b0..b3>` 由 `dongleport` 的 bit0~bit3 轉出（bit0→CH1, bit1→CH2, bit2→CH3, bit3→CH4）。

---

## 實際呼叫範例

### 1) 讀取 USB 狀態
```sh
# Python
python /usr/bin/azureIoT.py --get getUsbDongle
# C
/usr/bin/evi-azurectl --get getUsbDongle
# 可能輸出：
# usbdongle,1,1,0,1,0
```

### 2) 開啟 CH1（16 進位）
```sh
# Python
python /usr/bin/azureIoT.py --set setDevOn,01
# C
/usr/bin/evi-azurectl --set setDevOn,01
# 兩邊輸出：
# devon,01

# 驗證 UCI（bit0 應置 1）
uci get system.@system[0].dongleport
```

### 3) 關閉 CH1（十進位亦可）
```sh
# Python
python /usr/bin/azureIoT.py --set setDevOff,1
# C
/usr/bin/evi-azurectl --set setDevOff,1
# 兩邊輸出：
# devoff,1

# 驗證 UCI（bit0 應清 0）
uci get system.@system[0].dongleport
```

### 4) 設定 Ping 參數
```sh
# Python
python /usr/bin/azureIoT.py --set setPingSettings,8
# C
/usr/bin/evi-azurectl --set setPingSettings,8
# 兩邊輸出：
# pingsettings,8

# 驗證 UCI
uci get system.@system[0].pingsettings
```

---

## 錯誤處理與邊界
- 無效或格式錯誤：回傳 `Wrong settings`（兩邊一致）。
- `<value>` 支援 **16 進位**與**十進位**，**回傳沿用原字串**。
- `getUsbDongle` **讀 UCI**，不做即時 USB 查詢（兩邊一致）。
