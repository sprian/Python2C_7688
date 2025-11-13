#!/bin/bash
# 增強版本部署腳本 - v0.9.13a
# 用法: ./deploy_enhanced.sh [設備IP] [目標路徑]

DEVICE_IP="${1:-192.168.1.1}"
TARGET_PATH="${2:-/tmp}"
ENHANCED_BIN="test_enhanced"

echo "=== Azure IoT 增強版本部署 v0.9.13a ==="
echo "目標設備: $DEVICE_IP"
echo "部署路徑: $TARGET_PATH"

# 檢查二進制文件
if [ ! -f "src/$ENHANCED_BIN" ]; then
    echo "錯誤: 測試程序未找到，請先編譯"
    echo "編譯命令:"
    echo "  cd src && mipsel-openwrt-linux-gcc -static -std=c99 -Os -o test_enhanced \\"
    echo "      dps_certificate.c modbus_rtu.c enhanced_main.c -lm -lpthread"
    exit 1
fi

# 檢查文件大小
FILE_SIZE=$(stat -c%s "src/$ENHANCED_BIN")
echo "二進制文件大小: $((FILE_SIZE/1024))K"

# 傳輸文件
echo "傳輸文件到設備..."
scp "src/$ENHANCED_BIN" root@$DEVICE_IP:$TARGET_PATH/

if [ $? -eq 0 ]; then
    echo "✓ 文件傳輸成功"
else
    echo "✗ 文件傳輸失敗"
    exit 1
fi

# 在設備上運行測試
echo "在設備上運行測試程序..."
ssh root@$DEVICE_IP "
    cd $TARGET_PATH
    chmod +x $ENHANCED_BIN
    echo '=== 測試開始 ==='
    ./$ENHANCED_BIN
    TEST_RESULT=\$?
    echo '=== 測試結束 ==='
    echo '退出代碼: '\$TEST_RESULT
    exit \$TEST_RESULT
"

echo "=== 部署完成 ==="
