# libevi v0.9.0 — Unified EVI Library

- 單一 **libevi.so**，整合 HAL + 畫面（caption/charge），提供 **C 與 Python** 介面。
- **Thread-safe**：內建 pthread mutex；**No-heap**：無 malloc、僅固定長度緩衝。
- **不重覆下指令**：畫面快取避免重畫；**相容舊 API**（libevi_caption.so symlink）。

## Errata（鏡像）
# ERRATA (v0.9.0)

- UI 快取以畫面ID+參數為鍵；相同呼叫不重畫。
- getMValue/dispComplete 目前固定版面；參數僅作為快取鍵。
- RW 預設接地，未讀忙碌旗標；如需請告知。
- sysfs GPIO 效能有限；後續可改 fastgpio/SPI。
- 暫無 force redraw API；可切畫面代替。

