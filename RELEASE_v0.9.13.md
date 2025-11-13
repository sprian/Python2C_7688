# 版本 0.9.13 發布說明

## 發布摘要
- **版本號**: 0.9.13
- **發布日期**: $(date +%Y-%m-%d)
- **狀態**: STABLE
- **類型**: 重大修復版本

## 修復內容
✅ ELF文件頭損壞問題 - 完全解決  
✅ MIPS平台段錯誤 - 已修復
✅ 庫依賴問題 - 改用靜態連結
✅ 打包過程損壞 - 已優化

## 文件結構
Python2C_7688/
├── package/azureiotd/ # 更新後的源代碼 (v0.9.13)
├── package/azureiotd-0.9.12d-backup/ # 舊版本備份
├── bin/azureiotd/0.9.13/ # 二進制文件
│ ├── azureiotd-0.9.13-mipsel-static
│ ├── azureiotd-0.9.13-mipsel-static-backup
│ └── deploy.sh
├── README_AzureIoT_0.9.13.md # 版本說明
├── UPDATE_0.9.13.md # 更新說明
└── SOLUTION_REPORT.md # 問題解決報告

## 使用指南

### 從源代碼編譯
```bash
cd package/azureiotd
make clean
make
```

## 使用二進制文件部署
```bash
cd bin/azureiotd/0.9.13
./deploy.sh install
```

## 配置服務
```bash
# 編輯配置文件
vi /etc/config/azureiotd

# 啟動服務
/etc/init.d/azureiotd start
```

## 驗證狀態

* ✅ 實際板子測試通過
* ✅ QEMU模擬測試通過
* ✅ 文件完整性驗證通過

## 回滾方案
如果新版本有問題：
```bash
# 恢復備份源代碼
cp -r package/azureiotd-0.9.12d-backup/* package/azureiotd/

# 或使用Git回滾
git checkout v0.9.12d
```
