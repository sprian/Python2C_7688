# Azure IoT C SDK Build Helper (MT7688 / OpenWrt 15.05.1)

這個小工具包幫你把 **azure-iot-sdk-c (LTS_03_2025)** 交叉編譯成 MT7688 可用的 `build-mt7688` 目錄，
供 `azureiotd` 0.9.11n 自動連結（IoTHub + DPS + HTTP）。

## 快速步驟

1. 下載 Azure SDK：
   ```bash
   cd /home/ai7688/ai7688/py2c/azureProcess
   git clone https://github.com/Azure/azure-iot-sdk-c.git -b LTS_03_2025
   ```

2. 執行建置（預設啟用 HTTP 與 DPS）：
   ```bash
   cd /home/ai7688/ai7688/py2c/azureProcess/azure-iot-sdk-c
   /path/to/scripts/build_azure_sdk_mt7688.sh .
   ```

   *若你的工具鏈/路徑不同，可先輸出：*
   ```bash
   export STAGING_DIR=/home/ai7688/ai7688/SVN/ChargingPile_G4/trunk/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2
   export TOOLCHAIN_DIR=/home/ai7688/ai7688/SVN/ChargingPile_G4/trunk/staging_dir/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2
   ```

3. 驗證：
   ```bash
   /path/to/scripts/verify_azure_sdk_build.sh ./build-mt7688
   ```

4. 在 `azureiotd` 的 `make menuconfig`：
   - `External Azure IoT C SDK lib dir = /home/ai7688/ai7688/py2c/azureProcess/azure-iot-sdk-c/build-mt7688`

5. 重新編譯 `azureiotd`：
   ```bash
   make package/azureiotd/clean V=s
   make package/azureiotd/compile V=s
   ```

## 自訂參數
- 關閉 HTTP（若缺 libcurl/zlib）：
  ```bash
  ./scripts/build_azure_sdk_mt7688.sh . --http off
  ```
- 關閉 DPS（若只是測試純 IoTHub 或 UMQTT-only）：
  ```bash
  ./scripts/build_azure_sdk_mt7688.sh . --dps off
  ```
- 指定 toolchain triplet（預設 `mipsel-openwrt-linux`）：
  ```bash
  ./scripts/build_azure_sdk_mt7688.sh . --triplet mipsel-openwrt-linux
  ```

## 備註
- 此建置預設 `BUILD_SHARED_LIBS=OFF`（偏好 .a 靜態庫），對 OpenWrt 15.05.1 連結較穩定。
- 若開 HTTP 但找不到 `libcurl` / `zlib`，建議先在 OpenWrt SDK 補齊對應 *target* 套件或用 `--http off`。
- `verify_azure_sdk_build.sh` 會列出 `build-mt7688` 下關鍵庫檔，方便對照連結。
