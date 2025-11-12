# 對照結果（v0.9.7 ↔ azureSend.py）
> 版本日期：2025-10-09

本文件比對 **C 版 v0.9.7**（`evi-azurectl`）與 **azureSend.py** 在「**回傳值**」與「**UCI 存取**」的行為是否一致（或至少不衝突）。

---

## 1) 功能／欄位重疊範圍判定
`azureSend.py` 著重於 **雲端連線送訊**與**註冊流程**；在 UCI 上僅見：
- `uci set system.@system[0].mqtt=<某值>` → `uci commit system`（程式內部判定值）
- 會印出連線／註冊狀態（非供其他程式解析的介面）

`evi-azurectl v0.9.7` 著重於 **USB 繼電器控制與查詢**，涉及 UCI：
- 讀：`system.@system[0].usbdongle`、`system.@system[0].dongleport`
- 寫：`system.@system[0].dongleport`、`system.@system[0].pingsettings`（成功時 `uci commit system`）

> 結論：兩者的直接重疊欄位僅在 **UCI 套件層級（system 套件）**，**未使用相同的 key**，因此無衝突。

---

## 2) 比對表（回傳值／UCI 存取）

| 項目 | azureSend.py 行為 | C v0.9.7 行為 | 判定 |
|---|---|---|---|
| 回傳／輸出 | 僅印出註冊/連線狀態（非命令式回傳） | 透過 CLI 回傳固定格式字串（如 `usbdongle,...`、`devon/devoff,...`、`pingsettings,...`） | ✅ 無衝突 |
| UCI：`system.@system[0].mqtt` | **寫入**（`uci set` + `uci commit`） | **不讀不寫** | ✅ 一致（互不干擾） |
| UCI：`system.@system[0].dongleport` | **不操作** | `setDevOn/Off` 成功時**更新位元**並 `commit`；`getUsbDongle` 讀取 | ✅ 一致 |
| UCI：`system.@system[0].usbdongle` | **不操作** | `getUsbDongle` 讀取 | ✅ 一致 |
| UCI：`system.@system[0].pingsettings` | **不操作** | `setPingSettings` 會 `uci set` + `commit` | ✅ 一致 |

---

## 3) 結論
- 在 **回傳值** 與 **UCI 存取** 的可比對範圍內，**v0.9.7 與 azureSend.py 一致（無衝突）**，目前無需修正。

---

## 4) 驗證建議
```sh
# 由 azureSend.py 產生/更新 MQTT 設定（實際值由程式流程決定）
python /usr/bin/azureSend.py
uci get system.@system[0].mqtt

# 確認 evi-azurectl 不影響 mqtt
/usr/bin/evi-azurectl --get getUsbDongle
uci get system.@system[0].mqtt   # 應保持不變
```
