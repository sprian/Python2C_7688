#!/bin/bash
# Build Azure IoT C SDK (LTS_03_2025) for MT7688 (OpenWrt 15.05.1 / uClibc / mipsel)
# Usage:
#   ./scripts/build_azure_sdk_mt7688.sh <AZURE_IOT_SDK_ROOT> [--http on|off] [--dps on|off] [--clean] [--triplet mipsel-openwrt-linux]
# Defaults: --http on  --dps on
set -euo pipefail

AZ_SDK_ROOT="${1:-}"
shift || true

HTTP=on
DPS=on
TRIPLET_DEFAULT="/home/ai7688/ai7688/SVN/ChargingPile_G4/trunk/staging_dir/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux"
TRIPLET="${TRIPLET_DEFAULT}-uclibc"
CLEAN=0

while [[ $# -gt 0 ]]; do
  case "$1" in
    --http) HTTP="${2:-on}"; shift 2;;
    --dps) DPS="${2:-on}"; shift 2;;
    --triplet) TRIPLET="${2:-$TRIPLET_DEFAULT}"; shift 2;;
    --clean) CLEAN=1; shift;;
    *) echo "Unknown arg: $1"; exit 2;;
  esac
done

if [[ -z "${AZ_SDK_ROOT}" || ! -d "${AZ_SDK_ROOT}" ]]; then
  echo "Error: please specify the azure-iot-sdk-c root path."
  exit 1
fi

# ---- EDIT these if your SDK path不同 ----
export STAGING_DIR="${STAGING_DIR:-/home/ai7688/ai7688/SVN/ChargingPile_G4/trunk/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2}"
export TOOLCHAIN_DIR="${TOOLCHAIN_DIR:-/home/ai7688/ai7688/SVN/ChargingPile_G4/trunk/staging_dir/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2}"

export PATH="${TOOLCHAIN_DIR}/bin:${PATH}"

# Try to locate compiler by triplet
CC_CANDIDATE="${TRIPLET}-gcc"
CXX_CANDIDATE="${TRIPLET}-g++"
if ! command -v "${CC_CANDIDATE}" >/dev/null 2>&1; then
  echo "Warning: ${CC_CANDIDATE} not found in PATH. Trying to auto-detect..."
  CC_CANDIDATE="$(ls -1 ${TOOLCHAIN_DIR}/bin/*-gcc 2>/dev/null | grep mipsel | head -n1 || true)"
  CXX_CANDIDATE="$(echo "${CC_CANDIDATE}" | sed 's/gcc$/g++/')"
fi
if [[ -z "${CC_CANDIDATE}" || ! -x "${CC_CANDIDATE}" ]]; then
  echo "Error: could not find cross compiler (mipsel gcc). Check TOOLCHAIN_DIR or --triplet."
  exit 1
fi

export CC="${CC_CANDIDATE}"
export CXX="${CXX_CANDIDATE}"

BUILD_DIR="${AZ_SDK_ROOT}/build-mt7688"
if [[ ${CLEAN} -eq 1 ]]; then
  rm -rf "${BUILD_DIR}"
fi
mkdir -p "${BUILD_DIR}"

# CMake options
USE_HTTP_FLAG="-Duse_http=ON"
[[ "${HTTP}" == "off" ]] && USE_HTTP_FLAG="-Duse_http=OFF"

USE_PROV_FLAG="-Duse_prov_client=ON"
[[ "${DPS}" == "off" ]] && USE_PROV_FLAG="-Duse_prov_client=OFF"

# Verify libs if HTTP is on
if [[ "${HTTP}" == "on" ]]; then
  if [[ ! -f "${STAGING_DIR}/usr/lib/libcurl.so" && ! -f "${STAGING_DIR}/usr/lib/libcurl.a" ]]; then
    echo "Warning: libcurl not found in STAGING_DIR/usr/lib; HTTP build may fail at link. You can rerun with --http off"
  fi
fi

TOOLCHAIN_FILE="${AZ_SDK_ROOT}/toolchain-mt7688.cmake"
# Copy provided toolchain template nearby
cp -f "$(pwd)/cmake/toolchain-mt7688.cmake" "${TOOLCHAIN_FILE}"

cd "${BUILD_DIR}"

/home/ai7688/ai7688/py2c/azureProcess/cmake-3.22/bin/cmake ..   -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_FILE}"   -DCMAKE_BUILD_TYPE=Release   -Dwarnings_as_errors=OFF   -Drun_unittests=OFF -Drun_e2e_tests=OFF -Drun_longhaul_tests=OFF -Dskip_samples=ON   -Duse_mqtt=ON ${USE_HTTP_FLAG} -Duse_amqp=OFF   ${USE_PROV_FLAG}   -Dhsm_type_x509=ON -Dhsm_type_sastoken=ON -Dhsm_type_symm_key=ON   -DBUILD_SHARED_LIBS=OFF   -Duse_installed_dependencies=OFF

make -j"$(nproc 2>/dev/null || echo 2)"

echo
echo "=============================================="
echo "Build done. Your AZSDK_DIR is:"
echo "  ${BUILD_DIR}"
echo "Place this into azureiotd menuconfig -> External Azure IoT C SDK lib dir"
echo "=============================================="
