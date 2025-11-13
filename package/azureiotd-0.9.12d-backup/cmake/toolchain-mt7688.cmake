# Toolchain for MT7688 (OpenWrt 15.05.1 / uClibc / mipsel)
set(CMAKE_SYSTEM_NAME Linux)

# Env-provided CC/CXX will be used if present
if(NOT DEFINED CMAKE_C_COMPILER)
  set(CMAKE_C_COMPILER $ENV{CC})
endif()
if(NOT DEFINED CMAKE_CXX_COMPILER)
  set(CMAKE_CXX_COMPILER $ENV{CXX})
endif()

# Sysroot / search paths
set(STAGING_DIR $ENV{STAGING_DIR})
set(TOOLCHAIN_DIR $ENV{TOOLCHAIN_DIR})

set(CMAKE_FIND_ROOT_PATH
    ${STAGING_DIR}
    ${TOOLCHAIN_DIR}
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")

# Prefer these include/lib dirs
include_directories(${STAGING_DIR}/usr/include)
link_directories(${STAGING_DIR}/usr/lib)
