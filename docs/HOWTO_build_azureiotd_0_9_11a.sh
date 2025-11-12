#!/bin/sh
set -e
make package/azure-iot-sdk-c/{clean,compile} V=s || true
make package/azureiotd/{clean,compile} V=s
uci set azure.device.dm_timeout='30'
uci set azure.device.dm_max='32'
uci commit azure
/etc/init.d/azureiotd restart || true
