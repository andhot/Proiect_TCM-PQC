#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <vector>
#include <string>
#include <functional>

/**
 * @brief Utility class for benchmarking cryptographic operations
 */
class Benchmark {
public:
    /**
     * @brief Result structure for benchmark runs
     */
    struct Result {
        double averageTime;     // Average time in milliseconds
        double minTime;         // Minimum time in milliseconds
        double maxTime;         // Maximum time in milliseconds
        double stdDev;          // Standard deviation in milliseconds
        size_t iterations;      // Number of iterations run
    };

    /**
     * @brief Run a benchmark function multiple times
     * @param func The function to benchmark
     * @param iterations Number of times to run
     * @return Benchmark results
     */
    static Result run(std::function<void()> func, size_t iterations = 100);

    /**
     * @brief Generate random message of specified size
     * @param size Message size in bytes
     * @return Random bytes
     */
    static std::vector<uint8_t> generateRandomMessage(size_t size);

    /**
     * @brief Print benchmark results in formatted table
     * @param name Operation name
     * @param result Benchmark result
     */
    static void printResult(const std::string& name, const Result& result);

    /**
     * @brief Print comparison table header
     */
    static void printTableHeader();

    /**
     * @brief Print comparison row
     * @param algorithm Algorithm name
     * @param keySize Key size description
     * @param keyGen Key generation result
     * @param sign Signing result
     * @param verify Verification result
     * @param pubKeyBytes Public key size in bytes
     * @param sigBytes Signature size in bytes
     */
    static void printComparisonRow(
        const std::string& algorithm,
        const std::string& keySize,
        const Result& keyGen,
        const Result& sign,
        const Result& verify,
        size_t pubKeyBytes,
        size_t sigBytes
    );

    /**
     * @brief Print table separator
     */
    static void printSeparator();

private:
    /**
     * @brief Calculate standard deviation
     * @param values Vector of values
     * @param mean Mean value
     * @return Standard deviation
     */
    static double calculateStdDev(const std::vector<double>& values, double mean);
};

#endif // BENCHMARK_HPP