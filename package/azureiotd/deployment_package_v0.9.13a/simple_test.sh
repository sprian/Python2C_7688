#!/bin/bash
# 簡單測試腳本 - 直接複製到 OpenWrt 環境測試

echo "=== Azure IoT v0.9.13a 簡單測試 ==="

# 檢查必要文件
echo "1. 檢查文件..."
files=(
    "src/dps_certificate.c"
    "src/dps_certificate.h"
    "src/modbus_rtu.c" 
    "src/modbus_rtu.h"
    "src/enhanced_main.c"
    "Makefile"
)

for file in "${files[@]}"; do
    if [ -f "$file" ]; then
        echo "✅ $file"
    else
        echo "❌ $file 缺失"
        exit 1
    fi
done

# 檢查 Makefile 修改
echo "2. 檢查 Makefile 修改..."
if grep -q "ENHANCED_SRC_FILES" Makefile && grep -q "test_enhanced" Makefile; then
    echo "✅ Makefile 修改正確"
else
    echo "❌ Makefile 修改不完整"
    exit 1
fi

echo "✅ 簡單測試完成，可以進行實際編譯測試"
