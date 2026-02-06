# CRYSTALS-Dilithium Post-Quantum Digital Signature Implementation

## Project Overview

This project implements **Tema 12: Semnatura digitala PQC Dilithium** for the cryptographic systems course. It provides a C++ wrapper around the reference implementation of CRYSTALS-Dilithium, a lattice-based post-quantum digital signature scheme standardized by NIST.

## Algorithm Description

### CRYSTALS-Dilithium

Dilithium is a digital signature scheme based on the hardness of lattice problems, specifically the Module Learning With Errors (MLWE) problem. It was selected by NIST as the primary digital signature algorithm for post-quantum cryptography standardization (FIPS 204).

**Key Features:**
- **Security basis:** Module-LWE and Module-SIS problems
- **Signature type:** Fiat-Shamir with Aborts
- **Security levels:** Dilithium2 (NIST Level 2), Dilithium3 (Level 3), Dilithium5 (Level 5)

### Security Parameters (Dilithium3 - NIST Level 3)

| Parameter | Value |
|-----------|-------|
| Public Key Size | 1952 bytes |
| Secret Key Size | 4032 bytes |
| Signature Size | 3309 bytes |
| Security Level | ~128-bit quantum |

## Project Structure

```
tcm_proj/
├── CMakeLists.txt          # Build configuration
├── main.cpp                # Main benchmark program
├── Dilithiumwrapper.hpp    # C++ wrapper header
├── Dilithiumwrapper.cpp    # C++ wrapper implementation
├── RSABenchmark.hpp        # RSA benchmark header
├── RSABenchmark.cpp        # RSA benchmark implementation
├── Benchmark.hpp           # Benchmark utilities header
├── Benchmark.cpp           # Benchmark utilities implementation
├── README.md               # This file
└── dilithium/              # Reference implementation (pq-crystals)
    └── ref/                # Reference C implementation
```

## Dependencies

- **CMake** >= 3.15
- **OpenSSL** >= 1.1.1 (for RSA comparison)
- **C++17** compatible compiler (GCC, Clang)
- **C99** compiler for Dilithium reference implementation

## Building

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Running

```bash
./dilithium_benchmark
```

## API Usage

```cpp
#include "Dilithiumwrapper.hpp"

// Create instance
DilithiumWrapper dilithium;

// Generate key pair
dilithium.generateKeys();

// Sign a message
std::vector<uint8_t> message = {0x48, 0x65, 0x6c, 0x6c, 0x6f}; // "Hello"
auto signature = dilithium.sign(message);

// Verify signature
bool valid = dilithium.verify(message, signature);
```

## Benchmark Results

Comparison between Dilithium3 and RSA on typical hardware:

| Algorithm | Security | KeyGen (ms) | Sign (ms) | Verify (ms) | PubKey | Signature |
|-----------|----------|-------------|-----------|-------------|--------|-----------|
| Dilithium3 | NIST L3 | ~0.09 | ~0.37 | ~0.09 | 1952 B | 3309 B |
| RSA-2048 | 112-bit | ~80 | ~0.19 | ~0.01 | 288 B | 256 B |
| RSA-3072 | 128-bit | ~280 | ~0.53 | ~0.03 | 416 B | 384 B |

**Key observations:**
1. Dilithium key generation is **100-3000x faster** than RSA
2. Dilithium signing is competitive with RSA
3. RSA verification is faster due to small public exponent
4. Dilithium has larger keys and signatures (trade-off for quantum resistance)

## Security Comparison

| Aspect | Dilithium | RSA |
|--------|-----------|-----|
| Classical Security | ✓ | ✓ |
| Quantum Security | ✓ (Lattice-based) | ✗ (Shor's algorithm) |
| Mathematical Basis | MLWE/MSIS | Integer Factorization |
| Standardization | NIST FIPS 204 | PKCS#1, FIPS 186 |

## References

1. CRYSTALS-Dilithium: https://pq-crystals.org/dilithium/
2. NIST PQC Project: https://csrc.nist.gov/projects/post-quantum-cryptography
3. Dilithium Paper: Ducas et al., "CRYSTALS-Dilithium: A Lattice-Based Digital Signature Scheme"

## License

Educational use only. Dilithium reference implementation is public domain.
