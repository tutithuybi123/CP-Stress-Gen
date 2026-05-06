#include "cp_stress_gen.hpp"

#include <chrono>
#include <iostream>

int main(int argc, char** argv) {
    cp_stress_gen::core::Options opt(argc, argv);
    const int n = opt.get<int>("n", 100000);
    const long long seed = opt.get<long long>("seed", 1);
    cp_stress_gen::core::require(n > 0, "n must be positive");

    cp_stress_gen::core::Random rng(static_cast<unsigned long long>(seed));
    const auto start = std::chrono::high_resolution_clock::now();
    const auto text = cp_stress_gen::String(static_cast<std::size_t>(n)).lowercase().build(rng);
    const auto finish = std::chrono::high_resolution_clock::now();

    unsigned long long checksum = 0;
    for (const char ch : text) {
        checksum = checksum * 131ull + static_cast<unsigned char>(ch);
    }

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
    cp_stress_gen::core::Printer out(std::cout);
    out.line("string_ms", elapsed);
    out.line("n", text.size());
    out.line("checksum", checksum);
    return 0;
}
