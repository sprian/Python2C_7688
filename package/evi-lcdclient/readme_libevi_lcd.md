# readme_libevi_lcd.md
> 版本：v0.1.0　日期：2025-10-09

`libevi_lcd` 是 **LCD 客戶端函式庫**。它透過 **Unix socket** 連線到仲裁常駐服務 **`lcdsvc`**，將來自 **C 與 Python** 的顯示請求**序列化**，避免並發踩踏 GPIO。

---

## 用途
- 以**同一份 API**（C 函式庫 + Python 包裝）供 **C 程式**與 **Python 腳本**呼叫。
- 內部以 socket 對 `/var/run/evi-lcd.sock` 發送命令，由 `lcdsvc` 負責**仲裁與時序**。

---

## API（C）
標頭：`#include <evi_lcd.h>`

```c
int evi_lcd_init(void);
int evi_lcd_clear(void);
int evi_lcd_home(void);
int evi_lcd_instr(uint8_t v);
int evi_lcd_ddram(uint8_t addr);
int evi_lcd_put(const char* s);
int evi_lcd_printf(const char* fmt, ...);
void evi_lcd_set_socket(const char* path);
```

**回傳值**：`0` 成功；負值代表錯誤（一般可能為：`-2` 建立 socket 失敗、`-3` 連線失敗、`-4` 讀回覆失敗、`-5` 伺服器錯誤）。

---

## API（Python）
模組：`import evi_lcd as lcd`

```python
lcd.set_socket("/var/run/evi-lcd.sock")
lcd.init()
lcd.clear()
lcd.home()
lcd.instr(0x0C)
lcd.ddram(0x00)
lcd.put("HELLO EVSE")
lcd.ddram(0x40)
lcd.put("READY")
```

---

## 範例

### C 範例
```c
#include <evi_lcd.h>
int main(void){
    evi_lcd_init();
    evi_lcd_clear();
    evi_lcd_ddram(0x00);
    evi_lcd_put("HELLO EVSE");
    evi_lcd_ddram(0x40);
    evi_lcd_put("READY");
    return 0;
}
```

### Python 範例
```python
import evi_lcd as lcd
lcd.init()
lcd.clear()
lcd.ddram(0x00); lcd.put("HELLO")
lcd.ddram(0x40); lcd.put("WORLD")
```

---

## 安裝（OpenWrt 15.05.1, mt7688）
1. 先安裝仲裁服務 `evi-lcdsvc`（確保 `/var/run/evi-lcd.sock` 存在且服務啟動）。
2. 安裝本套件 `evi-lcdclient`：會安裝 `/usr/lib/libevi_lcd.so`、`/usr/include/evi_lcd.h`、Python 包裝 `evi_lcd.py`。

### C 專案連結
```
$(CC) app.c -o app -levi_lcd -lpthread
# 或指定 -L/usr/lib -levi_lcd
```

### Python
```
python -c "import evi_lcd as lcd; print(lcd.init())"
```

---

## 注意事項
- 需要 `lcdsvc` 在跑：`/etc/init.d/evi-lcdsvc start`。
- UCI 腳位設定：`/etc/config/lcd`（`rs/rw/en/d0..d7/bits`）；修改後重啟服務。
- 並發安全：行程內由函式庫 `pthread_mutex` 保護；跨行程由 `lcdsvc` socket 單入口序列化。
- 編碼：建議輸入 UTF-8；非 ASCII 字形需 LCD 字庫支援或自定 CGRAM。
- 權限：socket 預設 `0666`，可依需求調整。
