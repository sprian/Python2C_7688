# ERRATA (v0.9.0)

- UI 快取以畫面ID+參數為鍵；相同呼叫不重畫。
- getMValue/dispComplete 目前固定版面；參數僅作為快取鍵。
- RW 預設接地，未讀忙碌旗標；如需請告知。
- sysfs GPIO 效能有限；後續可改 fastgpio/SPI。
- 暫無 force redraw API；可切畫面代替。
