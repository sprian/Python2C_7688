# readme_evi_lcdsvc.md
> 版本：v0.1.0　日期：2025-10-09

`evi-lcdsvc` 是 **LCD 仲裁常駐服務**，提供 Unix socket `/var/run/evi-lcd.sock` 給 C/Python/CLI 使用。

## 用途
- 序列化所有 LCD 操作，避免多執行緒/多行程同時存取 GPIO 造成亂碼與死鎖。
- 以 UCI 讀取 LCD 腳位設定（`/etc/config/lcd`），初始化硬體。

## Socket 協定（每行一指令）
- `INIT` / `CLEAR` / `HOME` / `INSTR 0xNN` / `DDRAM 0xNN` / `PUT <UTF8>`
- 回覆：`OK\n` 或 `ERR <msg>\n`

## CLI（lcdctl）
```sh
lcdctl init
lcdctl put "HELLO"
lcdctl ddram 0x40
lcdctl put "READY"
lcdctl clear
```

## 安裝
```sh
opkg install evi-lcdsvc_*.ipk
/etc/init.d/evi-lcdsvc enable
/etc/init.d/evi-lcdsvc start
```

## 注意事項
- Socket 路徑：`/var/run/evi-lcd.sock`（權限預設 `0666`）。
- 鎖檔：`/var/run/evi-lcd.lock`（避免重複 daemon）。
- UCI 設定：`/etc/config/lcd`（修改後需重啟服務）。
