#include "cp_stress_gen.hpp"

#include <cassert>
#include <cstdint>
#include <stdexcept>

int main() {
    cp_stress_gen::core::Random a(123456789);
    cp_stress_gen::core::Random b(123456789);

    for (int i = 0; i < 1000; ++i) {
        assert(a.next_u64() == b.next_u64());
    }

    cp_stress_gen::core::Random rng(7);
    for (int i = 0; i < 1000; ++i) {
        const int value = rng.integer<int>(-5, 5);
        assert(value >= -5 && value <= 5);

        const double real = rng.real<double>(2.0, 3.0);
        assert(real >= 2.0 && real < 3.0);
    }

    assert(!rng.boolean(0.0));
    assert(rng.boolean(1.0));

    bool thrown = false;
    try {
        (void)rng.integer<int>(10, 1);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    return 0;
}

