#include "Benchmark.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <random>

Benchmark::Result Benchmark::run(std::function<void()> func, size_t iterations) {
    std::vector<double> times;
    times.reserve(iterations);

    for (size_t i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        times.push_back(duration.count() / 1000.0); // Convert to milliseconds
    }

    // Calculate statistics
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double mean = sum / times.size();
    double minTime = *std::min_element(times.begin(), times.end());
    double maxTime = *std::max_element(times.begin(), times.end());
    double stdDev = calculateStdDev(times, mean);

    return {mean, minTime, maxTime, stdDev, iterations};
}

std::vector<uint8_t> Benchmark::generateRandomMessage(size_t size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    std::vector<uint8_t> message(size);
    for (size_t i = 0; i < size; ++i) {
        message[i] = static_cast<uint8_t>(dis(gen));
    }
    return message;
}

void Benchmark::printResult(const std::string& name, const Result& result) {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << name << ":\n";
    std::cout << "  Average: " << result.averageTime << " ms\n";
    std::cout << "  Min:     " << result.minTime << " ms\n";
    std::cout << "  Max:     " << result.maxTime << " ms\n";
    std::cout << "  StdDev:  " << result.stdDev << " ms\n";
    std::cout << "  Iterations: " << result.iterations << "\n\n";
}

void Benchmark::printTableHeader() {
    printSeparator();
    std::cout << "| " << std::setw(15) << std::left << "Algorithm"
              << " | " << std::setw(12) << "Security"
              << " | " << std::setw(12) << "KeyGen (ms)"
              << " | " << std::setw(12) << "Sign (ms)"
              << " | " << std::setw(12) << "Verify (ms)"
              << " | " << std::setw(12) << "PubKey (B)"
              << " | " << std::setw(12) << "Sig (B)"
              << " |\n";
    printSeparator();
}

void Benchmark::printComparisonRow(
    const std::string& algorithm,
    const std::string& keySize,
    const Result& keyGen,
    const Result& sign,
    const Result& verify,
    size_t pubKeyBytes,
    size_t sigBytes) {
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "| " << std::setw(15) << std::left << algorithm
              << " | " << std::setw(12) << keySize
              << " | " << std::setw(12) << keyGen.averageTime
              << " | " << std::setw(12) << sign.averageTime
              << " | " << std::setw(12) << verify.averageTime
              << " | " << std::setw(12) << pubKeyBytes
              << " | " << std::setw(12) << sigBytes
              << " |\n";
}

void Benchmark::printSeparator() {
    std::cout << "+" << std::string(17, '-')
              << "+" << std::string(14, '-')
              << "+" << std::string(14, '-')
              << "+" << std::string(14, '-')
              << "+" << std::string(14, '-')
              << "+" << std::string(14, '-')
              << "+" << std::string(14, '-')
              << "+\n";
}

double Benchmark::calculateStdDev(const std::vector<double>& values, double mean) {
    double sumSquaredDiff = 0.0;
    for (double value : values) {
        double diff = value - mean;
        sumSquaredDiff += diff * diff;
    }
    return std::sqrt(sumSquaredDiff / values.size());
}