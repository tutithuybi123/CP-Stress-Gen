#include "cp_stress_gen.hpp"

#include <cassert>
#include <iostream>
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

    assert(cp_stress_gen::String(0).lowercase().build(rng).empty());
    assert(cp_stress_gen::String(1).prefix_heavy('x', 'y').build(rng) == "y");

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

    const auto palindrome = cp_stress_gen::String(9).alphabet("abc").palindrome().build(rng);
    assert(palindrome.size() == 9);
    for (std::size_t i = 0; i < palindrome.size(); ++i) {
        assert(palindrome[i] == palindrome[palindrome.size() - 1 - i]);
    }

    const auto almost = cp_stress_gen::String(8).alphabet("ab").almost_palindrome(2).build(rng);
    assert(almost.size() == 8);

    assert(cp_stress_gen::String(8).periodic("abc").build(rng) == "abcabcab");
    assert(cp_stress_gen::String(5).prefix_heavy('x', 'y').build(rng) == "xxxxy");

    cp_stress_gen::core::Random same_a(88);
    cp_stress_gen::core::Random same_b(88);
    assert(cp_stress_gen::String(16).alphabet("abcd").palindrome().build(same_a) == cp_stress_gen::String(16).alphabet("abcd").palindrome().build(same_b));
    assert(cp_stress_gen::String(16).alphabet("abcd").almost_palindrome(3).build(same_a) == cp_stress_gen::String(16).alphabet("abcd").almost_palindrome(3).build(same_b));

    bool thrown = false;
    try {
        (void)cp_stress_gen::String(3).alphabet("");
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::String(3).range('z', 'a');
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::String(3).periodic("");
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::String(3).almost_palindrome(2);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] test_string\n";
    return 0;
}
