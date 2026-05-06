#include "cp_stress_gen.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

long long brute_max_subarray(const std::vector<long long>& values) {
    long long best = std::numeric_limits<long long>::min();
    for (std::size_t left = 0; left < values.size(); ++left) {
        long long sum = 0;
        for (std::size_t right = left; right < values.size(); ++right) {
            sum += values[right];
            best = std::max(best, sum);
        }
    }
    return best;
}

long long optimized_max_subarray(const std::vector<long long>& values) {
    long long best = values.front();
    long long current = values.front();
    for (std::size_t i = 1; i < values.size(); ++i) {
        current = std::max(values[i], current + values[i]);
        best = std::max(best, current);
    }
    return best;
}

int main(int argc, char** argv) {
    cp_stress_gen::core::Options opt(argc, argv);
    const int tests = opt.get<int>("tests", 200);
    const int n = opt.get<int>("n", 30);
    const long long seed = opt.get<long long>("seed", 1);
    const bool hard = opt.flag("hard");
    cp_stress_gen::core::require(tests > 0, "tests must be positive");
    cp_stress_gen::core::require(n > 0, "n must be positive");

    cp_stress_gen::core::Random rng(static_cast<unsigned long long>(seed));
    for (int tc = 1; tc <= tests; ++tc) {
        const auto values = hard
            ? cp_stress_gen::Array(static_cast<std::size_t>(n)).many_equal(3).build(rng)
            : cp_stress_gen::Array(static_cast<std::size_t>(n)).range(-100, 100).build(rng);
        const long long brute = brute_max_subarray(values);
        const long long optimized = optimized_max_subarray(values);
        if (brute != optimized) {
            cp_stress_gen::core::Printer out(std::cout);
            out.line("mismatch", tc);
            out.line(n);
            out.vector(values);
            out.line("brute", brute);
            out.line("optimized", optimized);
            return 1;
        }
    }

    std::cout << "[PASS] local_stress_driver\n";
    return 0;
}
