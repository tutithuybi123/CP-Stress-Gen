#include "cp_stress_gen.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    cp_stress_gen::core::Options opt(argc, argv);
    if (opt.flag("help")) {
        std::cout << "usage: options_basic --n 10 --seed 1 --mode random [--hard]\n";
        return 0;
    }

    const int n = opt.get<int>("n", 10);
    const long long seed = opt.get<long long>("seed", 1);
    const std::string mode = opt.get<std::string>("mode", "random");
    const bool hard = opt.flag("hard");

    cp_stress_gen::core::Random rng(static_cast<unsigned long long>(seed));
    cp_stress_gen::core::Printer out(std::cout);

    auto values = hard
        ? cp_stress_gen::Array(n).many_equal(3).build(rng)
        : cp_stress_gen::Array(n).range(1, 100).build(rng);

    out.line("mode", mode);
    out.line(n);
    out.vector(values);
    return 0;
}
