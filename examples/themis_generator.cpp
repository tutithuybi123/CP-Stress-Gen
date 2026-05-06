#include "cp_stress_gen.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace {

void write_case(std::ostream& output, cp_stress_gen::core::Random& rng, const int n) {
    cp_stress_gen::core::Printer out(output);
    const auto values = cp_stress_gen::Array(static_cast<std::size_t>(n)).range(1, 1000000).build(rng);
    out.line(n);
    out.vector(values);
}

} // namespace

int main(int argc, char** argv) {
    cp_stress_gen::core::Options opt(argc, argv);
    if (opt.flag("help")) {
        std::cout << "usage: themis_generator --tests 5 --n 20 --seed 1 [--out case]\n";
        return 0;
    }

    const int tests = opt.get<int>("tests", 3);
    const int n = opt.get<int>("n", 20);
    const long long seed = opt.get<long long>("seed", 1);
    cp_stress_gen::core::require(tests > 0, "tests must be positive");
    cp_stress_gen::core::require(n > 0, "n must be positive");
    cp_stress_gen::core::Random rng(static_cast<unsigned long long>(seed));

    if (!opt.has("out")) {
        cp_stress_gen::core::Printer out(std::cout);
        out.line(tests);
        for (int tc = 1; tc <= tests; ++tc) {
            write_case(std::cout, rng, n);
        }
        return 0;
    }

    const std::string prefix = opt.get<std::string>("out", "case");
    for (int tc = 1; tc <= tests; ++tc) {
        const std::string path = prefix + "_" + std::to_string(tc) + ".inp";
        std::ofstream file(path.c_str());
        cp_stress_gen::core::require(static_cast<bool>(file), "cannot open output file: " + path);
        write_case(file, rng, n);
    }
    return 0;
}
