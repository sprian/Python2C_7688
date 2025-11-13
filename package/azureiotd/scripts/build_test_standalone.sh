#!/bin/bash
# 獨立編譯測試程序腳本

echo "=== 獨立編譯測試程序 ==="

# 設置環境
export STAGING_DIR=/home/ai7688/ai7688/SVN/ChargingPile_G4/trunk/staging_dir
export PATH=$STAGING_DIR/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2/bin:$PATH

# 編譯測試程序
cd src
echo "編譯 test_enhanced..."
mipsel-openwrt-linux-gcc -static -std=c99 -Os -o test_enhanced \
    dps_certificate.c modbus_rtu.c enhanced_main.c \
    -lm -lpthread

if [ $? -eq 0 ]; then
    echo "✅ 編譯成功"
    ls -lh test_enhanced
    echo "文件已生成: src/test_enhanced"
    
    # 優化大小
    echo "優化文件大小..."
    mipsel-openwrt-linux-strip test_enhanced
    ls -lh test_enhanced
else
    echo "❌ 編譯失敗"
    exit 1
fi
