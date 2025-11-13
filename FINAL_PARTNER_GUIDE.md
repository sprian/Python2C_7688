# Azure IoT v0.9.13a 最終測試指南

## 🎯 新功能
- ✅ DPS 證書自動生成
- ✅ Modbus RTU 通信支持  
- ✅ 靜態鏈接編譯
- ✅ 測試程序 (71K 優化版)

## 📥 獲取代碼
```bash
git clone https://github.com/sprian/Python2C_7688.git
cd Python2C_7688
```

## 🔧 編譯方法
cat > FINAL_PARTNER_GUIDE.md << 'EOF'
# Azure IoT v0.9.13a 最終測試指南

## 🎯 新功能
- ✅ DPS 證書自動生成
- ✅ Modbus RTU 通信支持  
- ✅ 靜態鏈接編譯
- ✅ 測試程序 (71K 優化版)

## 📥 獲取代碼
```bash
git clone https://github.com/sprian/Python2C_7688.git
cd Python2C_7688
```

## 🔧 編譯方法

### 方法一：獨立編譯（推薦）

```bash
cd package/azureiotd
./scripts/build_test_standalone.sh
```

### 方法二：OpenWrt 編譯

```bash
cp -r package/azureiotd ~/ai7688/SVN/ChargingPile_G4/trunk/package/
cd ~/ai7688/SVN/ChargingPile_G4/trunk
make package/azureiotd/compile V=99
```

## 🚀 部署測試
### 快速部署
```bash
cd package/azureiotd
./scripts/deploy_enhanced.sh 192.168.1.1 /tmp
```

### 完整部署包
```bash
./scripts/create_deployment_package.sh
# 會生成 azureiotd_v0.9.13a_deployment.tar.gz
```

## 📁 文件結構
```text
src/
├── dps_certificate.c/h    # DPS 證書管理
├── modbus_rtu.c/h        # Modbus RTU 通信
├── enhanced_main.c       # 測試程序主入口
└── test_enhanced         # 編譯後的測試程序 (71K)

scripts/
├── deploy_enhanced.sh    # 部署腳本
├── build_test_standalone.sh # 獨立編譯腳本
└── create_deployment_package.sh # 部署包創建
```

## ✅ 驗證步驟
1. 編譯測試程序

2. 部署到設備

3. 運行測試程序

4. 檢查功能是否正常

## 🆘 故障排除
* 如果編譯失敗，檢查工具鏈環境

* 如果部署失敗，檢查網絡連接

* 如果運行失敗，檢查設備架構兼容性
