# CP-Stress-Gen API Reference

This page summarizes the public C++17 API exposed by `include/cp_stress_gen.hpp`. The library is header-only and uses `std::invalid_argument` for invalid generator configurations.

## Core

### `core::Random`

Purpose: fast deterministic random generation based on Xoshiro256**.

Common API:

- `Random(seed)`
- `next_u64()`, `next_u32()`
- `integer<T>(left, right)`
- `real<T>(left, right)`
- `boolean()`, `boolean(probability_true)`

Example:

```cpp
cp_stress_gen::core::Random rng(42);
int x = rng.integer<int>(1, 100);
```

Validation: invalid integer or real ranges throw.

Limitations: this is not a cryptographic random generator.

### `core::Validate`

Purpose: shared validation helpers for generators.

Common API:

- `require(condition, message)`
- `require_range(left, right, message)`
- `require_positive(value, message)`

Example:

```cpp
cp_stress_gen::core::require(n > 0, "n must be positive");
```

Validation: failed checks throw `std::invalid_argument`.

Limitations: helpers intentionally stay small and do not define custom exception types.

### `core::Printer`

Purpose: print generated data to an `std::ostream`.

Common API:

- `line(args...)`
- `space(args...)`
- `vector(container)`
- `edges(container)`

Example:

```cpp
cp_stress_gen::core::Printer out(std::cout);
out.vector(std::vector<int>{1, 2, 3});
```

Validation: no runtime validation beyond stream behavior.

Limitations: `edges()` expects edge objects with `u`, `v`, `w`, and `weighted` fields.

## Modules

### `Array`

Purpose: generate integer arrays.

Common API:

- `range(left, right)`
- `fill(value)`
- `iota(start, step)`
- `shuffle()`
- `almost_sorted(swaps)`
- `many_equal(distinct_values)`
- `blocky(block_size)`

Example:

```cpp
auto a = cp_stress_gen::Array(10).range(1, 100).build(rng);
```

Validation: invalid ranges, zero distinct counts, zero block sizes, and excessive swap counts throw.

Limitations: values are `long long`; `almost_sorted` starts from `1..n`.

### `String`

Purpose: generate strings from alphabets and simple patterns.

Common API:

- `alphabet(chars)`
- `lowercase()`, `uppercase()`, `digits()`, `binary()`
- `range(left, right)`
- `palindrome()`
- `almost_palindrome(changes)`
- `periodic(pattern)`
- `prefix_heavy(prefix_char, tail_char)`

Example:

```cpp
auto s = cp_stress_gen::String(12).periodic("abc").build(rng);
```

Validation: empty alphabets, invalid ranges, empty periodic patterns, and excessive almost-palindrome changes throw.

Limitations: string generation is byte-oriented and does not handle Unicode grapheme logic.

### `Permutation`

Purpose: generate permutations of consecutive integer labels.

Common API:

- `one_based()`, `zero_based()`
- `shuffle()`
- `reversed()`
- `almost_sorted(swaps)`
- `cyclic_shift(k)`
- `few_swaps(count)`
- `blocks(block_size)`

Example:

```cpp
auto p = cp_stress_gen::Permutation(8).zero_based().shuffle().build(rng);
```

Validation: size must fit in `int`; excessive swap counts and zero block size throw.

Limitations: values are consecutive `int` labels only.

### `Tree`

Purpose: generate valid tree edge lists.

Common API:

- `one_based()`, `zero_based()`, `first_node(first)`
- `weight(value)`, `weighted(left, right)`
- `bamboo()`, `star()`, `star(center)`, `random()`
- `degree_limit(limit)` for random trees
- `binary()`, `caterpillar(spine)`, `deep_recursion(branches)`
- `shuffle()`

Example:

```cpp
auto edges = cp_stress_gen::Tree(10).weighted(1, 100).random().build(rng);
```

Validation: invalid labels, centers, spine sizes, branch counts, degree limits, non-random degree-limit use, and weight ranges throw.

Limitations: degree limits currently apply only to `random()` trees; arbitrary tree families are not guaranteed.

### `Graph`

Purpose: generate graph edge lists.

Common API:

- `edges(m)`
- `directed()`, `undirected()`
- `one_based()`, `zero_based()`, `first_node(first)`
- `no_multi_edges()`, `allow_multi_edges()`
- `weight(value)`, `weighted(left, right)`
- `dag()`, `layered_dag(layers)`, `dense()`
- `sparse_connected()`, `complete()`, `cycle()`, `path()`
- `forest(components)`, `connected_components(k)`, `bipartite(left_size)`
- `wheel()`, `grid(rows, cols)`, `complete_bipartite(left, right)`, `tournament()`
- `shuffle()`

Example:

```cpp
auto edges = cp_stress_gen::Graph(8).edges(12).dag().no_multi_edges().build(rng);
```

Validation: impossible edge counts, invalid partitions, invalid layer counts, invalid structure sizes, and incompatible directed modes throw.

Limitations: advanced graph families and weighted shortest-path adversarial cases are intentionally deferred.

### `Math`

Purpose: small math helpers useful in generators.

Common API:

- `gcd(a, b)`, `lcm(a, b)`
- `divisors(n)`
- `primes_up_to(n)`, `is_prime(n)`
- `random_prime(left, right, rng)`
- `random_composite(left, right, rng)`
- `coprime_pair(left, right, rng)`
- `with_gcd(g, multiplier_left, multiplier_right, rng)`
- `mod_pow(base, exponent, mod)`
- `extended_gcd(a, b)`
- `mod_inverse(value, mod)`
- `factorize_trial(value)`

Example:

```cpp
auto pair = cp_stress_gen::Math::with_gcd(6, 1, 20, rng);
```

Validation: invalid ranges, impossible prime/composite/coprime requests, invalid modular parameters, and missing modular inverses throw.

Limitations: primality and factor-related helpers use simple algorithms and do not target huge ranges.

### `Geometry`

Purpose: integer point generation and simple shapes.

Common API:

- `Point<T>`
- `Geometry::points(count).rectangle(...).unique()`
- `Geometry::collinear(count).from(...).step(...)`
- `Geometry::rectangle_boundary_points(count).rectangle(...)`
- `Geometry::clustered_points(count).center(...).radius(...)`
- `Geometry::duplicate_points(count).point(...)`
- `Geometry::rectangle_polygon(x1, y1, x2, y2)`
- `Geometry::triangle(a, b, c)`

Example:

```cpp
auto points = cp_stress_gen::Geometry::points(9).rectangle(1, 1, 3, 3).unique().build(rng);
```

Validation: invalid rectangles, impossible unique requests, invalid collinear steps, negative cluster radii, and degenerate simple polygons throw.

Limitations: arbitrary convex polygon generation is not implemented.

## Anti Pattern Modules

Purpose: provide honest, safe pattern helpers for local stress testing.

Common API:

- `anti::SortKiller::reversed`, `nearly_sorted`, `many_duplicates`
- `anti::SortKiller::organ_pipe`, `alternating_high_low`
- `anti::TreeKiller::chain`, `star`, `chain_with_branches`, `broom`
- `anti::GraphKiller::dense`, `long_dag_chain`, `sparse_connected`, `dense_dag`
- `anti::StringKiller::repeated`, `alternating`, `prefix_heavy`, `kmp_prefix_pattern`

Example:

```cpp
auto values = cp_stress_gen::anti::SortKiller::many_duplicates(20, 3, rng);
```

Validation: invalid sizes or counts throw where applicable.

Limitations: these names describe input shapes. They do not guarantee that a specific algorithm will fail. Hash collisions, Dijkstra-specific patterns, and risky adversarial generators are not implemented.
