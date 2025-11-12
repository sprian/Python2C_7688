# 對照結果一覽 — azureProcess.py vs C 版 v0.9.7

本文件比對 **回傳值** 與 **UCI 存取** 是否與 `azureProcess.py` 的動作一致。重點聚焦在 azureProcess 與裝置互動會用到的命令/欄位（如 USB dongle、Ping 設定等）。其他純雲端連線註冊/心跳相關欄位僅列為參考。

> 參考來源：`azureProcess.py`（你提供的檔案）。

---

## 1) 指令與回傳值

| 功能 | azureProcess.py 呼叫/預期 | C v0.9.7 行為 | 判定 |
|---|---|---|---|
| **setDevOn,<valStr> / setDevOff,<valStr>** | 透過 `setCommand()` 觸發 USB 控制；預期回傳 `devon,<valStr>` / `devoff,<valStr>`，`<valStr>` 可為 **16 進位**（例如 `01`）或 **十進位**（例如 `1`），回傳沿用原字串；之後通常會再呼叫一次 `returnMsg('getUsbDongle')` 發佈最新狀態。 fileciteturn3file0 | `evi-azurectl --set setDevOn,01` / `--set setDevOff,01` → 回傳 **`devon,01` / `devoff,01`**；接受 16/10 進位，回傳沿用原字串。 | **一致** |
| **getUsbDongle** | 由 `returnMsg('getUsbDongle')` 組出：`usbdongle,<usbdongle>,<b0,b1,b2,b3>`，資料來源為 **UCI**（而非即時序列查詢）；當移除 dongle 時，`dongleport=-1`，輸出 `-1,-1,-1,-1`。 fileciteturn3file0 | `evi-azurectl --get getUsbDongle` → **讀 UCI** `usbdongle/dongleport` 輸出 `usbdongle,<ud>,<b0,b1,b2,b3>`；`dongleport<0` 時輸出 `-1,-1,-1,-1`。 | **一致** |
| **setPingSettings,<val>** | 設定 ping 參數；預期回傳 `pingsettings,<val>`。 fileciteturn3file0 | `evi-azurectl --set setPingSettings,8` → 回傳 **`pingsettings,8`**。 | **一致** |

---

## 2) UCI 存取對照

| UCI Key | azureProcess.py 動作 | C v0.9.7 動作 | 判定 |
|---|---|---|---|
| `system.@system[0].dongleport` | dongle 移除時設為 `-1`；透過 `setCommand(setDevOn/Off)` 切換某路後，最終會反映在此欄位，`getUsbDongle` 由此輸出 bit 狀態。 fileciteturn3file0 | `setDevOn/Off` 成功後**同步更新**對應位元並 `uci commit system`；`getUsbDongle` 讀此欄位輸出 `b0..b3`。 | **一致** |
| `system.@system[0].usbdongle` | 由偵測流程維護（插入/拔除）；`getUsbDongle` 讀取此欄位當作可用性 `<ud>`。 fileciteturn3file0 | `getUsbDongle` 讀此欄位輸出 `<ud>`；本程式不更動其值（留給偵測流程維護）。 | **一致** |
| `system.@system[0].pingsettings` | 以 `uci set` + `uci commit` 更新。 fileciteturn3file0 | `--set setPingSettings,<val>` → `uci set` + `uci commit`。 | **一致** |
| `system.@system[0].AzureBindingStatus` / `hubname` / `regid` / `AzureKeyChange` / `NetLink` / `MqttBindingStatus` / `unbinding` / `WebpageIsOpened` / `linkit_firstboot` | 由 **azureProcess.py** 的註冊/連線/事件流程讀寫（與雲端溝通相關）。 fileciteturn3file0 | **不變更**（不屬此 CLI 範疇）。 | **N/A** |

---

## 3) 判定與結論

- 針對 **回傳值** 與 **UCI 存取** 的重疊範圍（`setDevOn/Off`、`getUsbDongle`、`setPingSettings`）：C 版 v0.9.7 **與 azureProcess.py 一致**。  
- 其他雲端連線與綁定流程相關的 UCI 欄位（如 `AzureBindingStatus/hubname/regid/...`）皆為 **azureProcess.py** 內部管理，C 版 **不更動**，避免交互影響。

---

## 4) 可選強化（非必要）
- 若未來希望 **完全以 C 版** 取代 python 偵測邏輯，可新增：USB 型號/插拔偵測程序，維護 `usbdongle` 與移除時寫入 `dongleport=-1` 的行為，讓整體行為完全鏡像。

