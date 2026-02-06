#include "RSABenchmark.hpp"
#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <cstring>

RSABenchmark::RSABenchmark(int keySize)
    : keySize_(keySize)
    , pkey_(nullptr)
    , signCtx_(nullptr)
    , verifyCtx_(nullptr) {
}

RSABenchmark::~RSABenchmark() {
    cleanup();
}

void RSABenchmark::cleanup() {
    if (signCtx_) {
        EVP_PKEY_CTX_free(signCtx_);
        signCtx_ = nullptr;
    }
    if (verifyCtx_) {
        EVP_PKEY_CTX_free(verifyCtx_);
        verifyCtx_ = nullptr;
    }
    if (pkey_) {
        EVP_PKEY_free(pkey_);
        pkey_ = nullptr;
    }
}

bool RSABenchmark::generateKeys() {
    cleanup();

    // Create RSA key generation context
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (!ctx) {
        return false;
    }

    // Initialize key generation
    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    // Set RSA key size
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, keySize_) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    // Generate the key
    if (EVP_PKEY_keygen(ctx, &pkey_) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }

    EVP_PKEY_CTX_free(ctx);
    return true;
}

std::vector<uint8_t> RSABenchmark::sign(const std::vector<uint8_t>& message) {
    if (!pkey_) {
        return {};
    }

    // Create signing context
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        return {};
    }

    // Initialize signing with SHA-256 and RSA-PSS
    if (EVP_DigestSignInit(mdctx, nullptr, EVP_sha256(), nullptr, pkey_) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return {};
    }

    // Determine signature length
    size_t sigLen = 0;
    if (EVP_DigestSign(mdctx, nullptr, &sigLen, message.data(), message.size()) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return {};
    }

    // Allocate signature buffer
    std::vector<uint8_t> signature(sigLen);

    // Create signature
    if (EVP_DigestSign(mdctx, signature.data(), &sigLen, 
                       message.data(), message.size()) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return {};
    }

    EVP_MD_CTX_free(mdctx);
    signature.resize(sigLen);
    return signature;
}

bool RSABenchmark::verify(const std::vector<uint8_t>& message,
                          const std::vector<uint8_t>& signature) {
    if (!pkey_) {
        return false;
    }

    // Create verification context
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        return false;
    }

    // Initialize verification with SHA-256
    if (EVP_DigestVerifyInit(mdctx, nullptr, EVP_sha256(), nullptr, pkey_) <= 0) {
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    // Verify signature
    int result = EVP_DigestVerify(mdctx, signature.data(), signature.size(),
                                  message.data(), message.size());

    EVP_MD_CTX_free(mdctx);
    return (result == 1);
}

size_t RSABenchmark::getPublicKeySize() const {
    if (!pkey_) {
        return 0;
    }
    
    // Approximate public key size (modulus + exponent)
    return (keySize_ / 8) + 32; // Modulus + small exponent overhead
}

size_t RSABenchmark::getPrivateKeySize() const {
    if (!pkey_) {
        return 0;
    }
    
    // Approximate private key size (multiple large integers in CRT form)
    return (keySize_ / 8) * 5; // Rough estimate for CRT parameters
}

size_t RSABenchmark::getSignatureSize() const {
    return keySize_ / 8; // RSA signature size equals modulus size
}