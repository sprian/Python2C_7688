#!/bin/bash
APP="$1"
ROOTFS="/home/ai7688/ai7688/SVN/ChargingPile_G4/trunk/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/root-ramips"

if [ ! -f "$APP" ]; then
    echo "错误: 程序文件 $APP 不存在"
    exit 1
fi

echo "在 QEMU 中调试: $APP"
echo "使用 gdbserver 模式..."

# 使用 gdbserver 模式运行
QEMU_CPU=74Kf QEMU_LD_PREFIX="$ROOTFS" qemu-mipsel -g 1234 "$APP"

echo "GDB 服务器运行在端口 1234"
echo "在另一个终端运行: mipsel-openwrt-linux-gdb $APP"
echo "然后在 gdb 中连接: target remote :1234"
