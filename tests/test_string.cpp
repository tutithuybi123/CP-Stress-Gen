#include "cp_stress_gen.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

static bool contains_only(const std::string& value, const std::string& alphabet) {
    for (const char ch : value) {
        if (alphabet.find(ch) == std::string::npos) {
            return false;
        }
    }
    return true;
}

int main() {
    cp_stress_gen::core::Random rng(4);

    const auto lower = cp_stress_gen::String(30).lowercase().build(rng);
    assert(lower.size() == 30);
    assert(contains_only(lower, "abcdefghijklmnopqrstuvwxyz"));

    const auto binary = cp_stress_gen::String(20).binary().build(rng);
    assert(binary.size() == 20);
    assert(contains_only(binary, "01"));

    const auto ranged = cp_stress_gen::String(15).range('x', 'z').build(rng);
    assert(ranged.size() == 15);
    assert(contains_only(ranged, "xyz"));

    cp_stress_gen::core::Random a(99);
    cp_stress_gen::core::Random b(99);
    assert(cp_stress_gen::String(25).digits().build(a) == cp_stress_gen::String(25).digits().build(b));

    bool thrown = false;
    try {
        (void)cp_stress_gen::String(3).alphabet("");
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    return 0;
}

