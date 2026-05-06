#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Printer out(std::cout);

    const auto pair = cp_stress_gen::StringHash::thue_morse_pair_power2(10);
    const unsigned long long base = 911382323ull;

    out.line(pair.first.size());
    out.line(pair.first);
    out.line(pair.second);
    out.line(cp_stress_gen::StringHash::hash_u64(pair.first, base));
    out.line(cp_stress_gen::StringHash::hash_u64(pair.second, base));

    return 0;
}
