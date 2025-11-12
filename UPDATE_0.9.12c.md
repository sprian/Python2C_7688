# Azure IoT Daemon 更新至 v0.9.12c

## 更新内容

### 主要改进
1. **版本升级**: 0.9.11n → 0.9.12c
2. **新增 safe_str 模块**: 提供安全的字符串处理函数
3. **代码优化**: 从 321 行增加到 342 行代码
4. **更好的错误处理**: 改进的诊断和错误处理机制

### 新增文件
- `src/safe_str.c` - 安全字符串处理实现
- `src/safe_str.h` - 安全字符串处理头文件

### 修改文件
- `src/azureiotd.c` - 增加版本信息和改进错误处理
- `src/azure_conn.c` - 优化连接逻辑和错误处理
- `Makefile` - 更新版本号到 0.9.12c

### 测试验证
- ✅ 在 AI7688 OpenWrt 15.05.1 环境编译通过
- ✅ 生成 IPK 包: azureiotd_0.9.12c-1_ramips_24kec.ipk
- ✅ 在 QEMU 模拟器中运行测试通过

### 依赖更新
- 新增可选组件: libevi-azure, evi-azurectl
- 保持向后兼容性
