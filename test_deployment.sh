#!/bin/bash
echo "=== Azure IoT Daemon 0.9.13 部署測試 ==="

# 測試二進制文件
if [ -f "bin/azureiotd/0.9.13/azureiotd-0.9.13-mipsel-static" ]; then
    echo "✅ 二進制文件存在"
    file bin/azureiotd/0.9.13/azureiotd-0.9.13-mipsel-static
else
    echo "❌ 二進制文件缺失"
fi

# 測試部署腳本
if [ -f "bin/azureiotd/0.9.13/deploy.sh" ]; then
    echo "✅ 部署腳本存在"
    chmod +x bin/azureiotd/0.9.13/deploy.sh
else
    echo "❌ 部署腳本缺失"
fi

# 測試源代碼
if [ -f "package/azureiotd/src/azureiotd.c" ]; then
    echo "✅ 源代碼文件存在"
else
    echo "❌ 源代碼文件缺失"
fi

echo "=== 測試完成 ==="
