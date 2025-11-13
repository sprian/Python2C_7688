#!/bin/bash
# 創建完整部署包

echo "=== 創建 Azure IoT v0.9.13a 部署包 ==="

# 設置變量
PACKAGE_DIR="deployment_package_v0.9.13a"
mkdir -p $PACKAGE_DIR

# 複製二進制文件
echo "1. 複製二進制文件..."
if [ -f "src/test_enhanced" ]; then
    cp src/test_enhanced $PACKAGE_DIR/
    echo "✅ test_enhanced 已複製"
else
    echo "❌ test_enhanced 未找到，請先編譯"
    exit 1
fi

# 複製腳本
echo "2. 複製腳本..."
cp scripts/deploy_enhanced.sh $PACKAGE_DIR/
cp scripts/simple_test.sh $PACKAGE_DIR/
cp scripts/build_test_standalone.sh $PACKAGE_DIR/
echo "✅ 腳本已複製"

# 創建說明文件
echo "3. 創建說明文件..."
cat > $PACKAGE_DIR/README.md << 'README_EOF'
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
README_EOF

echo "✅ 說明文件已創建"

## 設置權限
chmod +x $PACKAGE_DIR/*.sh
chmod +x $PACKAGE_DIR/test_enhanced

## 創建壓縮包
```bash
echo "4. 創建壓縮包..."
tar czf azureiotd_v0.9.13a_deployment.tar.gz $PACKAGE_DIR/

echo "✅ 部署包創建完成: azureiotd_v0.9.13a_deployment.tar.gz"
echo ""
echo "📦 包含文件:"
ls -la $PACKAGE_DIR/
echo ""
echo "🚀 使用方法:"
echo "tar xzf azureiotd_v0.9.13a_deployment.tar.gz"
echo "cd deployment_package_v0.9.13a"
echo "./deploy_enhanced.sh 192.168.1.1 /tmp"
```

## 運行完整部署包創建

```bash
# 運行部署包創建
./scripts/create_deployment_package.sh
```

## 更新 GitHub 倉庫
```bash
# 更新到 GitHub
cd ~/Python2C_7688/

# 添加新文件
git add package/azureiotd/scripts/build_test_standalone.sh
git add package/azureiotd/scripts/create_deployment_package.sh

# 提交更新
git commit -m "feat: 添加獨立編譯和部署包腳本

- 添加 build_test_standalone.sh 用於獨立編譯測試程序
- 添加 create_deployment_package.sh 創建完整部署包
- 解決 OpenWrt 編譯系統不執行額外編譯規則的問題
- 提供完整的部署解決方案"

git push origin main
```



