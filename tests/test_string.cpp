#include "cp_stress_gen.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

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
    assert(cp_stress_gen::String(5).alphabet("ab").border_heavy().build(rng) == "aaaaa");
    assert(cp_stress_gen::String(7).alphabet("ab").runs(2).build(rng) == "aabbaab");

    const auto noisy = cp_stress_gen::String(6).alphabet("abc").periodic_with_noise("ab", 2).build(rng);
    assert(noisy.size() == 6);
    std::size_t differences = 0;
    const std::string base_periodic = "ababab";
    for (std::size_t i = 0; i < noisy.size(); ++i) {
        if (noisy[i] != base_periodic[i]) {
            ++differences;
        }
    }
    assert(differences == 2);

    const auto pal_blocks = cp_stress_gen::String(10).alphabet("abc").random_palindromic_blocks(4).build(rng);
    assert(pal_blocks.size() == 10);
    for (std::size_t begin = 0; begin < pal_blocks.size(); begin += 4) {
        const std::size_t end = std::min<std::size_t>(pal_blocks.size(), begin + 4);
        for (std::size_t left = begin, right = end - 1; left <= right; ++left, --right) {
            assert(pal_blocks[left] == pal_blocks[right]);
            if (right == 0) {
                break;
            }
        }
    }

    const auto de_bruijn = cp_stress_gen::String::de_bruijn_binary(3);
    assert(de_bruijn.size() == 10);
    std::set<std::string> seen_binary;
    for (std::size_t i = 0; i + 3 <= de_bruijn.size(); ++i) {
        seen_binary.insert(de_bruijn.substr(i, 3));
    }
    assert(seen_binary.size() == 8);

    assert(cp_stress_gen::StringHash::hash_string("ab", 31, 1000) == 105);
    assert(cp_stress_gen::StringHash::same_hash("abc", "abc", 911382323, 1000000007));
    assert(!cp_stress_gen::StringHash::same_hash("abc", "abd", 31, 1000000007));
    const auto collision_like = cp_stress_gen::StringHash::collision_like_pair(4);
    assert(collision_like.first == "aaab");
    assert(collision_like.second == "baaa");

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

    thrown = false;
    try {
        (void)cp_stress_gen::String(3).runs(0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::String(3).periodic_with_noise("", 0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::String(3).periodic_with_noise("a", 4);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::String(3).random_palindromic_blocks(0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::String::de_bruijn_binary(0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::StringHash::hash_string("a", 31, 1);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        (void)cp_stress_gen::StringHash::collision_like_pair(0);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "[PASS] test_string\n";
    return 0;
}
