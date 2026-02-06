#ifndef RSA_BENCHMARK_HPP
#define RSA_BENCHMARK_HPP

#include <vector>
#include <cstdint>
#include <openssl/rsa.h>
#include <openssl/evp.h>

/**
 * @brief Wrapper for RSA operations using OpenSSL
 * 
 * Provides RSA key generation, signing, and verification
 * for comparison with Dilithium
 */
class RSABenchmark {
public:
    /**
     * @brief Constructor
     * @param keySize RSA key size in bits (2048, 3072, or 4096)
     */
    explicit RSABenchmark(int keySize = 2048);

    /**
     * @brief Destructor - cleans up OpenSSL resources
     */
    ~RSABenchmark();

    /**
     * @brief Generate RSA key pair
     * @return true if successful
     */
    bool generateKeys();

    /**
     * @brief Sign a message with RSA-PSS
     * @param message The message to sign
     * @return Signature bytes, or empty on failure
     */
    std::vector<uint8_t> sign(const std::vector<uint8_t>& message);

    /**
     * @brief Verify an RSA-PSS signature
     * @param message The original message
     * @param signature The signature to verify
     * @return true if valid
     */
    bool verify(const std::vector<uint8_t>& message,
                const std::vector<uint8_t>& signature);

    /**
     * @brief Get public key size in bytes
     */
    size_t getPublicKeySize() const;

    /**
     * @brief Get private key size in bytes
     */
    size_t getPrivateKeySize() const;

    /**
     * @brief Get signature size in bytes
     */
    size_t getSignatureSize() const;

    /**
     * @brief Check if keys are generated
     */
    bool hasKeys() const { return pkey_ != nullptr; }

private:
    int keySize_;
    EVP_PKEY* pkey_;
    EVP_PKEY_CTX* signCtx_;
    EVP_PKEY_CTX* verifyCtx_;

    void cleanup();
};

#endif // RSA_BENCHMARK_HPP