/**
 * @file DilithiumWrapper.cpp
 * @brief Implementation of C++ wrapper for CRYSTALS-Dilithium signature scheme
 * 
 * This implementation wraps the pq-crystals reference implementation of Dilithium.
 * The reference implementation is written in C and provides functions for:
 * - Key generation: pqcrystals_dilithium3_ref_keypair()
 * - Signing: pqcrystals_dilithium3_ref_signature()
 * - Verification: pqcrystals_dilithium3_ref_verify()
 */

#include "Dilithiumwrapper.hpp"
#include <cstring>
#include <stdexcept>

// Include Dilithium reference implementation header
// extern "C" is required because the reference implementation is in C
extern "C" {
#include "api.h"
}

/**
 * @brief Constructor - initializes key storage buffers
 * 
 * Allocates memory for public and secret keys with appropriate sizes
 * for Dilithium3 security level.
 */
DilithiumWrapper::DilithiumWrapper() 
    : publicKey_(PUBLIC_KEY_BYTES, 0)
    , secretKey_(SECRET_KEY_BYTES, 0)
    , keysGenerated_(false) {
}

/**
 * @brief Destructor - ensures secure cleanup of cryptographic material
 * 
 * Uses volatile pointer to prevent compiler optimization from
 * removing the memory wiping operation.
 */
DilithiumWrapper::~DilithiumWrapper() {
    // Securely wipe secret key from memory to prevent key extraction
    secureWipe(secretKey_.data(), secretKey_.size());
}

/**
 * @brief Generate Dilithium key pair
 * 
 * Key Generation Algorithm:
 * 1. Sample random seed ρ ∈ {0,1}^256 and K ∈ {0,1}^256
 * 2. Expand matrix A ∈ R_q^{k×l} from ρ using SHAKE-128
 * 3. Sample secret vectors s1 ∈ S_η^l and s2 ∈ S_η^k
 * 4. Compute t = As1 + s2
 * 5. Split t into (t1, t0) where t1 is the high bits
 * 6. pk = (ρ, t1), sk = (ρ, K, tr, s1, s2, t0)
 * 
 * @return true if key generation successful, false otherwise
 */
bool DilithiumWrapper::generateKeys() {
    try {
        // Call Dilithium reference implementation keypair function
        // This generates both public and secret keys atomically
        int result = pqcrystals_dilithium3_ref_keypair(
            publicKey_.data(),
            secretKey_.data()
        );

        if (result != 0) {
            return false;
        }

        keysGenerated_ = true;
        return true;
    } catch (...) {
        return false;
    }
}

/**
 * @brief Sign a message using Dilithium
 * 
 * Signing Algorithm (Fiat-Shamir with Aborts):
 * 1. Compute μ = H(tr || M) where tr is public key hash
 * 2. Sample mask vector y uniformly from S_{γ1-1}^l
 * 3. Compute w = Ay using NTT multiplication
 * 4. Compute commitment w1 = HighBits(w)
 * 5. Compute challenge c = H(μ || w1) ∈ B_τ (sparse polynomial)
 * 6. Compute response z = y + c·s1
 * 7. Check ||z||∞ < γ1 - β and ||LowBits(w - cs2)||∞ < γ2 - β
 * 8. If checks fail, restart from step 2 (rejection sampling)
 * 9. Compute hint h for verification
 * 10. Return σ = (c̃, z, h)
 * 
 * @param message The message to sign
 * @return Signature vector, or empty vector on failure
 */
std::vector<uint8_t> DilithiumWrapper::sign(const std::vector<uint8_t>& message) {
    if (!keysGenerated_) {
        return {};
    }

    try {
        std::vector<uint8_t> signature(SIGNATURE_BYTES);
        size_t signatureLength = 0;

        // Call Dilithium signing function (new API with ctx parameter)
        // ctx is an optional context string, we use nullptr/0 for no context
        int result = pqcrystals_dilithium3_ref_signature(
            signature.data(),
            &signatureLength,
            message.data(),
            message.size(),
            nullptr,  // ctx - context string (optional)
            0,        // ctxlen - context length
            secretKey_.data()
        );

        if (result != 0) {
            return {};
        }

        // Resize to actual signature length
        signature.resize(signatureLength);
        return signature;
    } catch (...) {
        return {};
    }
}

/**
 * @brief Verify a Dilithium signature
 * 
 * Verification Algorithm:
 * 1. Parse σ = (c̃, z, h) from signature
 * 2. Check ||z||∞ < γ1 - β
 * 3. Expand matrix A from ρ (from public key)
 * 4. Compute μ = H(tr || M)
 * 5. Reconstruct c from c̃
 * 6. Compute w'1 = UseHint(h, Az - ct1·2^d)
 * 7. Accept iff c = H(μ || w'1)
 * 
 * @param message The original message
 * @param signature The signature to verify
 * @return true if signature is valid, false otherwise
 */
bool DilithiumWrapper::verify(const std::vector<uint8_t>& message,
                              const std::vector<uint8_t>& signature) {
    if (!keysGenerated_) {
        return false;
    }

    try {
        // Call Dilithium verification function (new API with ctx parameter)
        int result = pqcrystals_dilithium3_ref_verify(
            signature.data(),
            signature.size(),
            message.data(),
            message.size(),
            nullptr,  // ctx - context string (optional)
            0,        // ctxlen - context length
            publicKey_.data()
        );

        return (result == 0);
    } catch (...) {
        return false;
    }
}

std::vector<uint8_t> DilithiumWrapper::getPublicKey() const {
    return publicKey_;
}

std::vector<uint8_t> DilithiumWrapper::getSecretKey() const {
    return secretKey_;
}

bool DilithiumWrapper::setPublicKey(const std::vector<uint8_t>& pubkey) {
    if (pubkey.size() != PUBLIC_KEY_BYTES) {
        return false;
    }
    publicKey_ = pubkey;
    return true;
}

bool DilithiumWrapper::setSecretKey(const std::vector<uint8_t>& seckey) {
    if (seckey.size() != SECRET_KEY_BYTES) {
        return false;
    }
    secretKey_ = seckey;
    keysGenerated_ = true;
    return true;
}

void DilithiumWrapper::secureWipe(void* data, size_t size) {
    if (data && size > 0) {
        volatile uint8_t* p = static_cast<volatile uint8_t*>(data);
        for (size_t i = 0; i < size; ++i) {
            p[i] = 0;
        }
    }
}