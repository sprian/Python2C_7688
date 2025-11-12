# 外部程式呼叫範例對照（v0.9.7 ↔ azureSend.py）
> 版本日期：2025-10-09

`azureSend.py` 與 `evi-azurectl v0.9.7` 的定位不同：前者是雲端連線送訊腳本，後者是本地 USB 控制 CLI。兩者沒有共用相同的命令介面，但可並行使用、互不干擾。

---

## 1) azureSend.py（Python）
**用途**：連線 Azure / 註冊 / 傳送。**UCI 影響**：`uci set system.@system[0].mqtt=<值>` → `uci commit system`。**執行例**：
```sh
python /usr/bin/azureSend.py
# 程式會印出註冊/連線狀態（可能包含 success/fail），並更新 UCI：system.@system[0].mqtt
uci get system.@system[0].mqtt
```

---

## 2) evi-azurectl v0.9.7（C）
**用途**：本地 USB dongle 控制／查詢；Ping 參數管理。**UCI 影響**：`dongleport`（寫）、`usbdongle`（讀）、`pingsettings`（寫）。**常用指令**：
```sh
# 讀 dongle 狀態（純讀 UCI，不即時查詢）
/usr/bin/evi-azurectl --get getUsbDongle
# 範例輸出：usbdongle,1,1,0,0,1

# 開/關某一路（<valStr> 可 16 進位 01 或 十進位 1）
/usr/bin/evi-azurectl --set setDevOn,01   # 回傳：devon,01（並置位 UCI bit）
/usr/bin/evi-azurectl --set setDevOff,01  # 回傳：devoff,01（並清位 UCI bit）

# 設 Ping
/usr/bin/evi-azurectl --set setPingSettings,8   # 回傳：pingsettings,8
```

---

## 3) 並行測試（互不干擾）
```sh
# 先由 azureSend.py 更新 mqtt
python /usr/bin/azureSend.py
uci get system.@system[0].mqtt

# 再操作本地 USB，不應影響 mqtt
/usr/bin/evi-azurectl --set setDevOn,01
uci get system.@system[0].mqtt   # 應保持不變
```
