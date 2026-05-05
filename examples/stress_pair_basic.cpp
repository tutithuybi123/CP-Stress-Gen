#include "cp_stress_gen.hpp"

#include <iostream>
#include <numeric>
#include <vector>

long long brute_sum(const std::vector<long long>& values) {
    long long answer = 0;
    for (std::size_t i = 0; i < values.size(); ++i) {
        answer += values[i];
    }
    return answer;
}

long long optimized_sum(const std::vector<long long>& values) {
    return std::accumulate(values.begin(), values.end(), 0LL);
}

int main() {
    cp_stress_gen::core::Random rng(2026);
    cp_stress_gen::core::Printer out(std::cout);

    for (int tc = 1; tc <= 100; ++tc) {
        const auto values = cp_stress_gen::Array(20).range(-100, 100).build(rng);
        const long long brute = brute_sum(values);
        const long long optimized = optimized_sum(values);

        if (brute != optimized) {
            out.line("mismatch", tc);
            out.line(values.size());
            out.vector(values);
            out.line("brute", brute);
            out.line("optimized", optimized);
            return 1;
        }
    }

    out.line("all tests matched");
    return 0;
}

