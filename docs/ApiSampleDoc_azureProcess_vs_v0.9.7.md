# 外部程式呼叫範例對照 — azureProcess.py vs evi-azurectl v0.9.7
> 版本日期：2025-10-09

`azureProcess.py`：偏重 **雲端連線 / 綁定 / 發佈回報** 流程，主要透過 **UCI** 交換狀態或轉送命令。  
`evi-azurectl v0.9.7`：偏重 **本地 USB 繼電器控制/查詢** 與 **Ping 設定** 的 CLI。

下列對照讓你在**測試/替換**時，看得出 **azureProcess 觸發的行為** 與 **C 版 CLI** 的**等效呼叫**與**輸出**。

---

## 呼叫對照總表

| 功能 | azureProcess.py（觸發/行為） | evi-azurectl v0.9.7（等效 CLI） | 典型輸出（兩邊規格一致） | UCI 影響 |
|---|---|---|---|---|
| 讀 USB 狀態 | 透過內部 `returnMsg('getUsbDongle')` 回報，目前狀態由 **UCI** 取得 | `/usr/bin/evi-azurectl --get getUsbDongle` | `usbdongle,<ud>,<b0,b1,b2,b3>`<br>例：`usbdongle,1,1,0,0,1` | 讀 `system.@system[0].usbdongle`、`system.@system[0].dongleport` |
| 開某一路 | 由雲端指令（如 C2D/方法）驅動 `setDevOn,<valStr>` 流程，最終需控制 USB 並回報 | `/usr/bin/evi-azurectl --set setDevOn,01` | `devon,01` | 成功則**置位** `dongleport` 的對應 bit 並 `uci commit system` |
| 關某一路 | 類似上列，雲端驅動 `setDevOff,<valStr>` | `/usr/bin/evi-azurectl --set setDevOff,01` | `devoff,01` | 成功則**清位** `dongleport` 的對應 bit 並 `uci commit system` |
| 設定 Ping | 雲端下發參數，流程寫入 UCI | `/usr/bin/evi-azurectl --set setPingSettings,8` | `pingsettings,8` | `uci set system.@system[0].pingsettings=8` → `uci commit system` |

> 註 1：`<ud>` 代表 UCI `system.@system[0].usbdongle`（0/1）；`<b0..b3>` 由 `dongleport` 的 bit0~bit3 轉出（bit0→CH1, bit1→CH2, bit2→CH3, bit3→CH4）。  
> 註 2：`<valStr>` 支援 **16 進位**（如 `01`）或 **十進位**（如 `1`）；**回傳沿用原字串**。

---

## 實際呼叫與驗證範例

### 1) 讀取 USB 狀態（azureProcess 會回報同一格式）
```sh
# C 版等效呼叫
/usr/bin/evi-azurectl --get getUsbDongle
# 可能輸出：
# usbdongle,1,1,0,1,0

# 驗證 UCI 來源（兩者都讀這些欄位）
uci get system.@system[0].usbdongle
uci get system.@system[0].dongleport
```

### 2) 開啟/關閉某一路（<valStr> 可 16 進位 01 或 十進位 1）
```sh
# 開 CH1
/usr/bin/evi-azurectl --set setDevOn,01
# 輸出：devon,01

# 關 CH1（十進位也可）
/usr/bin/evi-azurectl --set setDevOff,1
# 輸出：devoff,1

# 驗證 UCI（bit0 置 1 / 清 0）
uci get system.@system[0].dongleport
```

### 3) 設定 Ping 參數
```sh
/usr/bin/evi-azurectl --set setPingSettings,8
# 輸出：pingsettings,8

# 驗證 UCI
uci get system.@system[0].pingsettings
```

---

## 並行運作與注意事項
- **azureProcess.py** 與 **evi-azurectl** 可**並行**：前者專注雲端/回報，後者專注本地控制。  
- `getUsbDongle` 的回報完全**以 UCI 為準**（不做即時 USB 讀取），因此只要 UCI 被正確維護，兩者輸出便一致。  
- 若 USB 拔除的偵測由其他程序維護（寫 `usbdongle` 與將 `dongleport=-1`），azureProcess 與 evi-azurectl 的回報會一致顯示 `-1,-1,-1,-1`。

---

## 錯誤/邊界一致性
- 無效或格式錯誤：回傳 `Wrong settings`（C 版對齊既有規格；azureProcess 會在上層流程攔截/轉譯錯誤）。
- `<valStr>` 支援 16/10 進位；**回傳沿用原字串**（利於雲端端到端比對）。
- `getUsbDongle` 不即時查 USB，**只讀 UCI**（兩邊一致）。
