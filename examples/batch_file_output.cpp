#include "cp_stress_gen.hpp"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    cp_stress_gen::core::Options opt(argc, argv);
    if (opt.flag("help")) {
        std::cout << "usage: batch_file_output --tests 5 --seed 1 --out generated_case\n";
        return 0;
    }

    const int tests = opt.get<int>("tests", 3);
    const long long seed = opt.get<long long>("seed", 1);
    const std::string prefix = opt.get<std::string>("out", "generated_case");
    cp_stress_gen::core::require(tests > 0, "tests must be positive");

    cp_stress_gen::core::Random rng(static_cast<unsigned long long>(seed));
    for (int tc = 1; tc <= tests; ++tc) {
        const std::string path = prefix + "_" + std::to_string(tc) + ".inp";
        std::ofstream file(path.c_str());
        cp_stress_gen::core::require(static_cast<bool>(file), "cannot open output file: " + path);

        cp_stress_gen::core::Printer out(file);
        const int n = 5 + tc;
        const auto values = cp_stress_gen::Array(static_cast<std::size_t>(n)).range(1, 100).build(rng);
        out.line(n);
        out.vector(values);
    }

    std::cout << "[PASS] wrote " << tests << " input files with prefix " << prefix << "\n";
    return 0;
}
