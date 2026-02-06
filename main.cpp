#include "Dilithiumwrapper.hpp"
#include "RSABenchmark.hpp"
#include "Benchmark.hpp"
#include <iostream>
#include <vector>
#include <iomanip>

/**
 * @brief Run comprehensive benchmarks comparing Dilithium with RSA
 */
void runComprehensiveBenchmark() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "  DILITHIUM vs RSA BENCHMARK SUITE\n";
    std::cout << "========================================\n\n";

    const size_t MESSAGE_SIZE = 1024;     // 1 KB message
    const size_t ITERATIONS = 100;        // Number of iterations for sign/verify
    const size_t KEYGEN_ITERATIONS = 10;  // Fewer iterations for slow key generation

    std::cout << "Configuration:\n";
    std::cout << "  Message size: " << MESSAGE_SIZE << " bytes\n";
    std::cout << "  Sign/Verify iterations: " << ITERATIONS << "\n";
    std::cout << "  KeyGen iterations: " << KEYGEN_ITERATIONS << "\n\n";

    // Generate test message
    auto message = Benchmark::generateRandomMessage(MESSAGE_SIZE);

    // ==================== DILITHIUM3 BENCHMARK ====================
    std::cout << "Testing CRYSTALS-Dilithium3 (NIST Level 3)...\n";
    
    DilithiumWrapper dilithium;
    std::vector<uint8_t> dilithiumSig;

    // Benchmark key generation
    auto dilithiumKeyGen = Benchmark::run([&]() {
        dilithium.generateKeys();
    }, KEYGEN_ITERATIONS);

    // Generate keys once for signing/verification
    dilithium.generateKeys();

    // Benchmark signing
    auto dilithiumSign = Benchmark::run([&]() {
        dilithiumSig = dilithium.sign(message);
    }, ITERATIONS);

    // Sign once for verification benchmark
    dilithiumSig = dilithium.sign(message);

    // Benchmark verification
    auto dilithiumVerify = Benchmark::run([&]() {
        dilithium.verify(message, dilithiumSig);
    }, ITERATIONS);

    std::cout << "Done!\n\n";

    // ==================== RSA-2048 BENCHMARK ====================
    std::cout << "Testing RSA-2048 (Traditional)...\n";
    
    RSABenchmark rsa2048(2048);
    std::vector<uint8_t> rsa2048Sig;

    auto rsa2048KeyGen = Benchmark::run([&]() {
        rsa2048.generateKeys();
    }, KEYGEN_ITERATIONS);

    rsa2048.generateKeys();

    auto rsa2048Sign = Benchmark::run([&]() {
        rsa2048Sig = rsa2048.sign(message);
    }, ITERATIONS);

    rsa2048Sig = rsa2048.sign(message);

    auto rsa2048Verify = Benchmark::run([&]() {
        rsa2048.verify(message, rsa2048Sig);
    }, ITERATIONS);

    std::cout << "Done!\n\n";

    // ==================== RSA-3072 BENCHMARK ====================
    std::cout << "Testing RSA-3072 (128-bit security)...\n";
    
    RSABenchmark rsa3072(3072);
    std::vector<uint8_t> rsa3072Sig;

    auto rsa3072KeyGen = Benchmark::run([&]() {
        rsa3072.generateKeys();
    }, KEYGEN_ITERATIONS);

    rsa3072.generateKeys();

    auto rsa3072Sign = Benchmark::run([&]() {
        rsa3072Sig = rsa3072.sign(message);
    }, ITERATIONS);

    rsa3072Sig = rsa3072.sign(message);

    auto rsa3072Verify = Benchmark::run([&]() {
        rsa3072.verify(message, rsa3072Sig);
    }, ITERATIONS);

    std::cout << "Done!\n\n";

    // ==================== RESULTS COMPARISON ====================
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "         PERFORMANCE COMPARISON\n";
    std::cout << "========================================\n\n";

    Benchmark::printTableHeader();

    Benchmark::printComparisonRow(
        "Dilithium3",
        "NIST Level 3",
        dilithiumKeyGen,
        dilithiumSign,
        dilithiumVerify,
        DilithiumWrapper::PUBLIC_KEY_BYTES,
        dilithiumSig.size()
    );

    Benchmark::printComparisonRow(
        "RSA-2048",
        "112-bit",
        rsa2048KeyGen,
        rsa2048Sign,
        rsa2048Verify,
        rsa2048.getPublicKeySize(),
        rsa2048Sig.size()
    );

    Benchmark::printComparisonRow(
        "RSA-3072",
        "128-bit",
        rsa3072KeyGen,
        rsa3072Sign,
        rsa3072Verify,
        rsa3072.getPublicKeySize(),
        rsa3072Sig.size()
    );

    Benchmark::printSeparator();

    // ==================== DETAILED ANALYSIS ====================
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "          DETAILED ANALYSIS\n";
    std::cout << "========================================\n\n";

    // Speed comparison
    std::cout << "Speed Comparison (vs Dilithium3):\n";
    std::cout << "  RSA-2048 KeyGen: " 
              << std::fixed << std::setprecision(2)
              << (rsa2048KeyGen.averageTime / dilithiumKeyGen.averageTime) 
              << "x slower\n";
    std::cout << "  RSA-3072 KeyGen: " 
              << (rsa3072KeyGen.averageTime / dilithiumKeyGen.averageTime) 
              << "x slower\n\n";

    std::cout << "  RSA-2048 Signing: " 
              << (rsa2048Sign.averageTime / dilithiumSign.averageTime) 
              << "x " << (rsa2048Sign.averageTime > dilithiumSign.averageTime ? "slower" : "faster") << "\n";
    std::cout << "  RSA-3072 Signing: " 
              << (rsa3072Sign.averageTime / dilithiumSign.averageTime) 
              << "x " << (rsa3072Sign.averageTime > dilithiumSign.averageTime ? "slower" : "faster") << "\n\n";

    std::cout << "  RSA-2048 Verification: " 
              << (rsa2048Verify.averageTime / dilithiumVerify.averageTime) 
              << "x " << (rsa2048Verify.averageTime > dilithiumVerify.averageTime ? "slower" : "faster") << "\n";
    std::cout << "  RSA-3072 Verification: " 
              << (rsa3072Verify.averageTime / dilithiumVerify.averageTime) 
              << "x " << (rsa3072Verify.averageTime > dilithiumVerify.averageTime ? "slower" : "faster") << "\n\n";

    // Size comparison
    std::cout << "Size Comparison:\n";
    std::cout << "  Dilithium3 Public Key: " << DilithiumWrapper::PUBLIC_KEY_BYTES << " bytes\n";
    std::cout << "  RSA-2048 Public Key:   " << rsa2048.getPublicKeySize() << " bytes\n";
    std::cout << "  RSA-3072 Public Key:   " << rsa3072.getPublicKeySize() << " bytes\n\n";

    std::cout << "  Dilithium3 Signature:  " << dilithiumSig.size() << " bytes\n";
    std::cout << "  RSA-2048 Signature:    " << rsa2048Sig.size() << " bytes\n";
    std::cout << "  RSA-3072 Signature:    " << rsa3072Sig.size() << " bytes\n\n";

    // Security analysis
    std::cout << "Security Level:\n";
    std::cout << "  Dilithium3: NIST Level 3 (~128-bit quantum security)\n";
    std::cout << "  RSA-2048:   112-bit classical security (broken by quantum)\n";
    std::cout << "  RSA-3072:   128-bit classical security (broken by quantum)\n\n";

    std::cout << "Post-Quantum Security:\n";
    std::cout << "  Dilithium3: ✓ Quantum-resistant (lattice-based)\n";
    std::cout << "  RSA-2048:   ✗ Vulnerable to Shor's algorithm\n";
    std::cout << "  RSA-3072:   ✗ Vulnerable to Shor's algorithm\n\n";
}

/**
 * @brief Demonstrate basic Dilithium usage
 */
void demonstrateBasicUsage() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "     DILITHIUM BASIC USAGE DEMO\n";
    std::cout << "========================================\n\n";

    // Create Dilithium instance
    DilithiumWrapper dilithium;

    // Generate keys
    std::cout << "1. Generating Dilithium key pair...\n";
    if (!dilithium.generateKeys()) {
        std::cerr << "   Failed to generate keys!\n";
        return;
    }
    std::cout << "   ✓ Key generation successful\n\n";

    // Create a message
    std::string messageStr = "Hello, Post-Quantum World!";
    std::vector<uint8_t> message(messageStr.begin(), messageStr.end());
    std::cout << "2. Message to sign: \"" << messageStr << "\"\n\n";

    // Sign the message
    std::cout << "3. Signing message...\n";
    auto signature = dilithium.sign(message);
    if (signature.empty()) {
        std::cerr << "   Failed to sign message!\n";
        return;
    }
    std::cout << "   ✓ Signature created (" << signature.size() << " bytes)\n\n";

    // Verify the signature
    std::cout << "4. Verifying signature...\n";
    bool valid = dilithium.verify(message, signature);
    std::cout << "   " << (valid ? "✓" : "✗") << " Signature is " 
              << (valid ? "VALID" : "INVALID") << "\n\n";

    // Test with tampered message
    std::cout << "5. Testing with tampered message...\n";
    message[0] ^= 0x01; // Flip one bit
    bool validTampered = dilithium.verify(message, signature);
    std::cout << "   " << (validTampered ? "✗" : "✓") << " Tampered signature is " 
              << (validTampered ? "VALID (ERROR!)" : "INVALID (CORRECT!)") << "\n\n";
}

int main() {
    try {
        // Demonstrate basic usage
        demonstrateBasicUsage();

        // Run comprehensive benchmarks
        runComprehensiveBenchmark();

        std::cout << "========================================\n";
        std::cout << "      BENCHMARK COMPLETED SUCCESSFULLY\n";
        std::cout << "========================================\n\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}