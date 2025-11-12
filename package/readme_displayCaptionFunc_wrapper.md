# readme_displayCaptionFunc_wrapper.md
> 版本：v0.1.0　日期：2025-10-09

`displayCaptionFunc` **wrapper** 是原 `displayCaptionFunc.py` 的**即插即用（drop-in）替代**版本，
內部不再直接操作 GPIO，而是透過 **同一份 API**（`libevi_lcd`）連到 **LCD 仲裁常駐服務 `lcdsvc`（Unix socket）**。
如此可讓 **C 與 Python** 在多執行緒 / 多行程下**安全並行**使用 LCD。

---

## 用途（Why）
- 維持原 Python UI 模組（`capCharger*` 系列）對 `displayCaptionFunc` 的**呼叫介面不變**。
- 將實作改為呼叫 **`libevi_lcd` → `/var/run/evi-lcd.sock` → `lcdsvc`**，由 daemon **序列化**所有硬體操作。
- 讓 C 與 Python **共用一條路徑**，降低維護成本與競態風險。

---

## 相依（Dependencies）
- **仲裁服務**：`evi-lcdsvc`（提供 `/var/run/evi-lcd.sock`）。
- **客戶端函式庫**：`evi-lcdclient`（安裝 `libevi_lcd.so` 與 Python 包裝 `evi_lcd.py`）。
- **UCI 腳位設定**：`/etc/config/lcd`（`rs/rw/en/d0..d7/bits`）。

> 請先確認 `lcdsvc` 已啟動再使用本 wrapper。

---

## API 對照（Compatibility Map）
| 原 `displayCaptionFunc.py` | Wrapper 內部對應 | 備註 |
|---|---|---|
| `write_instructions(val)` | `lcd.instr(val)` | 傳送 1 byte 指令（如 0x0C 顯示ON/游標OFF/閃爍OFF） |
| `write_letters(wordStr)`  | `lcd.put(wordStr)` | 以 UTF-8 輸出字串到目前游標 |
| `write_graphic(data)`     | `lcd.put(data)` | 先以文字模式代用；若需圖形模式可後續擴充（ST7920/GDRAM） |
| `LCD_clr()`               | `lcd.clear()` | 清屏 |
| `DDRAM_ADDR(pos)`         | **保留原計算** | 12 chars/line 的歷史位址計算（0x80/0x90）；可依實際面板調整 |
| `GDRAM_ADDR(val)`         | **保留原行為** | 目前返回 `val | 0x80`（占位） |
| `delay72u()` / `delay1600u()` | **no-op** | 時序交由 daemon 控制 |
| `dispStartLogo(...)`      | `lcd.clear()` + `lcd.put("STARTING...")` | 可自行改字串或替換為專案 Logo |

> 若你原程式使用到其它少見函式，請告知，我會補齊 wrapper 對應。

---

## 使用範例（Examples）

### 1) 舊程式不改匯入名稱（drop-in）
```python
# 與以前相同的呼叫方式
import displayCaptionFunc as lcd

lcd.write_instructions(lcd.DDRAM_ADDR(0))  # 第一行開頭
lcd.write_letters("HELLO EVSE")
lcd.write_instructions(lcd.DDRAM_ADDR(0x40))  # 第二行開頭（常見 2x16/2x20 的偏移）
lcd.write_letters("READY")
```

### 2) 與 `evi_lcd` 直接搭配（可選）
```python
import evi_lcd as lcd
lcd.init(); lcd.clear()
lcd.ddram(0x00); lcd.put("LINE1")
lcd.ddram(0x40); lcd.put("LINE2")
```

---

## 安裝（Installation）
1. 安裝 **服務與函式庫**：`evi-lcdsvc`、`evi-lcdclient`。  
   - 服務啟動：`/etc/init.d/evi-lcdsvc enable && /etc/init.d/evi-lcdsvc start`
2. 將 wrapper 覆蓋原檔：
   ```sh
   # 假設你的模組放在 /usr/bin 或專案路徑下
   cp displayCaptionFunc.py.replacement /usr/bin/displayCaptionFunc.py
   # 或覆蓋到你的專案目錄（保持模組名稱為 displayCaptionFunc.py）
   ```
3. 測試：
   ```sh
   python - <<'PY'
import displayCaptionFunc as lcd
lcd.LCD_clr()
lcd.write_instructions(lcd.DDRAM_ADDR(0))
lcd.write_letters("HELLO")
lcd.write_instructions(lcd.DDRAM_ADDR(0x40))
lcd.write_letters("WORLD")
print("OK")
PY
   ```

---

## 注意事項（Notes）
- **仲裁**：跨行程仲裁由 `lcdsvc` 負責；行程內再由 `libevi_lcd` 的 `pthread_mutex` 互斥，避免交錯。
- **面板行列/位址**：`DDRAM_ADDR` 以 12 chars/line 的歷史行為實作；若你的 LCD 為 16/20 列，請改用常見位址（0x00/0x40/0x14/0x54）或更新計算邏輯。
- **字型/編碼**：建議以 UTF-8；非 ASCII 字形需面板字庫支援或另行繪字（未含 CGRAM 例子）。
- **相容性**：`write_graphic()` 目前以文字模式代用；若需要 ST7920/GDRAM 圖形，請提出面板型號與需求，我會擴充 `lcdsvc` 與 `libevi_lcd`。
- **故障排查**：
  - `FileNotFoundError: /var/run/evi-lcd.sock` → 確認 `evi-lcdsvc` 已啟動。
  - `ImportError: evi_lcd` → 確認 `evi-lcdclient` 已安裝，`evi_lcd.py` 在 Python 路徑中。
  - 權限問題 → 檢查 `/var/run/evi-lcd.sock` 權限（預設 `0666`）。

---

## 回退（Rollback）
若需回到舊版，將原始 `displayCaptionFunc.py` 覆蓋回來即可（或把 wrapper 改名保留）。

---

## 版本與授權
- 版本：v0.1.0（與 `evi-lcdsvc` / `evi-lcdclient` 對齊）
- 授權：依專案策略（預設內部專案使用）
