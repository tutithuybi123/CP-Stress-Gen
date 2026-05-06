#include "cp_stress_gen.hpp"

#include <iostream>
#include <vector>

int main() {
    cp_stress_gen::core::Printer out(std::cout);

    out.line(cp_stress_gen::String::thue_morse(16));
    out.line(cp_stress_gen::String::fibonacci_word(20));
    out.line(cp_stress_gen::String::border_chain(12));
    out.line(cp_stress_gen::String::periodic_blocks("abc", 4));
    out.line(cp_stress_gen::String::runs(std::vector<std::pair<char, std::size_t>>{
        {'a', 3},
        {'b', 2},
        {'c', 4}
    }));
    out.line(cp_stress_gen::String::de_bruijn("01", 4));

    return 0;
}
