#!/bin/bash
echo "=== Azure IoT Daemon 0.9.13 部署腳本 ==="

# 檢查參數
if [ "$1" = "install" ]; then
    echo "安裝 Azure IoT Daemon..."
    cp azureiotd-0.9.13-mipsel-static /usr/sbin/azureiotd
    chmod +x /usr/sbin/azureiotd
    echo "安裝完成"
    
elif [ "$1" = "backup" ]; then
    echo "創建備份..."
    cp azureiotd-0.9.13-mipsel-static-backup /usr/sbin/azureiotd.backup
    chmod +x /usr/sbin/azureiotd.backup
    echo "備份完成"
    
elif [ "$1" = "restore" ]; then
    echo "從備份恢復..."
    cp /usr/sbin/azureiotd.backup /usr/sbin/azureiotd
    chmod +x /usr/sbin/azureiotd
    echo "恢復完成"
    
else
    echo "用法: $0 {install|backup|restore}"
    echo "  install - 安裝新版本"
    echo "  backup - 創建備份"
    echo "  restore - 從備份恢復"
fi
