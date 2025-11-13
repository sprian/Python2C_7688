# Azure IoT v0.9.13a 部署包

## 文件說明
- `test_enhanced` - 測試程序 (靜態鏈接，71K)
- `deploy_enhanced.sh` - 部署腳本
- `simple_test.sh` - 簡單測試腳本
- `build_test_standalone.sh` - 獨立編譯腳本

## 使用方法

### 1. 部署到設備
```bash
./deploy_enhanced.sh [設備IP] [目標路徑]
```

### 2. 在設備上運行
```bash
./test_enhanced
```

### 3. 重新編譯（如果需要）
```bash
./build_test_standalone.sh
```

## 新功能
✅ DPS 證書管理
✅ Modbus RTU 通信
✅ 靜態鏈接編譯
