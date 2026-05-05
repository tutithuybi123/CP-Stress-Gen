#include "cp_stress_gen.hpp"

#include <iostream>

int main() {
    cp_stress_gen::core::Random rng(102);
    cp_stress_gen::core::Printer out(std::cout);

    const auto palindrome = cp_stress_gen::String(11).alphabet("abc").palindrome().build(rng);
    const auto periodic = cp_stress_gen::String(12).periodic("abca").build(rng);

    out.line(2);
    out.line(palindrome.size(), palindrome);
    out.line(periodic.size(), periodic);
    return 0;
}
