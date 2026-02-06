cmake_minimum_required(VERSION 3.15)
project(DilithiumBenchmark CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)

# Add OpenSSL for RSA comparison
find_package(OpenSSL REQUIRED)

# Dilithium reference implementation sources
set(DILITHIUM_DIR ${CMAKE_SOURCE_DIR}/dilithium/ref)

# Dilithium source files (Dilithium3 security level)
set(DILITHIUM_SOURCES
    ${DILITHIUM_DIR}/sign.c
    ${DILITHIUM_DIR}/packing.c
    ${DILITHIUM_DIR}/polyvec.c
    ${DILITHIUM_DIR}/poly.c
    ${DILITHIUM_DIR}/ntt.c
    ${DILITHIUM_DIR}/reduce.c
    ${DILITHIUM_DIR}/rounding.c
    ${DILITHIUM_DIR}/symmetric-shake.c
    ${DILITHIUM_DIR}/fips202.c
    ${DILITHIUM_DIR}/randombytes.c
)

# Include directories
include_directories(
    ${DILITHIUM_DIR}
    ${CMAKE_SOURCE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)

# Create Dilithium library with Dilithium3 mode
add_library(dilithium STATIC ${DILITHIUM_SOURCES})
target_compile_definitions(dilithium PRIVATE DILITHIUM_MODE=3)

# Main executable - source files are in root directory
add_executable(dilithium_benchmark
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/Dilithiumwrapper.cpp
    ${CMAKE_SOURCE_DIR}/RSABenchmark.cpp
    ${CMAKE_SOURCE_DIR}/Benchmark.cpp
)

# Also set DILITHIUM_MODE for the wrapper
target_compile_definitions(dilithium_benchmark PRIVATE DILITHIUM_MODE=3)

target_link_libraries(dilithium_benchmark
    dilithium
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(dilithium_benchmark PRIVATE -Wall -Wextra -O3)
    target_compile_options(dilithium PRIVATE -O3)
endif()