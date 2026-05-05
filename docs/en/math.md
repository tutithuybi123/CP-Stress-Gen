# Math

`cp_stress_gen::Math` provides small helpers useful in generators.

## Functions

- `gcd(a, b)`: greatest common divisor.
- `lcm(a, b)`: least common multiple.
- `divisors(n)`: sorted positive divisors of `abs(n)`.
- `primes_up_to(n)`: sieve primes up to `n`.
- `is_prime(n)`: trial-division primality check.
- `random_prime(left, right, rng)`: random prime in range.
- `random_composite(left, right, rng)`: random composite in range.
- `coprime_pair(left, right, rng)`: pair with gcd `1`.
- `with_gcd(g, multiplier_left, multiplier_right, rng)`: pair with gcd exactly `g` when a coprime multiplier pair is available.

```cpp
cp_stress_gen::core::Random rng(6);
int p = cp_stress_gen::Math::random_prime(10, 100, rng);
auto pair = cp_stress_gen::Math::with_gcd(6, 1, 20, rng);
```

Large-range prime generation is intentionally deferred.
