/**
 * @file DilithiumWrapper.hpp
 * @brief C++ wrapper for CRYSTALS-Dilithium post-quantum digital signature scheme
 * 
 * CRYSTALS-Dilithium is a lattice-based digital signature scheme selected by NIST
 * for post-quantum cryptography standardization (FIPS 204). This wrapper provides
 * a clean C++ interface for the reference implementation.
 * 
 * Algorithm Overview:
 * - Security basis: Module-Learning With Errors (MLWE) and Module-Short Integer 
 *   Solution (MSIS) problems
 * - Signature technique: Fiat-Shamir with Aborts paradigm
 * - This implementation uses Dilithium3 (NIST Security Level 3)
 * 
 * Mathematical Foundation:
 * - Works in polynomial ring R_q = Z_q[X]/(X^256 + 1) where q = 8380417
 * - Uses rejection sampling to ensure signature security
 * - Number-Theoretic Transform (NTT) for efficient polynomial multiplication
 * 
 * Security Level (Dilithium3):
 * - Equivalent to ~128-bit security against quantum adversaries
 * - Parameters: k=6, l=5, η=4, β=196, ω=55
 * 
 * @author Student Project - Cryptographic Systems Course
 * @date December 2025
 */

#ifndef DILITHIUM_WRAPPER_HPP
#define DILITHIUM_WRAPPER_HPP

#include <vector>
#include <string>
#include <cstdint>

/**
 * @brief C++ wrapper for CRYSTALS-Dilithium post-quantum signature scheme
 * 
 * This class wraps the reference implementation of Dilithium3 (NIST security level 3)
 * providing a clean C++ interface for key generation, signing, and verification.
 */
class DilithiumWrapper {
public:
    // Dilithium3 parameters (from api.h)
    // These values are determined by the security level and mathematical structure
    static constexpr size_t PUBLIC_KEY_BYTES = 1952;   // pk = (ρ, t1) encoded
    static constexpr size_t SECRET_KEY_BYTES = 4032;   // sk = (ρ, K, tr, s1, s2, t0)
    static constexpr size_t SIGNATURE_BYTES = 3309;    // σ = (c̃, z, h) encoded

    /**
     * @brief Constructor - initializes empty key pair
     */
    DilithiumWrapper();

    /**
     * @brief Destructor - securely wipes secret key material
     */
    ~DilithiumWrapper();

    /**
     * @brief Generate a new Dilithium key pair
     * @return true if successful, false otherwise
     */
    bool generateKeys();

    /**
     * @brief Sign a message with the secret key
     * @param message The message to sign
     * @return The signature bytes, or empty vector on failure
     */
    std::vector<uint8_t> sign(const std::vector<uint8_t>& message);

    /**
     * @brief Verify a signature with the public key
     * @param message The original message
     * @param signature The signature to verify
     * @return true if signature is valid, false otherwise
     */
    bool verify(const std::vector<uint8_t>& message, 
                const std::vector<uint8_t>& signature);

    /**
     * @brief Get the public key
     * @return Public key bytes
     */
    std::vector<uint8_t> getPublicKey() const;

    /**
     * @brief Get the secret key (use with caution!)
     * @return Secret key bytes
     */
    std::vector<uint8_t> getSecretKey() const;

    /**
     * @brief Set public key from bytes
     * @param pubkey Public key bytes
     * @return true if successful
     */
    bool setPublicKey(const std::vector<uint8_t>& pubkey);

    /**
     * @brief Set secret key from bytes
     * @param seckey Secret key bytes
     * @return true if successful
     */
    bool setSecretKey(const std::vector<uint8_t>& seckey);

    /**
     * @brief Check if keys have been generated
     * @return true if keys exist
     */
    bool hasKeys() const { return keysGenerated_; }

private:
    std::vector<uint8_t> publicKey_;
    std::vector<uint8_t> secretKey_;
    bool keysGenerated_;

    /**
     * @brief Securely wipe memory
     * @param data Pointer to data
     * @param size Size of data in bytes
     */
    void secureWipe(void* data, size_t size);
};

#endif // DILITHIUM_WRAPPER_HPP